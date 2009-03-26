// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "COctTreeSceneNode.h"
#include "OctTree.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"
#include "ICameraSceneNode.h"
#include "IMeshCache.h"
#include "IAnimatedMesh.h"
#include "IMaterialRenderer.h"

#include "os.h"

namespace irr
{
namespace scene
{


//! constructor
COctTreeSceneNode::COctTreeSceneNode(ISceneNode* parent, ISceneManager* mgr,
					 s32 id, s32 minimalPolysPerNode)
: IMeshSceneNode(parent, mgr, id), StdOctTree(0), LightMapOctTree(0), TangentsOctTree(0),
	MinimalPolysPerNode(minimalPolysPerNode), Mesh(0)
{
#ifdef _DEBUG
	setDebugName("COctTreeSceneNode");
#endif

	vertexType = (video::E_VERTEX_TYPE)-1;
}


//! destructor
COctTreeSceneNode::~COctTreeSceneNode()
{
	deleteTree();
}


void COctTreeSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		// because this node supports rendering of mixed mode meshes consisting of
		// transparent and solid material at the same time, we need to go through all
		// materials, check of what type they are and register this node for the right
		// render pass according to that.

		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		PassCount = 0;
		u32 transparentCount = 0;
		u32 solidCount = 0;

		// count transparent and solid materials in this scene node
		for (u32 i=0; i<Materials.size(); ++i)
		{
			const video::IMaterialRenderer* const rnd =
				driver->getMaterialRenderer(Materials[i].MaterialType);

			if (rnd && rnd->isTransparent())
				++transparentCount;
			else
				++solidCount;

			if (solidCount && transparentCount)
				break;
		}

		// register according to material types counted

		if (solidCount)
			SceneManager->registerNodeForRendering(this, scene::ESNRP_SOLID);

		if (transparentCount)
			SceneManager->registerNodeForRendering(this, scene::ESNRP_TRANSPARENT);

		ISceneNode::OnRegisterSceneNode();
	}
}


//! renders the node.
void COctTreeSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	if (vertexType == -1 || !driver)
		return;

	ICameraSceneNode* camera = SceneManager->getActiveCamera();
	if (!camera)
		return;

	bool isTransparentPass =
		SceneManager->getSceneNodeRenderPass() == scene::ESNRP_TRANSPARENT;
	++PassCount;

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	SViewFrustum frust = *camera->getViewFrustum();

	//transform the frustum to the current absolute transformation
	if ( !AbsoluteTransformation.isIdentity() )
	{
		core::matrix4 invTrans(AbsoluteTransformation, core::matrix4::EM4CONST_INVERSE);
		frust.transform(invTrans);
	}

#if defined ( OCTTREE_BOX_BASED )
	const core::aabbox3d<float> &box = frust.getBoundingBox();
