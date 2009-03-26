// Copyright (C) 2002-2009 Nikolaus Gebhardt / Thomas Alten
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_BURNING_SHADER_H_INCLUDED__
#define __I_BURNING_SHADER_H_INCLUDED__

#include "SoftwareDriver2_compile_config.h"
#include "IReferenceCounted.h"
#include "irrMath.h"
#include "IImage.h"
#include "S2DVertex.h"
#include "rect.h"
#include "IDepthBuffer.h"
#include "S4DVertex.h"
#include "irrArray.h"
#include "SLight.h"
#include "SMaterial.h"
#include "os.h"


namespace irr
{

namespace video
{

	struct SBurningShaderLight
	{
		//SLight org;
		bool LightIsOn;

		E_LIGHT_TYPE Type;
		f32 radius;
		f32 linearAttenuation;
		f32 constantAttenuation;
		f32 quadraticAttenuation;
		sVec4 pos;

		sVec3 AmbientColor;
		sVec3 DiffuseColor;
		sVec3 SpecularColor;
	};

	enum eLightFlags
	{
		ENABLED		= 0x01,
		POINTLIGHT	= 0x02,
		SPECULAR	= 0x04,
		FOG			= 0x08,
		NORMALIZE	= 0x10,
		VERTEXTRANSFORM	= 0x20,
	};

	struct SBurningShaderLightSpace
	{
		void reset ()
		{
			Light.set_used ( 0 );
			Global_AmbientLight.set ( 0.f, 0.f, 0.f );
			Flags = 0;
		}
		core::array<SBurningShaderLight> Light;
		sVec3 Global_AmbientLight;
		sVec4 FogColor;
		sVec4 campos;
		sVec4 vertex;
		sVec4 normal;
		u32 Flags;
	};

	struct SBurningShaderMaterial
	{
		SMaterial org;

		sVec3 AmbientColor;
		sVec3 DiffuseColor;
		sVec3 SpecularColor;
		sVec3 EmissiveColor;

	};

	enum EBurningFFShader
	{
		ETR_FLAT = 0,
		ETR_FLAT_WIRE,
		ETR_GOURAUD,
		ETR_GOURAUD_WIRE,
		ETR_TEXTURE_FLAT,
		ETR_TEXTURE_FLAT_WIRE,
		ETR_TEXTURE_GOURAUD,
		ETR_TEXTURE_GOURAUD_WIRE,
		ETR_TEXTURE_GOURAUD_NOZ,
		ETR_TEXTURE_GOURAUD_ADD,
		ETR_TEXTURE_GOURAUD_ADD_NO_Z,

		ETR_TEXTURE_GOURAUD_VERTEX_ALPHA,

		ETR_TEXTURE_GOURAUD_LIGHTMAP_M1,
		ETR_TEXTURE_GOURAUD_LIGHTMAP_M2,
		ETR_TEXTURE_GOURAUD_LIGHTMAP_M4,
		ETR_TEXTURE_LIGHTMAP_M4,

		ETR_TEXTURE_GOURAUD_DETAIL_MAP,
		ETR_TEXTURE_GOURAUD_LIGHTMAP_ADD,
	
		ETR_GOURAUD_ALPHA,
		ETR_GOURAUD_ALPHA_NOZ,

		ETR_TEXTURE_GOURAUD_ALPHA,
		ETR_TEXTURE_GOURAUD_ALPHA_NOZ,

		ETR_TEXTURE_BLEND,
		ETR_REFERENCE,
		ETR_INVALID,

		ETR2_COUNT
	};


	class IBurningShader : public virtual IReferenceCounted
	{
	public:
		IBurningShader(IDepthBuffer* zbuffer);

		//! destructor
		virtual ~IBurningShader();

		//! sets a render target
		virtual void setRenderTarget(video::IImage* surface, const core::rect<s32>& viewPort);

		//! sets the Texture
		virtual void setTextureParam( u32 stage, video::CSoftwareTexture2* texture, s32 lodLevel);
		virtual void drawTriangle ( const s4DVertex *a,const s4DVertex *b,const s4DVertex *c ) = 0;
		virtual void drawLine ( const s4DVertex *a,const s4DVertex *b) {};

		virtual void setParam ( u32 index, f32 value) {};
		virtual void setZCompareFunc ( u32 func) {};

		virtual void setMaterial ( const SBurningShaderMaterial &material ) {};

	protected:

		video::CImage* RenderTarget;
		IDepthBuffer* DepthBuffer;

		sInternalTexture IT[ BURNING_MATERIAL_MAX_TEXTURES ];

		static const tFixPointu dithermask[ 4 * 4];
	};


	IBurningShader* createTriangleRendererTextureGouraud2(IDepthBuffer* zbuffer);
	IBurningShader* createTriangleRendererTextureLightMap2_M1(IDepthBuffer* zbuffer);
	IBurningShader* createTriangleRendererTextureLightMap2_M2(IDepthBuffer* zbuffer);
	IBurningShader* createTriangleRendererTextureLightMap2_M4(IDepthBuffer* zbuffer);
	IBurningShader* createTriangleRendererGTextureLightMap2_M4(IDepthBuffer* zbuffer);
	IBurningShader* createTriangleRendererTextureLightMap2_Add(IDepthBuffer* zbuffer);
	IBurningShader* createTriangleRendererTextureDetailMap2(IDepthBuffer* zbuffer);
	IBurningShader* createTriangleRendererTextureVertexAlpha2(IDepthBuffer* zbuffer);


	IBurningShader* createTriangleRendererTextureGouraudWire2(IDepthBuffer* zbuffer);
	IBurningShader* createTriangleRendererGouraud2(IDepthBuffer* zbuffer);
	IBurningShader* createTriangleRendererGouraudAlpha2(IDepthBuffer* zbuffer);
	IBurningShader* createTRGouraudAlphaNoZ2(IDepthBuffer* zbuffer);
	IBurningShader* createTriangleRendererGouraudWire2(IDepthBuffer* zbuffer);
	IBurningShader* createTriangleRendererTextureFlat2(IDepthBuffer* zbuffer);
	IBurningShader* createTriangleRendererTextureFlatWire2(IDepthBuffer* zbuffer);
	IBurningShader* createTRFlat2(IDepthBuffer* zbuffer);
	IBurningShader* createTRFlatWire2(IDepthBuffer* zbuffer);
	IBurningShader* createTRTextureGouraudNoZ2();
	IBurningShader* createTRTextureGouraudAdd2(IDepthBuffer* zbuffer);
	IBurningShader* createTRTextureGouraudAddNoZ2(IDepthBuffer* zbuffer);

	IBurningShader* createTRTextureGouraudAlpha(IDepthBuffer* zbuffer);
	IBurningShader* createTRTextureGouraudAlphaNoZ(IDepthBuffer* zbuffer);
	IBurningShader* createTRTextureBlend(IDepthBuffer* zbuffer);
	IBurningShader* createTRTextureInverseAlphaBlend(IDepthBuffer* zbuffer);

	IBurningShader* createTriangleRendererReference(IDepthBuffer* zbuffer);



} // end namespace video
} // end namespace irr

#endif

