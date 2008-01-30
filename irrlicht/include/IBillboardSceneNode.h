// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_BILLBOARD_SCENE_NODE_H_INCLUDED__
#define __I_BILLBOARD_SCENE_NODE_H_INCLUDED__

#include "ISceneNode.h"

namespace irr
{
namespace scene
{

//! A billboard scene node.
/** A billboard is like a 3d sprite: A 2d element,
which always looks to the camera. It is usually used for explosions, fire,
lensflares, particles and things like that.

Irrlicht billboards can rotate around a point, or can be fixed to rotate around
an axis.
*/
class IBillboardSceneNode : public ISceneNode
{
public:

	//! The alignment type of the billboard.  All newly added billboards have an
	//! alignment of PointAligned.  Call setAlignmentType() if you want to make
	//! a billboard AxisAligned.
	typedef enum
	{
		//! The billboard will rotate around its centre point.  It will always
		//! be flat and will face the camera fully.
		PointAligned,

		//! The billboard will rotate around an axis, i.e. a vertical line running
		//! up its centre.  It will rotate around that axis to face the camera as
		//! fully as possible
		AxisAligned

	} BillboardType;

	//! constructor
	IBillboardSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
		const core::vector3df& position = core::vector3df(0,0,0))
		: ISceneNode(parent, mgr, id, position) {}

	//! Sets the size of the billboard, making it rectangular.
	virtual void setSize(const core::dimension2d<f32>& size) = 0;

	//! Sets the widths of the start (bottom) and end (top) edges of the billboard
	//! independently.
	//! \param[in] startEdgeWidth The width of the start (bottom) edge of the billboard.
	//! \param[in] endEdgeWidth   The width of the end (top) edge of the billboard.
	virtual void setWidths(f32 startEdgeWidth, f32 endEdgeWidth) = 0;

	//! Returns the size of the billboard.  This will return the width of the
	//! start (bottom) edge of the billboard.  Use getWidths() to retrieve
	//! the start (bottom) end (top) edges independently.
	virtual const core::dimension2d<f32>& getSize() const = 0;

	//! Gets the widths of the top and bottom edges of the billboard.
	//! \param[out] startEdgeWidth The width of the start (bottom) edge of the billboard.
	//! \param[out] endEdgeWidth   The width of the end (top) edge of the billboard.

	virtual void getWidths(f32 & startEdgeWidth, f32 & endEdgeWidth) const = 0;

	//! Set the color of all vertices of the billboard
	//! \param[in] overallColor: the color to set
	virtual void setColor(const video::SColor & overallColor) = 0;

	//! Set the color at the start (bottom) and end (top) edges vertices of the billboard.
	//! \param[in] startColor: the color to set the vertices of the start (bottom) edge.
	//! \param[in] endColor: the color to set the vertices of the end (top) edge.
	virtual void setColor(const video::SColor & startColor,	const video::SColor & endColor) = 0;

	//! Gets the color of the top and bottom vertices of the billboard
	//! \param[out] startColor: stores the color of the start (bottom) edge vertices
    //! \param[out] endColor: stores the color of the end (top) edge vertices
    virtual void getColor(video::SColor & startColor, video::SColor & endColor) const = 0;

	//! Set the type of the billboard.
	//! \param[in] alignment	The alignment type
	virtual void setBillboardType(BillboardType billboardType) = 0;

	//! Get the current type of the billboard.
	//! \return	The current alignment type
	virtual BillboardType getBillboardType(void) const = 0;

	//! Sets the texture co-ordinates for each corner of the billboard.
	//! \param[in] uvLeftTop     The coordinates for the top left corner.
	//! \param[in] uvRightTop    The coordinates for the top right corner.
	//! \param[in] uvLeftBottom  The coordinates for the bottom left corner.
	//! \param[in] uvRightBottom The coordinates for the bottom right corner.
	virtual void setTextureCoordinates(const core::vector2df & uvLeftTop,
										const core::vector2df & uvRightTop,
										const core::vector2df & uvLeftBottom,
										const core::vector2df & uvRightBottom) = 0;

	//! Gets the current texture co-ordinates for each corner of the billboard.
	//! \param[out] uvLeftTop     The coordinates for the top left corner.
	//! \param[out] uvRightTop    The coordinates for the top right corner.
	//! \param[out] uvLeftBottom  The coordinates for the bottom left corner.
	//! \param[out] uvRightBottom The coordinates for the bottom right corner.
	virtual void getTextureCoordinates(core::vector2df & uvLeftTop,
										core::vector2df & uvRightTop,
										core::vector2df & uvLeftBottom,
										core::vector2df & uvRightBottom) const = 0;

	//! Set the alignment of the billboard's axis.  This is only applicable
	//! to AxisAligned billboards and will have no effect on point billboards.
	//! \param[in] alignment	The alignment of the billboard
	virtual void setAxisAlignment(const core::vector3df & alignment) = 0;

	//! Get the alignment of the billboard's axis.
	//! \param[out] alignment	The alignment of the billboard
	virtual const core::vector3df & getAxisAlignment(void) const = 0;

	//! Set the start and end points of the billboard.  This is only applicable
	//! to AxisAligned billboards and will have no effect on point billboards.
	//! It will set the axis alignment of the billboard and scale its height so that
	//! it will appear to start and ends at the given points, which need *not* be in
	//! a plane orthogonal to the camera's POV. It will not change the billboard's
	//! width (you can set that first using setSize()).
	//!
	//! \param[in] startPoint	The start point for the billboard
	//! \param[in] endPoint		The end point for the billboard
	virtual void setStartAndEndPoints(const core::vector3df & startPoint,
									  const core::vector3df & endPoint) = 0;
};

} // end namespace scene
} // end namespace irr


#endif

