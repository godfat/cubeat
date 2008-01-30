// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CGUIToolBar.h"
#ifdef _IRR_COMPILE_WITH_GUI_

#include "IGUISkin.h"
#include "IGUIEnvironment.h"
#include "IVideoDriver.h"
#include "IGUIButton.h"
#include "IGUIFont.h"
#include "CGUIButton.h"

namespace irr
{
namespace gui
{

//! constructor
CGUIToolBar::CGUIToolBar(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rect<s32> rectangle)
:IGUIToolBar(environment, parent, id, rectangle), ButtonX(5)
{
	#ifdef _DEBUG
	setDebugName("CGUIToolBar");
	#endif

	// calculate position and find other menubars
	s32 y = 0;
	s32 parentwidth = 100;

	if (parent)
	{
		parentwidth = Parent->getAbsolutePosition().getWidth();

		const core::list<IGUIElement*>& children = parent->getChildren();
		core::list<IGUIElement*>::ConstIterator it = children.begin();
		for (; it != children.end(); ++it)
		{
			core::rect<s32> r = (*it)->getAbsolutePosition();
			if (r.UpperLeftCorner.X == 0 && r.UpperLeftCorner.Y <= y &&
				r.LowerRightCorner.X == parentwidth)
				y = r.LowerRightCorner.Y;
		}
	}

	core::rect<s32> rr;
	rr.UpperLeftCorner.X = 0;
	rr.UpperLeftCorner.Y = y;
	s32 height = Environment->getSkin()->getSize ( EGDS_MENU_HEIGHT );

	/*IGUISkin* skin = Environment->getSkin();
	IGUIFont* font = skin->getFont();
	if (font)
	{
		s32 t = font->getDimension(L"A").Height + 5;
		if (t > height)
			height = t;
	}*/

	rr.LowerRightCorner.X = parentwidth;
	rr.LowerRightCorner.Y = rr.UpperLeftCorner.Y + height;
	setRelativePosition(rr);
}


//! called if an event happened.
bool CGUIToolBar::OnEvent(const SEvent& event)
{
	if (event.EventType == EET_MOUSE_INPUT_EVENT && 
		event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
	{
		if (AbsoluteClippingRect.isPointInside(core::position2di(event.MouseInput.X, event.MouseInput.Y)))
			return true;
	}

	return Parent ? Parent->OnEvent(event) : false;
}


//! draws the element and its children
void CGUIToolBar::draw()
{
	if (!IsVisible)
		return;

	IGUISkin* skin = Environment->getSkin();
	if (!skin)
		return;

	core::rect<s32> rect = AbsoluteRect;
	core::rect<s32>* clip = 0;

	// draw frame
	skin->draw3DToolBar(this, rect, clip);

	IGUIElement::draw();
}


//! Updates the absolute position.
void CGUIToolBar::updateAbsolutePosition()
{
	if (Parent)
	{
		DesiredRect.UpperLeftCorner.X = 0;
		DesiredRect.LowerRightCorner.X = Parent->getAbsolutePosition().getWidth();
	}

	IGUIElement::updateAbsolutePosition();
}


//! Adds a button to the tool bar
IGUIButton* CGUIToolBar::addButton(s32 id, const wchar_t* text,const wchar_t* tooltiptext,
	video::ITexture* img, video::ITexture* pressed, bool isPushButton, 
	bool useAlphaChannel)
{
	ButtonX += 3;

	core::rect<s32> rectangle(ButtonX,2,0,0);
	if ( img )
	{
		const core::dimension2di &size = img->getOriginalSize();
		rectangle.LowerRightCorner.X = rectangle.UpperLeftCorner.X + size.Width + 8;
		rectangle.LowerRightCorner.Y = rectangle.UpperLeftCorner.Y + size.Height + 6;
	}

	ButtonX += rectangle.getWidth();

	IGUIButton* button = new CGUIButton(Environment, this, id, rectangle);
	button->drop();

	if (text)
		button->setText(text);

	if (tooltiptext)
		button->setToolTipText(tooltiptext);

	if (img)
		button->setImage(img);

	if (pressed)
		button->setPressedImage(pressed);

	if (isPushButton)
		button->setIsPushButton(isPushButton);

	if (useAlphaChannel)
		button->setUseAlphaChannel(useAlphaChannel);
	
	return button;
}
	
} // end namespace gui
} // end namespace irr

#endif // _IRR_COMPILE_WITH_GUI_

