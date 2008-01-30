
#include "CBoneSceneNode.h"



namespace irr
{
namespace scene
{

//! constructor
CBoneSceneNode::CBoneSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
	u32 boneIndex, const c8* boneName)
: IBoneSceneNode(parent, mgr, id), AnimationMode(EBAM_AUTOMATIC), SkinningSpace(EBSS_LOCAL),
  BoneIndex(boneIndex), BoneName(boneName)
{

}

//! destructor
CBoneSceneNode::~CBoneSceneNode()
{

}

//! Returns the name of the bone
const c8* CBoneSceneNode::getBoneName() const
{
	return BoneName.c_str();
}

//! Returns the index of the bone
u32 CBoneSceneNode::getBoneIndex() const
{
	return BoneIndex;
}

//! Sets the animation mode of the bone. Returns true if successful.
bool CBoneSceneNode::setAnimationMode(E_BONE_ANIMATION_MODE mode)
{
	AnimationMode = mode;
	return true;
}

//! Gets the current animation mode of the bone
E_BONE_ANIMATION_MODE CBoneSceneNode::getAnimationMode() const
{
	return AnimationMode;
}

//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CBoneSceneNode::getBoundingBox() const
{
	return Box;
}
/*
//! Returns the relative transformation of the scene node.
core::matrix4 CBoneSceneNode::getRelativeTransformation() const
{
	return core::matrix4(); // RelativeTransformation;
}
*/

void CBoneSceneNode::OnAnimate(u32 timeMs)
{
	if (IsVisible)
	{
		// animate this node with all animators

		core::list<ISceneNodeAnimator*>::Iterator ait = Animators.begin();
		for (; ait != Animators.end(); ++ait)
			(*ait)->animateNode(this, timeMs);

		// update absolute position
		//updateAbsolutePosition();

		// perform the post render process on all children
		core::list<ISceneNode*>::Iterator it = Children.begin();
		for (; it != Children.end(); ++it)
			(*it)->OnAnimate(timeMs);
	}
}




void CBoneSceneNode::helper_updateAbsolutePositionOfAllChildren(ISceneNode *Node)
{
	Node->updateAbsolutePosition();

	core::list<ISceneNode*>::ConstIterator it = Node->getChildren().begin();
	for (; it != Node->getChildren().end(); ++it)
	{
		helper_updateAbsolutePositionOfAllChildren( (*it) );
	}
}


void CBoneSceneNode::updateAbsolutePositionOfAllChildren()
{
	helper_updateAbsolutePositionOfAllChildren( this );
}





void CBoneSceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	out->addInt("BoneIndex", BoneIndex);
	out->addString("BoneName", BoneName.c_str());
	out->addEnum("AnimationMode", AnimationMode, BoneAnimationModeNames);
}

void CBoneSceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	BoneIndex = in->getAttributeAsInt("BoneIndex");
	BoneName = in->getAttributeAsString("BoneName");
	AnimationMode = (E_BONE_ANIMATION_MODE)in->getAttributeAsEnumeration("AnimationMode", BoneAnimationModeNames);
	// todo: add/replace bone in parent with bone from mesh
}

} // namespace scene
} // namespace irr
