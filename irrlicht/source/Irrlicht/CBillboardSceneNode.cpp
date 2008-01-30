// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CBillboardSceneNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "ICameraSceneNode.h"
#include "os.h"

namespace irr
{
namespace scene
{

//! constructor
CBillboardSceneNode::CBillboardSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
				const core::vector3df& position, const core::dimension2d<f32>& size,
				video::SColor shade_top, video::SColor shade_down)
	: IBillboardSceneNode(parent, mgr, id, position)
{
	#ifdef _DEBUG
	setDebugName("CBillboardSceneNode");
	#endif

	setSize(size);

	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;
	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 2;

	vertices[0].TCoords.set(1.0f, 1.0f);
	vertices[0].Color = shade_down;

	vertices[1].TCoords.set(1.0f, 0.0f);
	vertices[1].Color = shade_top;

	vertices[2].TCoords.set(0.0f, 0.0f);
	vertices[2].Color = shade_top;

	vertices[3].TCoords.set(0.0f, 1.0f);
	vertices[3].Color = shade_down;

	Type = PointAligned;
	AxisAlignment.set(0.f, 1.f, 0.f);
}


//! pre render event
void CBillboardSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		SceneManager->registerNodeForRendering(this);
		ISceneNode::OnRegisterSceneNode();
	}
}


//! render
void CBillboardSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	ICameraSceneNode* camera = SceneManager->getActiveCamera();

	if (!camera || !driver)
		return;

	core::vector3df centreOfBottomEdge(getAbsolutePosition());
	core::vector3df centreOfTopEdge(centreOfBottomEdge);
	core::vector3df halfWidthOfBottomEdge;
	core::vector3df halfWidthOfTopEdge;
	core::vector3df upVector;

	if(PointAligned == Type)
	{
		core::vector3df cameraView(camera->getTarget() - camera->getAbsolutePosition());
		cameraView.normalize();

		// Always set the up vector to be opposite the camera's view vector.
		upVector = cameraView * -1.f;

		// The width of the billboard is orthogonal to the camera's up and view vectors.
		halfWidthOfBottomEdge = camera->getUpVector().crossProduct(cameraView);
		if(core::equals(halfWidthOfBottomEdge.getLengthSQ(), 0.f))
			halfWidthOfBottomEdge.set(camera->getUpVector());

		halfWidthOfBottomEdge.normalize();

		// The height of the billboard is orthogonal to the width and the camera's view.
		core::vector3df halfAxis(halfWidthOfBottomEdge.crossProduct(cameraView));

		halfAxis.normalize();
		halfAxis *= Size.Height * 0.5f;

		centreOfBottomEdge -= halfAxis;
		centreOfTopEdge += halfAxis;

		halfWidthOfTopEdge = halfWidthOfBottomEdge;
		halfWidthOfBottomEdge *= Size.Width * 0.5f;
		halfWidthOfTopEdge *= TopEdgeWidth * 0.5f;
	}
	else
	{
		// Find the locations of the centre of the top and bottom edges.  These
		// define the ends of the axis of rotation of the the billboard.
		core::vector3df axis(AxisAlignment * Size.Height);
		core::vector3df halfAxis(axis * 0.5f);
		centreOfBottomEdge -= halfAxis;
		centreOfTopEdge += halfAxis;

		// Find the point on the billboard that's closest to the camera, and use that
		// to calculate the direction to the camera.
		core::vector3df cameraPos = camera->getAbsolutePosition();
		core::vector3df closestPoint(centreOfBottomEdge);

		f32 distanceFromStart =
	(((cameraPos.X - centreOfBottomEdge.X) * (centreOfTopEdge.X - centreOfBottomEdge.X)) +
    ((cameraPos.Y - centreOfBottomEdge.Y) * (centreOfTopEdge.Y - centreOfBottomEdge.Y)) +
    ((cameraPos.Z - centreOfBottomEdge.Z) * (centreOfTopEdge.Z - centreOfBottomEdge.Z))) /
    (Size.Height * Size.Height);

		if(distanceFromStart > 1.f)
		{
			closestPoint = centreOfTopEdge;
		}
		else if(distanceFromStart > 0.f)
		{
			closestPoint += axis * distanceFromStart;
		}
		// else centreOfBottomEdge is closest, and that's already assumed

		// Face the billboard towards the camera from its closest point.
		upVector = camera->getAbsolutePosition() - closestPoint;
		upVector.normalize();

		halfWidthOfBottomEdge = AxisAlignment.crossProduct(upVector);
		if(core::equals(halfWidthOfBottomEdge.getLengthSQ(), 0.f))
			halfWidthOfBottomEdge.set(upVector);
		halfWidthOfBottomEdge.normalize();

		halfWidthOfTopEdge = halfWidthOfBottomEdge;
		halfWidthOfBottomEdge *= Size.Width * 0.5f;
		halfWidthOfTopEdge *= TopEdgeWidth * 0.5f;
	}
	/* Vertices are:
	3--0
	| /|
	|/ |
	2--1
	*/

	vertices[0].Normal = upVector;
	vertices[0].Pos = centreOfTopEdge + halfWidthOfTopEdge;

	vertices[1].Normal = upVector;
	vertices[1].Pos = centreOfBottomEdge + halfWidthOfBottomEdge;

	vertices[2].Normal = upVector;
	vertices[2].Pos = centreOfBottomEdge - halfWidthOfBottomEdge;

	vertices[3].Normal = upVector;
	vertices[3].Pos = centreOfTopEdge - halfWidthOfTopEdge;

	// draw

	if ( DebugDataVisible & scene::EDS_BBOX )
	{
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		video::SMaterial m;
		m.Lighting = false;
		driver->setMaterial(m);
		driver->draw3DBox(BBox, video::SColor(0,208,195,152));
	}

	core::matrix4 mat;
	driver->setTransform(video::ETS_WORLD, mat);

	driver->setMaterial(Material);

	driver->drawIndexedTriangleList(vertices, 4, indices, 2);
}


