// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_GUI_COLOR_SELECT_DIALOG_H_INCLUDED__
#define __C_GUI_COLOR_SELECT_DIALOG_H_INCLUDED__

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_GUI_

#include "IGUIColorSelectDialog.h"
#include "IGUIButton.h"
#include "IGUIEditBox.h"
#include "IGUIScrollBar.h"
#include "IGUIImage.h"
#include "irrArray.h"


namespace irr
{
namespace gui
{

	class CGUIColorSelectDialog : public IGUIColorSelectDialog
	{
	public:

		//! constructor
		CGUIColorSelectDialog(const wchar_t* title, IGUIEnvironment* environment, IGUIElement* parent, s32 id);

		//! destructor
		virtual ~CGUIColorSelectDialog();

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event);

		//! draws the element and its children
		virtual void draw();

	private:

		//! sends the event that the file has been selected.
		void sendSelectedEvent();

		//! sends the event that the file choose process has been canceld
		void sendCancelEvent();

		core::position2d<s32> DragStart;
		bool Dragging;
		IGUIButton* CloseButton;
		IGUIButton* OKButton;
		IGUIButton* CancelButton;

		struct SBatteryItem
		{
			f32 Incoming;
			f32 Outgoing;
			IGUIEditBox * Edit;
			IGUIScrollBar *Scrollbar;
		};
		core::array< SBatteryItem > Battery;

		struct SColorCircle
		{
			IGUIImage * Control;
			video::ITexture * Texture;
		};
		SColorCircle ColorRing;

		void buildColorRing( const core::dimension2d<u32> & dim, s32 supersample, const u32 borderColor );
	};


} // end namespace gui
} // end namespace irr

#endif // _IRR_COMPILE_WITH_GUI_

#endif // __C_GUI_COLOR_SELECT_DIALOG_H_INCLUDED__

