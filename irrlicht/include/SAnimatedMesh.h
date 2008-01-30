// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __S_ANIMATED_MESH_H_INCLUDED__
#define __S_ANIMATED_MESH_H_INCLUDED__

#include "IAnimatedMesh.h"
#include "IMesh.h"
#include "aabbox3d.h"
#include "irrArray.h"

namespace irr
{
namespace scene
{

	//! Simple implementation of the IAnimatedMesh interface.
	struct SAnimatedMesh : public IAnimatedMesh
	{
		//! constructor
		SAnimatedMesh() : IAnimatedMesh(), Type(EAMT_UNKNOWN)
		{
			#ifdef _DEBUG
			setDebugName("SAnimatedMesh");
			#endif
		}


		//! constructor
		SAnimatedMesh(scene::IMesh* mesh, scene::E_ANIMATED_MESH_TYPE type) : IAnimatedMesh(), Type(type)
		{
			#ifdef _DEBUG
			setDebugName("SAnimatedMesh");
			#endif
			addMesh(mesh);
			recalculateBoundingBox();
		}


		//! destructor
		virtual ~SAnimatedMesh()
		{
			// drop meshes
			for (u32 i=0; i<Meshes.size(); ++i)
				Meshes[i]->drop();
		};


		//! Gets the frame count of the animated mesh.
		//! \return Returns the amount of frames. If the amount is 1, it is a static, non animated mesh.
		virtual u32 getFrameCount() const
		{
			return Meshes.size();
		}



		//! Returns the IMesh interface for a frame.
		//! \param frame: Frame number as zero based index. The maximum frame number is
		//! getFrameCount() - 1;
		//! \param detailLevel: Level of detail. 0 is the lowest,
		//! 255 the highest level of detail. Most meshes will ignore the detail level.
		//! \param startFrameLoop: start frame
		//! \param endFrameLoop: end frame
		//! \return Returns the animated mesh based on a detail level.
		virtual IMesh* getMesh(s32 frame, s32 detailLevel, s32 startFrameLoop=-1, s32 endFrameLoop=-1)
		{
			if (Meshes.empty())
				return 0;

			return Meshes[frame];
		}


		//! adds a Mesh
		void addMesh(IMesh* mesh)
		{
			if (mesh)
			{
				mesh->grab();
				Meshes.push_back(mesh);
			}
		}


		//! Returns an axis aligned bounding box of the mesh.
		//! \return A bounding box of this mesh is returned.
		virtual const core::aabbox3d<f32>& getBoundingBox() const
		{
			return Box;
		}

		//! set user axis aligned bounding box
		virtual void setBoundingBox( const core::aabbox3df& box)
		{
			Box = box;
		}

		void recalculateBoundingBox()
		{
			Box.reset(0,0,0);

			if (Meshes.empty())
				return;

			Box = Meshes[0]->getBoundingBox();

			for (u32 i=1; i<Meshes.size(); ++i)
				Box.addInternalBox(Meshes[i]->getBoundingBox());
		}

		//! Returns the type of the animated mesh.
		virtual E_ANIMATED_MESH_TYPE getMeshType() const
		{
			return Type;
		}

		//! returns amount of mesh buffers.
		virtual u32 getMeshBufferCount() const
		{
			return 0;
		}

		//! returns pointer to a mesh buffer
		virtual IMeshBuffer* getMeshBuffer(u32 nr) const
		{
			return 0;
		}

		//! Returns pointer to a mesh buffer which fits a material
 		/** \param material: material to search for
		\return Returns the pointer to the mesh buffer or
		NULL if there is no such mesh buffer. */
		virtual IMeshBuffer* getMeshBuffer( const video::SMaterial &material) const
		{
			return 0;
		}

		virtual void setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue)
		{
		}


		core::aabbox3d<f32> Box;
		core::array<IMesh*> Meshes;
		E_ANIMATED_MESH_TYPE Type;
	};


} // end namespace scene
} // end namespace irr

#endif

