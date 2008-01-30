#ifndef __C_BONE_SCENE_NODE_H_INCLUDED__
#define __C_BONE_SCENE_NODE_H_INCLUDED__

// Used with SkinnedMesh and IAnimatedMeshSceneNode, for boned meshes

#include "IBoneSceneNode.h"
#include "irrString.h"


namespace irr
{
namespace scene
{

	class CBoneSceneNode : public IBoneSceneNode
	{
	public:

		//! constructor
		CBoneSceneNode(ISceneNode* parent, ISceneManager* mgr,
			s32 id=-1, u32 boneIndex=0, const c8* boneName=0);

		//! destructor
		~CBoneSceneNode();

		//! Returns the name of the bone
		virtual const c8* getBoneName() const;

		//! Returns the index of the bone
		virtual u32 getBoneIndex() const;

		//! Sets the animation mode of the bone. Returns true if successful.
		virtual bool setAnimationMode(E_BONE_ANIMATION_MODE mode);

		//! Gets the current animation mode of the bone
		virtual E_BONE_ANIMATION_MODE getAnimationMode() const;

		//! returns the axis aligned bounding box of this node
		virtual const core::aabbox3d<f32>& getBoundingBox() const;

		//! Returns the relative transformation of the scene node.
		//virtual core::matrix4 getRelativeTransformation() const;

		virtual void OnAnimate(u32 timeMs);


		void helper_updateAbsolutePositionOfAllChildren(ISceneNode *Node);

		virtual void updateAbsolutePositionOfAllChildren();

		//! Writes attributes of the scene node.
		virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;

		//! Reads attributes of the scene node.
		virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

		//! How the relative transformation of the bone is used
		virtual void setSkinningSpace( E_BONE_SKINNING_SPACE space )
		{
			SkinningSpace=space;
		}

		virtual E_BONE_SKINNING_SPACE getSkinningSpace()
		{
			return SkinningSpace;
		}

	private:
		E_BONE_ANIMATION_MODE AnimationMode;
		E_BONE_SKINNING_SPACE SkinningSpace;

		u32 BoneIndex;
		core::stringc BoneName;





		core::aabbox3d<f32> Box;
	};

} // end namespace scene
} // end namespace irr



#endif