#endif

	switch(vertexType)
	{
	case video::EVT_STANDARD:
		{
#if defined ( OCTTREE_BOX_BASED )
			StdOctTree->calculatePolys(box);
#else
			StdOctTree->calculatePolys(frust);
#endif

			const OctTree<video::S3DVertex>::SIndexData* d = StdOctTree->getIndexData();

			for (u32 i=0; i<Materials.size(); ++i)
			{
				if ( 0 == d[i].CurrentSize )
					continue;

				const video::IMaterialRenderer* const rnd = driver->getMaterialRenderer(Materials[i].MaterialType);
				const bool transparent = (rnd && rnd->isTransparent());

				// only render transparent buffer if this is the transparent render pass
				// and solid only in solid pass
				if (transparent == isTransparentPass)
				{
					driver->setMaterial(Materials[i]);
					driver->drawIndexedTriangleList(
						&StdMeshes[i].Vertices[0], StdMeshes[i].Vertices.size(),
						d[i].Indices, d[i].CurrentSize / 3);
				}
			}

			// for debug purposes only
			if (DebugDataVisible && !Materials.empty() && PassCount==1)
			{
				const core::aabbox3df& box = frust.getBoundingBox();
				core::array< const core::aabbox3d<f32>* > boxes;
				video::SMaterial m;
				m.Lighting = false;
				driver->setMaterial(m);
				if ( DebugDataVisible & scene::EDS_BBOX_BUFFERS )
				{
					StdOctTree->getBoundingBoxes(box, boxes);
					for (u32 b=0; b!=boxes.size(); ++b)
						driver->draw3DBox(*boxes[b]);
				}

				if ( DebugDataVisible & scene::EDS_BBOX )
					driver->draw3DBox(Box,video::SColor(0,255,0,0));
			}
		}
		break;
	case video::EVT_2TCOORDS:
		{
#if defined ( OCTTREE_BOX_BASED )
			LightMapOctTree->calculatePolys(box);
#else
			LightMapOctTree->calculatePolys(frust);
#endif
			const OctTree<video::S3DVertex2TCoords>::SIndexData* d = LightMapOctTree->getIndexData();

			for (u32 i=0; i<Materials.size(); ++i)
			{
				if ( 0 == d[i].CurrentSize )
					continue;

				const video::IMaterialRenderer* const rnd = driver->getMaterialRenderer(Materials[i].MaterialType);
				const bool transparent = (rnd && rnd->isTransparent());

				// only render transparent buffer if this is the transparent render pass
				// and solid only in solid pass
				if (transparent == isTransparentPass)
				{
					driver->setMaterial(Materials[i]);
#if defined (OCTTREE_USE_HARDWARE)
					driver->drawMeshBuffer ( &LightMapMeshes[i] );

#else
					driver->drawIndexedTriangleList(
						&LightMapMeshes[i].Vertices[0], LightMapMeshes[i].Vertices.size(),
						d[i].Indices, d[i].CurrentSize / 3);
#endif
				}
			}

			// for debug purposes only
			if (DebugDataVisible && !Materials.empty() && PassCount==1)
			{
				const core::aabbox3d<float> &box = frust.getBoundingBox();
				core::array< const core::aabbox3d<f32>* > boxes;
				video::SMaterial m;
				m.Lighting = false;
				driver->setMaterial(m);
				if ( DebugDataVisible & scene::EDS_BBOX_BUFFERS )
				{
					LightMapOctTree->getBoundingBoxes(box, boxes);
					for (u32 b=0; b<boxes.size(); ++b)
						driver->draw3DBox(*boxes[b]);
				}

				if ( DebugDataVisible & scene::EDS_BBOX )
					driver->draw3DBox(Box,video::SColor(0,255,0,0));
			}
		}
		break;
	case video::EVT_TANGENTS:
		{
#if defined ( OCTTREE_BOX_BASED )
			TangentsOctTree->calculatePolys(box);
#else
			TangentsOctTree->calculatePolys(frust);
#endif

			const OctTree<video::S3DVertexTangents>::SIndexData* d =  TangentsOctTree->getIndexData();

			for (u32 i=0; i<Materials.size(); ++i)
			{
				if ( 0 == d[i].CurrentSize )
					continue;

				const video::IMaterialRenderer* const rnd = driver->getMaterialRenderer(Materials[i].MaterialType);
				const bool transparent = (rnd && rnd->isTransparent());

				// only render transparent buffer if this is the transparent render pass
				// and solid only in solid pass
				if (transparent == isTransparentPass)
				{
					driver->setMaterial(Materials[i]);
					driver->drawIndexedTriangleList(
						&TangentsMeshes[i].Vertices[0], TangentsMeshes[i].Vertices.size(),
						d[i].Indices, d[i].CurrentSize / 3);
				}
			}

			// for debug purposes only
			if (DebugDataVisible && !Materials.empty() && PassCount==1)
			{
				const core::aabbox3d<float> &box = frust.getBoundingBox();
				core::array< const core::aabbox3d<f32>* > boxes;
				video::SMaterial m;
				m.Lighting = false;
				driver->setMaterial(m);
				if ( DebugDataVisible & scene::EDS_BBOX_BUFFERS )
				{
					TangentsOctTree->getBoundingBoxes(box, boxes);
					for (u32 b=0; b<boxes.size(); ++b)
						driver->draw3DBox(*boxes[b]);
				}

				if ( DebugDataVisible & scene::EDS_BBOX )
					driver->draw3DBox(Box,video::SColor(0,255,0,0));
			}
		}
		break;
	}
}