//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CBillboardSceneNode::getBoundingBox() const
{
	return BBox;
}


//! sets the size of the billboard
void CBillboardSceneNode::setSize(const core::dimension2d<f32>& size)
{
	Size = size;
	TopEdgeWidth = size.Width;

	if (core::equals(Size.Width, 0.f))
		Size.Width = 1.0f;

	if (core::equals(Size.Height, 0.f))
		Size.Height = 1.0f;

	if (core::equals(TopEdgeWidth, 0.f))
		TopEdgeWidth = 1.0f;

	calculateBoundingBox();
}

void CBillboardSceneNode::setWidths(f32 bottomEdgeWidth,
									f32 topEdgeWidth)
{
	Size.Width = bottomEdgeWidth;
	TopEdgeWidth = topEdgeWidth;

	if (core::equals(Size.Width, 0.f))
		Size.Width = 1.0f;

	if (core::equals(TopEdgeWidth, 0.f))
		TopEdgeWidth = 1.0f;

	calculateBoundingBox();
}


video::SMaterial& CBillboardSceneNode::getMaterial(u32 i)
{
	return Material;
}


//! returns amount of materials used by this scene node.
u32 CBillboardSceneNode::getMaterialCount() const
{
	return 1;
}


//! gets the size of the billboard
const core::dimension2d<f32>& CBillboardSceneNode::getSize() const
{
	return Size;
}

//! Gets the widths of the top and bottom edges of the billboard.
//! \param[out] bottomEdgeWidth The width of the bottom edge of the billboard.
//! \param[out] topEdgeWidth    The width of the top edge of the billboard.
void CBillboardSceneNode::getWidths(f32 & bottomEdgeWidth,
									f32 & topEdgeWidth) const
{
	bottomEdgeWidth = Size.Width;
	topEdgeWidth = TopEdgeWidth;
}


