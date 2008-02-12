// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_TEXT_SCENE_NODE_H_INCLUDED__
#define __I_TEXT_SCENE_NODE_H_INCLUDED__

#include "ISceneNode.h"

namespace irr
{
namespace scene
{

//! A scene node for displaying 2d text at a position in three dimensional space
class ITextSceneNode : public ISceneNode
{
public:

	//! constructor
	ITextSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
		const core::vector3df& position = core::vector3df(0,0,0))
			: ISceneNode(parent, mgr, id, position) {}

	//! sets the text string
	virtual void setText(const wchar_t* text) = 0;

	//! sets the color of the text
	virtual void setTextColor(video::SColor color) = 0;

// >> add by arch_jslin 2008.02.02
    //! gets the color of the text
    virtual video::SColor& getTextColor() = 0;

// >> add by arch_jslin 2008.02.03
    //! sets the center alignment of the text
    virtual void setCenter(const bool& hcenter, const bool& vcenter) = 0;
};

} // end namespace scene
} // end namespace irr


#endif