//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& COctTreeSceneNode::getBoundingBox() const
{
	return Box;
}


//! creates the tree
bool COctTreeSceneNode::createTree(IMesh* mesh)
{
	if (!mesh)
		return false;

	MeshName = SceneManager->getMeshCache()->getMeshFilename( mesh );

	deleteTree();

	Mesh = mesh;
	Mesh->grab();

	u32 beginTime = os::Timer::getRealTime();

	u32 nodeCount = 0;
	u32 polyCount = 0;
	u32 i;

	Box = mesh->getBoundingBox();

	if (mesh->getMeshBufferCount())
	{
		vertexType = mesh->getMeshBuffer(0)->getVertexType();

		switch(vertexType)
		{
		case video::EVT_STANDARD:
			{
				for (i=0; i<mesh->getMeshBufferCount(); ++i)
				{
					IMeshBuffer* b = mesh->getMeshBuffer(i);

					if (b->getVertexCount() && b->getIndexCount()) 
					{
						Materials.push_back(b->getMaterial());

						StdMeshes.push_back(OctTree<video::S3DVertex>::SMeshChunk());
						OctTree<video::S3DVertex>::SMeshChunk &nchunk = StdMeshes.getLast();
						nchunk.MaterialId = Materials.size() - 1;

						u32 v;
						nchunk.Vertices.reallocate(b->getVertexCount());
						for (v=0; v<b->getVertexCount(); ++v)
							nchunk.Vertices.push_back(((video::S3DVertex*)b->getVertices())[v]);

						polyCount += b->getIndexCount();

						nchunk.Indices.reallocate(b->getIndexCount());
						for (v=0; v<b->getIndexCount(); ++v)
							nchunk.Indices.push_back(b->getIndices()[v]);
					}
				}

				StdOctTree = new OctTree<video::S3DVertex>(StdMeshes, MinimalPolysPerNode);
				nodeCount = StdOctTree->getNodeCount();
			}
			break;
		case video::EVT_2TCOORDS:
			{
				IMeshBuffer* b;
				u32 meshReserve = 0;
				for ( i=0; i < mesh->getMeshBufferCount(); ++i)
				{
					b = mesh->getMeshBuffer(i);
					if (b->getVertexCount() && b->getIndexCount()) 
					{
						meshReserve += 1;
					}

				}
				LightMapMeshes.reallocate ( LightMapMeshes.size() + meshReserve );

				for ( i=0; i < mesh->getMeshBufferCount(); ++i)
				{
					b = mesh->getMeshBuffer(i);

					if (b->getVertexCount() && b->getIndexCount()) 
					{
						Materials.push_back(b->getMaterial());
						LightMapMeshes.push_back(OctTree<video::S3DVertex2TCoords>::SMeshChunk());
						OctTree<video::S3DVertex2TCoords>::SMeshChunk& nchunk = LightMapMeshes.getLast();
						nchunk.MaterialId = Materials.size() - 1;

#if defined (OCTTREE_USE_HARDWARE)
						nchunk.setHardwareMappingHint ( b->getHardwareMappingHint_Vertex() );
#endif

						u32 v;
						nchunk.Vertices.reallocate(b->getVertexCount());
						for (v=0; v<b->getVertexCount(); ++v)
							nchunk.Vertices.push_back(((video::S3DVertex2TCoords*)b->getVertices())[v]);

						polyCount += b->getIndexCount();
						nchunk.Indices.reallocate(b->getIndexCount());
						for (v=0; v<b->getIndexCount(); ++v)
							nchunk.Indices.push_back(b->getIndices()[v]);
					}
				}

				LightMapOctTree = new OctTree<video::S3DVertex2TCoords>(LightMapMeshes, MinimalPolysPerNode);
				nodeCount = LightMapOctTree->getNodeCount();
			}
			break;
		case video::EVT_TANGENTS:
			{
				for (u32 i=0; i<mesh->getMeshBufferCount(); ++i)
				{
					IMeshBuffer* b = mesh->getMeshBuffer(i);

					if (b->getVertexCount() && b->getIndexCount()) 
					{
						Materials.push_back(b->getMaterial());
						TangentsMeshes.push_back(OctTree<video::S3DVertexTangents>::SMeshChunk());
						OctTree<video::S3DVertexTangents>::SMeshChunk& nchunk = TangentsMeshes.getLast();
						nchunk.MaterialId = Materials.size() - 1;

						u32 v;
						nchunk.Vertices.reallocate(b->getVertexCount());
						for (v=0; v<b->getVertexCount(); ++v)
							nchunk.Vertices.push_back(((video::S3DVertexTangents*)b->getVertices())[v]);

						polyCount += b->getIndexCount();
						nchunk.Indices.reallocate(b->getIndexCount());
						for (v=0; v<b->getIndexCount(); ++v)
							nchunk.Indices.push_back(b->getIndices()[v]);
					}
				}

				TangentsOctTree = new OctTree<video::S3DVertexTangents>(TangentsMeshes, MinimalPolysPerNode);
				nodeCount = TangentsOctTree->getNodeCount();
			}
			break;
		}
	}

	u32 endTime = os::Timer::getRealTime();
	c8 tmp[255];
	sprintf(tmp, "Needed %ums to create OctTree SceneNode.(%u nodes, %u polys)",
		endTime - beginTime, nodeCount, polyCount/3);
	os::Printer::log(tmp, ELL_INFORMATION);

	return true;
}


//! returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hirachy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
video::SMaterial& COctTreeSceneNode::getMaterial(u32 i)
{
	if ( i >= Materials.size() )
		return ISceneNode::getMaterial(i);

	return Materials[i];
}


//! returns amount of materials used by this scene node.
u32 COctTreeSceneNode::getMaterialCount() const
{
	return Materials.size();
}


//! Writes attributes of the scene node.
void COctTreeSceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	ISceneNode::serializeAttributes(out, options);

	out->addInt("MinimalPolysPerNode", MinimalPolysPerNode);
	out->addString("Mesh", MeshName.c_str());
}


//! Reads attributes of the scene node.
void COctTreeSceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	const s32 oldMinimal = MinimalPolysPerNode;

	MinimalPolysPerNode = in->getAttributeAsInt("MinimalPolysPerNode");
	core::string<c16> newMeshStr = in->getAttributeAsString("Mesh");

	IMesh* newMesh = 0;

	if (newMeshStr == "")
		newMeshStr = MeshName;

	IAnimatedMesh* newAnimatedMesh = SceneManager->getMesh(newMeshStr.c_str());

	if (newAnimatedMesh)
		newMesh = newAnimatedMesh->getMesh(0);

	if (newMesh && ((MeshName != newMeshStr) || (MinimalPolysPerNode != oldMinimal)))
	{
		// recalculate tree
		createTree(newMesh);
	}

	ISceneNode::deserializeAttributes(in, options);
}


void COctTreeSceneNode::deleteTree()
{
	delete StdOctTree;
	StdOctTree = 0;
	StdMeshes.clear();

	delete LightMapOctTree;
	LightMapOctTree = 0;
	LightMapMeshes.clear();

	delete TangentsOctTree;
	TangentsOctTree = 0;
	TangentsMeshes.clear();

	Materials.clear();

	if(Mesh)
		Mesh->drop();
}

void COctTreeSceneNode::setMesh(IMesh* mesh)
{
	createTree(mesh);
}

IMesh* COctTreeSceneNode::getMesh(void)
{
	return Mesh;
}

void COctTreeSceneNode::setReadOnlyMaterials(bool readonly)
{
	// Do nothing
}

bool COctTreeSceneNode::isReadOnlyMaterials() const
{
	return false;
}


} // end namespace scene
} // end namespace irr