//! Writes attributes of the scene node.
void CBillboardSceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	IBillboardSceneNode::serializeAttributes(out, options);

	out->addFloat("BottomEdgeWidth", Size.Width);
	out->addFloat("TopEdgeWidth", TopEdgeWidth);
	out->addFloat("Height", Size.Height);
	out->addColor ("Shade_Top", vertices[1].Color );
	out->addColor ("Shade_Down", vertices[0].Color );
	out->addVector3d("AxisAlignment", AxisAlignment );
	out->addInt("BillboardType", static_cast<s32>(Type) );
	out->addVector2d("uvLeftTop", vertices[3].TCoords );
	out->addVector2d("uvRightTop", vertices[0].TCoords );
	out->addVector2d("uvLeftBottom", vertices[2].TCoords );
	out->addVector2d("uvRightBottom", vertices[1].TCoords );
}


//! Reads attributes of the scene node.
void CBillboardSceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	IBillboardSceneNode::deserializeAttributes(in, options);

	Size.Width = in->getAttributeAsFloat("BottomEdgeWidth");
	TopEdgeWidth = in->getAttributeAsFloat("TopEdgeWidth");
 	Size.Height = in->getAttributeAsFloat("Height");
 	vertices[1].Color = in->getAttributeAsColor ( "Shade_Top" );
 	vertices[0].Color = in->getAttributeAsColor ( "Shade_Down" );
	AxisAlignment = in->getAttributeAsVector3d("AxisAlignment" );
	Type = static_cast<BillboardType>(in->getAttributeAsInt("BillboardType"));
	vertices[3].TCoords = in->getAttributeAsVector2d("uvLeftTop");
	vertices[0].TCoords = in->getAttributeAsVector2d("uvRightTop");
	vertices[2].TCoords = in->getAttributeAsVector2d("uvLeftBottom");
	vertices[1].TCoords = in->getAttributeAsVector2d("uvRightBottom");

	setSize(Size);
}


//! Set the color of all vertices of the billboard
//! \param overallColor: the color to set
void CBillboardSceneNode::setColor(const video::SColor & overallColor)
{
	for(u32 vertex = 0; vertex < 4; ++vertex)
		vertices[0].Color = overallColor;
}


//! Set the color at the start (bottom) and end (top) edges vertices of the billboard.
//! \param[in] startColor: the color to set the vertices of the start (bottom) edge.
//! \param[in] endColor: the color to set the vertices of the end (top) edge.
void CBillboardSceneNode::setColor(const video::SColor & startColor,
                                   const video::SColor & endColor)
{
	vertices[0].Color = endColor;
	vertices[1].Color = startColor;
	vertices[2].Color = startColor;
	vertices[3].Color = endColor;
}


//! Gets the color of the top and bottom vertices of the billboard
//! \param[out] startColor: stores the color of the start (bottom) edge vertices
//! \param[out] endColor: stores the color of the end (top) edge vertices
void CBillboardSceneNode::getColor(video::SColor & startColor, video::SColor & endColor) const
{
	endColor = vertices[0].Color;
	startColor = vertices[1].Color;
}

//! Set the alignment type of the billboard.
//! \param[in] alignment	The alignment type
void CBillboardSceneNode::setBillboardType(BillboardType billboardType)
{
	Type = billboardType;
}

//! Get the current alignment type of the billboard.
//! \return	The current alignment type
CBillboardSceneNode::BillboardType CBillboardSceneNode::getBillboardType(void) const
{
	return Type;
}

//! Sets the texture co-ordinates for each corner of the billboard.
//! These default to 0,1 / 1,1 / 0,0 / 0,1.  You can set different values
//! in order to only use part of a texture.
//! \param[in] uvLeftTop     The coordinates for the top left corner.
//! \param[in] uvRightTop    The coordinates for the top right corner.
//! \param[in] uvLeftBottom  The coordinates for the bottom left corner.
//! \param[in] uvRightBottom The coordinates for the bottom right corner.
void CBillboardSceneNode::setTextureCoordinates(const core::vector2df & uvLeftTop,
												const core::vector2df & uvRightTop,
												const core::vector2df & uvLeftBottom,
												const core::vector2df & uvRightBottom)
{
	vertices[0].TCoords = uvRightTop;
	vertices[1].TCoords = uvRightBottom;
	vertices[2].TCoords = uvLeftBottom;
	vertices[3].TCoords = uvLeftTop;
}

