// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_BILLBOARD_SCENE_NODE_H_INCLUDED__
#define __C_BILLBOARD_SCENE_NODE_H_INCLUDED__

#include "IBillboardSceneNode.h"
#include "S3DVertex.h"

namespace irr
{
namespace scene
{

//! Scene node which is a billboard. A billboard is like a 3d sprite: A 2d element,
//! which always looks to the camera.
class CBillboardSceneNode : public IBillboardSceneNode
{
public:

	//! constructor
	CBillboardSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
		const core::vector3df& position, const core::dimension2d<f32>& size,
		video::SColor shade_top = video::SColor(0xFFFFFFFF),
		video::SColor shade_down = video::SColor(0xFFFFFFFF));

	//! pre render event
	virtual void OnRegisterSceneNode();

	//! render
	virtual void render();

	//! returns the axis aligned bounding box of this node
	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	//! sets the size of the billboard
	virtual void setSize(const core::dimension2d<f32>& size);

	//! Sets the widths of the top and bottom edges of the billboard
	//! independently.
	//! \param[in] bottomEdgeWidth The width of the bottom edge of the billboard.
	//! \param[in] topEdgeWidth    The width of the top edge of the billboard.
	virtual void setWidths(f32 bottomEdgeWidth,
							f32 topEdgeWidth);

	//! gets the size of the billboard
	virtual const core::dimension2d<f32>& getSize() const;

	//! Gets the widths of the top and bottom edges of the billboard.
	//! \param[out] bottomEdgeWidth The width of the bottom edge of the billboard.
	//! \param[out] topEdgeWidth    The width of the top edge of the billboard.
	virtual void getWidths(f32 & bottomEdgeWidth,
							f32 & topEdgeWidth) const;

	virtual video::SMaterial& getMaterial(u32 i);

	//! returns amount of materials used by this scene node.
	virtual u32 getMaterialCount() const;

	//! Set the color of all vertices of the billboard
	//! \param overallColor: the color to set
	virtual void setColor(const video::SColor & overallColor);

	//! Set the color at the start (bottom) and end (top) edges vertices of the billboard.
	//! \param[in] startColor: the color to set the vertices of the start (bottom) edge.
	//! \param[in] endColor: the color to set the vertices of the end (top) edge.
	virtual void setColor(const video::SColor & startColor,
							const video::SColor & endColor);

	//! Gets the color of the top and bottom vertices of the billboard
	//! \param[out] startColor: stores the color of the start (bottom) edge vertices
	//! \param[out] endColor: stores the color of the end (top) edge vertices
	virtual void getColor(video::SColor & startColor, video::SColor & endColor) const;

	//! Set the type of the billboard.
	//! \param[in] alignment	The alignment type
	virtual void setBillboardType(BillboardType billboardType);

	//! Get the current type of the billboard.
	//! \return	The current alignment type
	virtual BillboardType getBillboardType(void) const;

	//! Sets the texture co-ordinates for each corner of the billboard.
	//! These default to 0,1 / 1,1 / 0,0 / 0,1.  You can set different values
	//! in order to only use part of a texture.
	//! \param[in] uvLeftTop     The coordinates for the top left corner.
	//! \param[in] uvRightTop    The coordinates for the top right corner.
	//! \param[in] uvLeftBottom  The coordinates for the bottom left corner.
	//! \param[in] uvRightBottom The coordinates for the bottom right corner.
	virtual void setTextureCoordinates(const core::vector2df & uvLeftTop,
										const core::vector2df & uvRightTop,
										const core::vector2df & uvLeftBottom,
										const core::vector2df & uvRightBottom);

	//! Gets the current texture co-ordinates for each corner of the billboard.
	//! \param[out] uvLeftTop     The coordinates for the top left corner.
	//! \param[out] uvRightTop    The coordinates for the top right corner.
	//! \param[out] uvLeftBottom  The coordinates for the bottom left corner.
	//! \param[out] uvRightBottom The coordinates for the bottom right corner.
	virtual void getTextureCoordinates(core::vector2df & uvLeftTop,
										core::vector2df & uvRightTop,
										core::vector2df & uvLeftBottom,
										core::vector2df & uvRightBottom) const;

	//! Set the alignment of the billboard's axis.  This is only applicable
	//! to AxisAligned billboards and will have no effect on point billboards.
	//! \param[in] alignment	The alignment of the billboard
	virtual void setAxisAlignment(const core::vector3df & alignment);

	//! Get the alignment of the billboard's axis.
	//! \param[out] alignment	The alignment of the billboard
	virtual const core::vector3df & getAxisAlignment(void) const;

	//! Set the start and end points of the billboard.  This is only applicable
	//! to AxisAligned billboards and will have no effect on point billboards.
	//! It will rotate the billboard and stretch its height so that it starts and
	//! ends at the given points, which need *not* be in a plane orthogonal to the
	//! camera's POV. It will not change the billboard's width (you can set that
	//! first using setSize()).
	//!
	//! \param[in] startPoint	The start point for the billboard
	//! \param[in] endPoint		The end point for the billboard
	virtual void setStartAndEndPoints(const core::vector3df & startPoint,
									  const core::vector3df & endPoint);

	//! Writes attributes of the scene node.
	virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;

	//! Reads attributes of the scene node.
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

	//! Returns type of the scene node
	virtual ESCENE_NODE_TYPE getType() const { return ESNT_BILLBOARD; }

	//! Creates a clone of this scene node and its children.
	virtual ISceneNode* clone(ISceneNode* newParent=0, ISceneManager* newManager=0);

protected:
	//! A utility function to calculate the bounding box
	virtual void calculateBoundingBox(void);

private:

	//! Size.Width is the bottom edge width
    core::dimension2d<f32> Size;
	f32 TopEdgeWidth;
 	core::aabbox3d<f32> BBox;
 	video::SMaterial Material;

 	video::S3DVertex vertices[4];
 	u16 indices[6];

	core::vector3df AxisAlignment;
	BillboardType Type;
};

} // end namespace scene
} // end namespace irr

#endif

