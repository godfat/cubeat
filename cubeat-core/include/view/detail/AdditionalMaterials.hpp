#ifndef _SHOOTING_CUBES_VIEW_DETAIL_ADDITIONAL_MATERIALS_
#define _SHOOTING_CUBES_VIEW_DETAIL_ADDITIONAL_MATERIALS_

#include "IrrDevice.hpp"

namespace psc {
namespace view {
namespace detail {

//class Material_ADD : public irr::video::IMaterialRenderer
//{
//public:
//
//	Material_ADD() {}
//
//	virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
//		bool resetAllRenderstates, IMaterialRendererServices* services)
//	{
//	    irr::video::IVideoDriver* Driver = IrrDevice::i()->d()->getVideoDriver();
//		Driver->setActiveTexture
//		->disableTextures(1);
//		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
//
//		if ((material.MaterialType != lastMaterial.MaterialType) || resetAllRenderstates)
//		{
//			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
//			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//			glEnable(GL_BLEND);
//		}
//	}
//
//	virtual void OnUnsetMaterial()
//	{
//		glDisable(GL_BLEND);
//	}
//
//	//! Returns if the material is transparent.
//	virtual bool isTransparent() const
//	{
//		return true;
//	}
//};

}
}
}

#endif //_SHOOTING_CUBES_VIEW_DETAIL_ADDITIONAL_MATERIALS_