//! Gets the current texture co-ordinates for each corner of the billboard.
//! \param[out] uvLeftTop     The coordinates for the top left corner.
//! \param[out] uvRightTop    The coordinates for the top right corner.
//! \param[out] uvLeftBottom  The coordinates for the bottom left corner.
//! \param[out] uvRightBottom The coordinates for the bottom right corner.
void CBillboardSceneNode::getTextureCoordinates(core::vector2df & uvLeftTop,
												core::vector2df & uvRightTop,
												core::vector2df & uvLeftBottom,
												core::vector2df & uvRightBottom) const
{
	uvRightTop = vertices[0].TCoords;
	uvRightBottom = vertices[1].TCoords;
	uvLeftBottom = vertices[2].TCoords;
	uvLeftTop = vertices[3].TCoords;
}

//! Set the alignment of the billboard's axis.  This is only applicable
//! to AxisAligned billboards and will have no effect on point billboards.
//! \param[in] alignment	The alignment of the billboard
void CBillboardSceneNode::setAxisAlignment(const core::vector3df & alignment)
{
	AxisAlignment = alignment;
	AxisAlignment.normalize();
	calculateBoundingBox();
}

//! Get the alignment of the billboard's axis.
//! \param[out] alignment	The alignment of the billboard
const core::vector3df & CBillboardSceneNode::getAxisAlignment(void) const
{
	return AxisAlignment;
}



//! Set the start and end points of the billboard.  This is only applicable
//! to AxisAligned billboards and will have no effect on point billboards.
//! It will rotate the billboard and stretch its height so that it starts and
//! ends at the given points, which need *not* be in a plane orthogonal to the
//! camera's POV. It will not change the billboard's width (you can set that
//! first using setSize()).
//!
//! \param[in] centreOfBottomEdge	The start point for the billboard
//! \param[in] centreOfTopEdge		The end point for the billboard
void CBillboardSceneNode::setStartAndEndPoints(const core::vector3df & centreOfBottomEdge,
											  const core::vector3df & centreOfTopEdge)
{
	core::vector3df startToEnd(centreOfTopEdge - centreOfBottomEdge);
	Size.Height = startToEnd.getLength();
	setAxisAlignment(startToEnd);
	setPosition((centreOfBottomEdge + centreOfTopEdge) / 2.f);
}

//! Creates a clone of this scene node and its children.
ISceneNode* CBillboardSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
{
	if (!newParent)
		newParent = Parent;
	if (!newManager)
		newManager = SceneManager;

	CBillboardSceneNode* nb = new CBillboardSceneNode(newParent,
		newManager, ID, RelativeTranslation, Size);

	nb->AxisAlignment = this->AxisAlignment;
	nb->Type = this->Type;
	nb->TopEdgeWidth = this->TopEdgeWidth;

	nb->cloneMembers(this, newManager);
	nb->Material = Material;

	nb->drop();
	return nb;
}

void CBillboardSceneNode::calculateBoundingBox(void)
{
	switch(Type)
	{
		case PointAligned:
		{
			f32 avg = (Size.Width + Size.Height)/6;
			BBox.MinEdge.set(-avg,-avg,-avg);
			BBox.MaxEdge.set(avg,avg,avg);
		}
		break;

		default:
		{
			// Use the largest possible size when viewed orthogonally
			core::vector3df halfAxis(AxisAlignment * Size.Height * 0.5f);
			halfAxis.X += core::max_(Size.Width, TopEdgeWidth);
			halfAxis.Y += core::max_(Size.Width, TopEdgeWidth);
			halfAxis.Z += core::max_(Size.Width, TopEdgeWidth);

			BBox.MinEdge.set(-halfAxis.X, -halfAxis.Y, -halfAxis.Z);
			BBox.MaxEdge.set(halfAxis.X, halfAxis.Y, halfAxis.Z);
		}
		break;
	}
}


} // end namespace scene
} // end namespace irr

