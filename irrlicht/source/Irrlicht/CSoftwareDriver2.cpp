// Copyright (C) 2002-2009 Nikolaus Gebhardt / Thomas Alten
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"
#include "CSoftwareDriver2.h"

#ifdef _IRR_COMPILE_WITH_BURNINGSVIDEO_

#include "SoftwareDriver2_helper.h"
#include "CSoftwareTexture2.h"
#include "CSoftware2MaterialRenderer.h"
#include "S3DVertex.h"
#include "S4DVertex.h"


#define MAT_TEXTURE(tex) ( (video::CSoftwareTexture2*) Material.org.getTexture ( tex ) )


namespace irr
{
namespace video
{


//! constructor
CBurningVideoDriver::CBurningVideoDriver(const core::dimension2d<u32>& windowSize, bool fullscreen, io::IFileSystem* io, video::IImagePresenter* presenter)
: CNullDriver(io, windowSize), BackBuffer(0), Presenter(presenter),
	WindowId(0), SceneSourceRect(0),
	RenderTargetTexture(0), RenderTargetSurface(0), CurrentShader(0),
	 DepthBuffer(0), CurrentOut ( 12 * 2, 128 ), Temp ( 12 * 2, 128 )
{
	#ifdef _DEBUG
	setDebugName("CBurningVideoDriver");
	#endif

	// create backbuffer
	BackBuffer = new CImage(BURNINGSHADER_COLOR_FORMAT, windowSize);
	if (BackBuffer)
	{
		BackBuffer->fill(SColor(0));

		// create z buffer
		DepthBuffer = video::createDepthBuffer(BackBuffer->getDimension());
	}

	// create triangle renderers

	irr::memset32 ( BurningShader, 0, sizeof ( BurningShader ) );
	//BurningShader[ETR_FLAT] = createTRFlat2(DepthBuffer);
	//BurningShader[ETR_FLAT_WIRE] = createTRFlatWire2(DepthBuffer);
	BurningShader[ETR_GOURAUD] = createTriangleRendererGouraud2(DepthBuffer);
	BurningShader[ETR_GOURAUD_ALPHA] = createTriangleRendererGouraudAlpha2(DepthBuffer );
	BurningShader[ETR_GOURAUD_ALPHA_NOZ] = createTRGouraudAlphaNoZ2(DepthBuffer );
	//BurningShader[ETR_GOURAUD_WIRE] = createTriangleRendererGouraudWire2(DepthBuffer);
	//BurningShader[ETR_TEXTURE_FLAT] = createTriangleRendererTextureFlat2(DepthBuffer);
	//BurningShader[ETR_TEXTURE_FLAT_WIRE] = createTriangleRendererTextureFlatWire2(DepthBuffer);
	BurningShader[ETR_TEXTURE_GOURAUD] = createTriangleRendererTextureGouraud2(DepthBuffer);
	BurningShader[ETR_TEXTURE_GOURAUD_LIGHTMAP_M1] = createTriangleRendererTextureLightMap2_M1(DepthBuffer);
	BurningShader[ETR_TEXTURE_GOURAUD_LIGHTMAP_M2] = createTriangleRendererTextureLightMap2_M2(DepthBuffer);
	BurningShader[ETR_TEXTURE_GOURAUD_LIGHTMAP_M4] = createTriangleRendererGTextureLightMap2_M4(DepthBuffer);
	BurningShader[ETR_TEXTURE_LIGHTMAP_M4] = createTriangleRendererTextureLightMap2_M4(DepthBuffer);
	BurningShader[ETR_TEXTURE_GOURAUD_LIGHTMAP_ADD] = createTriangleRendererTextureLightMap2_Add(DepthBuffer);
	BurningShader[ETR_TEXTURE_GOURAUD_DETAIL_MAP] = createTriangleRendererTextureDetailMap2(DepthBuffer);

	BurningShader[ETR_TEXTURE_GOURAUD_WIRE] = createTriangleRendererTextureGouraudWire2(DepthBuffer);
	BurningShader[ETR_TEXTURE_GOURAUD_NOZ] = createTRTextureGouraudNoZ2();
	BurningShader[ETR_TEXTURE_GOURAUD_ADD] = createTRTextureGouraudAdd2(DepthBuffer);
	BurningShader[ETR_TEXTURE_GOURAUD_ADD_NO_Z] = createTRTextureGouraudAddNoZ2(DepthBuffer);
	BurningShader[ETR_TEXTURE_GOURAUD_VERTEX_ALPHA] = createTriangleRendererTextureVertexAlpha2 ( DepthBuffer );

	BurningShader[ETR_TEXTURE_GOURAUD_ALPHA] = createTRTextureGouraudAlpha(DepthBuffer );
	BurningShader[ETR_TEXTURE_GOURAUD_ALPHA_NOZ] = createTRTextureGouraudAlphaNoZ( DepthBuffer );

	BurningShader[ETR_TEXTURE_BLEND] = createTRTextureBlend( DepthBuffer );

	BurningShader[ETR_REFERENCE] = createTriangleRendererReference ( DepthBuffer );


	// add the same renderer for all solid types
	CSoftware2MaterialRenderer_SOLID* smr = new CSoftware2MaterialRenderer_SOLID( this);
	CSoftware2MaterialRenderer_TRANSPARENT_ADD_COLOR* tmr = new CSoftware2MaterialRenderer_TRANSPARENT_ADD_COLOR( this);
	CSoftware2MaterialRenderer_UNSUPPORTED * umr = new CSoftware2MaterialRenderer_UNSUPPORTED ( this );

	//!TODO: addMaterialRenderer depends on pushing order....
	addMaterialRenderer ( smr ); // EMT_SOLID
	addMaterialRenderer ( smr ); // EMT_SOLID_2_LAYER,
	addMaterialRenderer ( smr ); // EMT_LIGHTMAP,
	addMaterialRenderer ( tmr ); // EMT_LIGHTMAP_ADD,
	addMaterialRenderer ( smr ); // EMT_LIGHTMAP_M2,
	addMaterialRenderer ( smr ); // EMT_LIGHTMAP_M4,
	addMaterialRenderer ( smr ); // EMT_LIGHTMAP_LIGHTING,
	addMaterialRenderer ( smr ); // EMT_LIGHTMAP_LIGHTING_M2,
	addMaterialRenderer ( smr ); // EMT_LIGHTMAP_LIGHTING_M4,
	addMaterialRenderer ( smr ); // EMT_DETAIL_MAP,
	addMaterialRenderer ( umr ); // EMT_SPHERE_MAP,
	addMaterialRenderer ( smr ); // EMT_REFLECTION_2_LAYER,
	addMaterialRenderer ( tmr ); // EMT_TRANSPARENT_ADD_COLOR,
	addMaterialRenderer ( tmr ); // EMT_TRANSPARENT_ALPHA_CHANNEL,
	addMaterialRenderer ( tmr ); // EMT_TRANSPARENT_ALPHA_CHANNEL_REF,
	addMaterialRenderer ( tmr ); // EMT_TRANSPARENT_VERTEX_ALPHA,
	addMaterialRenderer ( smr ); // EMT_TRANSPARENT_REFLECTION_2_LAYER,
	addMaterialRenderer ( umr ); // EMT_NORMAL_MAP_SOLID,
	addMaterialRenderer ( umr ); // EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR,
	addMaterialRenderer ( umr ); // EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA,
	addMaterialRenderer ( umr ); // EMT_PARALLAX_MAP_SOLID,
	addMaterialRenderer ( umr ); // EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR,
	addMaterialRenderer ( umr ); // EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA,
	addMaterialRenderer ( tmr ); // EMT_ONETEXTURE_BLEND

	smr->drop ();
	tmr->drop ();
	umr->drop ();

	// select render target
	setRenderTarget(BackBuffer);

	//reset Lightspace
	LightSpace.reset ();

	// select the right renderer
	setCurrentShader();
}


//! destructor
CBurningVideoDriver::~CBurningVideoDriver()
{
	// delete Backbuffer
	if (BackBuffer)
		BackBuffer->drop();

	// delete triangle renderers

	for (s32 i=0; i<ETR2_COUNT; ++i)
		if (BurningShader[i])
			BurningShader[i]->drop();

	// delete zbuffer

	if (DepthBuffer)
		DepthBuffer->drop();

	if (RenderTargetTexture)
		RenderTargetTexture->drop();

	if (RenderTargetSurface)
		RenderTargetSurface->drop();
}


/*!
	selects the right triangle renderer based on the render states.
*/
void CBurningVideoDriver::setCurrentShader()
{
	ITexture *texture0 = Material.org.getTexture(0);
	ITexture *texture1 = Material.org.getTexture(1);

	bool zMaterialTest =	Material.org.ZBuffer != ECFN_NEVER && 
							Material.org.ZWriteEnable &&
							( AllowZWriteOnTransparent || !Material.org.isTransparent() );

	EBurningFFShader shader = zMaterialTest ? ETR_TEXTURE_GOURAUD : ETR_TEXTURE_GOURAUD_NOZ;

	TransformationFlag[ ETS_TEXTURE_0] &= ~(ETF_TEXGEN_CAMERA_NORMAL|ETF_TEXGEN_CAMERA_REFLECTION);
	LightSpace.Flags &= ~VERTEXTRANSFORM;

	switch ( Material.org.MaterialType )
	{
		case EMT_ONETEXTURE_BLEND:
			shader = ETR_TEXTURE_BLEND;
			break;

		case EMT_TRANSPARENT_ALPHA_CHANNEL_REF:
			Material.org.MaterialTypeParam = 0.5f;
			// fall through
		case EMT_TRANSPARENT_ALPHA_CHANNEL:
			if ( texture0 && texture0->hasAlpha () )
			{
				shader = zMaterialTest ? ETR_TEXTURE_GOURAUD_ALPHA : ETR_TEXTURE_GOURAUD_ALPHA_NOZ;
				break;
			}
			// fall through

		case EMT_TRANSPARENT_ADD_COLOR:
			shader = zMaterialTest ? ETR_TEXTURE_GOURAUD_ADD : ETR_TEXTURE_GOURAUD_ADD_NO_Z;
			break;

		case EMT_TRANSPARENT_VERTEX_ALPHA:
			shader = ETR_TEXTURE_GOURAUD_VERTEX_ALPHA;
			break;

		case EMT_LIGHTMAP:
		case EMT_LIGHTMAP_LIGHTING:
			shader = ETR_TEXTURE_GOURAUD_LIGHTMAP_M1;
			break;

		case EMT_LIGHTMAP_M2:
		case EMT_LIGHTMAP_LIGHTING_M2:
			shader = ETR_TEXTURE_GOURAUD_LIGHTMAP_M2;
			break;

		case EMT_LIGHTMAP_LIGHTING_M4:
			if ( texture1 )
				shader = ETR_TEXTURE_GOURAUD_LIGHTMAP_M4;
			break;
		case EMT_LIGHTMAP_M4:
			if ( texture1 )
				shader = ETR_TEXTURE_LIGHTMAP_M4;
			break;

		case EMT_LIGHTMAP_ADD:
			if ( texture1 )
				shader = ETR_TEXTURE_GOURAUD_LIGHTMAP_ADD;
			break;

		case EMT_DETAIL_MAP:
			shader = ETR_TEXTURE_GOURAUD_DETAIL_MAP;
			break;

		case EMT_SPHERE_MAP:
			TransformationFlag[ ETS_TEXTURE_0] |= ETF_TEXGEN_CAMERA_REFLECTION; // ETF_TEXGEN_CAMERA_NORMAL;
			LightSpace.Flags |= VERTEXTRANSFORM;
			break;
		case EMT_REFLECTION_2_LAYER:
			shader = ETR_TEXTURE_GOURAUD_LIGHTMAP_M1;
			TransformationFlag[ ETS_TEXTURE_1] |= ETF_TEXGEN_CAMERA_REFLECTION;
			LightSpace.Flags |= VERTEXTRANSFORM;
			break;

		default:
			break;

	}

	if ( !texture0 )
	{
		shader = ETR_GOURAUD;
	}

	if ( Material.org.Wireframe )
	{
		shader = ETR_TEXTURE_GOURAUD_WIRE;
	}

	//shader = ETR_REFERENCE;

	// switchToTriangleRenderer
	CurrentShader = BurningShader[shader];
	if ( CurrentShader )
	{
		CurrentShader->setZCompareFunc ( Material.org.ZBuffer );
		CurrentShader->setRenderTarget(RenderTargetSurface, ViewPort);
		CurrentShader->setMaterial ( Material );

		switch ( shader )
		{
			case ETR_TEXTURE_GOURAUD_ALPHA:
			case ETR_TEXTURE_GOURAUD_ALPHA_NOZ:
			case ETR_TEXTURE_BLEND:
				CurrentShader->setParam ( 0, Material.org.MaterialTypeParam );
				break;
			default:
			break;
		}
	}

}



//! queries the features of the driver, returns true if feature is available
bool CBurningVideoDriver::queryFeature(E_VIDEO_DRIVER_FEATURE feature) const
{
	if (!FeatureEnabled[feature])
		return false;

	switch (feature)
	{
#ifdef SOFTWARE_DRIVER_2_BILINEAR
	case EVDF_BILINEAR_FILTER:
		return true;
#endif
#ifdef SOFTWARE_DRIVER_2_MIPMAPPING
	case EVDF_MIP_MAP:
		return true;
#endif
	case EVDF_RENDER_TO_TARGET:
	case EVDF_MULTITEXTURE:
	case EVDF_HARDWARE_TL:
	case EVDF_TEXTURE_NSQUARE:
		return true;

	default:
		return false;
	}
}



//! sets transformation
void CBurningVideoDriver::setTransform(E_TRANSFORMATION_STATE state, const core::matrix4& mat)
{
	Transformation[state] = mat;
	core::setbit_cond ( TransformationFlag[state], mat.isIdentity(), ETF_IDENTITY );

	switch ( state )
	{
		case ETS_VIEW:
			Transformation[ETS_VIEW_PROJECTION].setbyproduct_nocheck (
				Transformation[ETS_PROJECTION],
				Transformation[ETS_VIEW]
			);
			getCameraPosWorldSpace ();
			break;

		case ETS_WORLD:
			if ( TransformationFlag[state] & ETF_IDENTITY )
			{
				Transformation[ETS_CURRENT] = Transformation[ETS_VIEW_PROJECTION];
			}
			else
			{
				Transformation[ETS_CURRENT].setbyproduct_nocheck (
					Transformation[ETS_VIEW_PROJECTION],
					Transformation[ETS_WORLD]
				);
			}
			TransformationFlag[ETS_CURRENT] = 0;
			break;
		case ETS_TEXTURE_0:
		case ETS_TEXTURE_1:
		case ETS_TEXTURE_2:
		case ETS_TEXTURE_3:
			if ( 0 == (TransformationFlag[state] & ETF_IDENTITY ) )
				LightSpace.Flags |= VERTEXTRANSFORM;
		default:
			break;
	}
}


//! clears the zbuffer
bool CBurningVideoDriver::beginScene(bool backBuffer, bool zBuffer,
		SColor color, void* windowId, core::rect<s32>* sourceRect)
{
	CNullDriver::beginScene(backBuffer, zBuffer, color, windowId, sourceRect);
	WindowId = windowId;
	SceneSourceRect = sourceRect;

	if (backBuffer && BackBuffer)
		BackBuffer->fill(color);

	if (zBuffer && DepthBuffer)
		DepthBuffer->clear();

	memset ( TransformationFlag, 0, sizeof ( TransformationFlag ) );
	return true;
}


//! presents the rendered scene on the screen, returns false if failed
bool CBurningVideoDriver::endScene()
{
	CNullDriver::endScene();

	return Presenter->present(BackBuffer, WindowId, SceneSourceRect);
}


//! sets a render target
bool CBurningVideoDriver::setRenderTarget(video::ITexture* texture, bool clearBackBuffer,
								 bool clearZBuffer, SColor color)
{
	if (texture && texture->getDriverType() != EDT_BURNINGSVIDEO)
	{
		os::Printer::log("Fatal Error: Tried to set a texture not owned by this driver.", ELL_ERROR);
		return false;
	}

	if (RenderTargetTexture)
		RenderTargetTexture->drop();

	RenderTargetTexture = texture;

	if (RenderTargetTexture)
	{
		RenderTargetTexture->grab();
		setRenderTarget(((CSoftwareTexture2*)RenderTargetTexture)->getTexture());
	}
	else
	{
		setRenderTarget(BackBuffer);
	}

	if (RenderTargetSurface && (clearBackBuffer || clearZBuffer))
	{
		if (clearZBuffer)
			DepthBuffer->clear();

		if (clearBackBuffer)
			((video::CImage*)RenderTargetSurface)->fill( color );
	}

	return true;
}


//! sets a render target
void CBurningVideoDriver::setRenderTarget(video::CImage* image)
{
	if (RenderTargetSurface)
		RenderTargetSurface->drop();

	RenderTargetSurface = image;
	RenderTargetSize.Width = 0;
	RenderTargetSize.Height = 0;

	if (RenderTargetSurface)
	{
		RenderTargetSurface->grab();
		RenderTargetSize = RenderTargetSurface->getDimension();
	}

	setViewPort(core::rect<s32>(0,0,RenderTargetSize.Width,RenderTargetSize.Height));

	if (DepthBuffer)
		DepthBuffer->setSize(RenderTargetSize);
}



//! sets a viewport
void CBurningVideoDriver::setViewPort(const core::rect<s32>& area)
{
	ViewPort = area;

	core::rect<s32> rendert(0,0,RenderTargetSize.Width,RenderTargetSize.Height);
	ViewPort.clipAgainst(rendert);

	Transformation [ ETS_CLIPSCALE ].buildNDCToDCMatrix ( ViewPort, 1 );


	if (CurrentShader)
		CurrentShader->setRenderTarget(RenderTargetSurface, ViewPort);
}

/*
	generic plane clipping in homogenous coordinates
	special case ndc frustum <-w,w>,<-w,w>,<-w,w>
	can be rewritten with compares e.q near plane, a.z < -a.w and b.z < -b.w
*/

const sVec4 CBurningVideoDriver::NDCPlane[6] =
{
	sVec4(  0.f,  0.f, -1.f, -1.f ),	// near
	sVec4(  0.f,  0.f,  1.f, -1.f ),	// far
	sVec4(  1.f,  0.f,  0.f, -1.f ),	// left
	sVec4( -1.f,  0.f,  0.f, -1.f ),	// right
	sVec4(  0.f,  1.f,  0.f, -1.f ),	// bottom
	sVec4(  0.f, -1.f,  0.f, -1.f )		// top
};



/*
	test a vertex if it's inside the standard frustum

	this is the generic one..

	f32 dotPlane;
	for ( u32 i = 0; i!= 6; ++i )
	{
		dotPlane = v->Pos.dotProduct ( NDCPlane[i] );
		core::setbit_cond( flag, dotPlane <= 0.f, 1 << i );
	}

	// this is the base for ndc frustum <-w,w>,<-w,w>,<-w,w>
	core::setbit_cond( flag, ( v->Pos.z - v->Pos.w ) <= 0.f, 1 );
	core::setbit_cond( flag, (-v->Pos.z - v->Pos.w ) <= 0.f, 2 );
	core::setbit_cond( flag, ( v->Pos.x - v->Pos.w ) <= 0.f, 4 );
	core::setbit_cond( flag, (-v->Pos.x - v->Pos.w ) <= 0.f, 8 );
	core::setbit_cond( flag, ( v->Pos.y - v->Pos.w ) <= 0.f, 16 );
	core::setbit_cond( flag, (-v->Pos.y - v->Pos.w ) <= 0.f, 32 );

*/
#ifdef IRRLICHT_FAST_MATH

REALINLINE u32 CBurningVideoDriver::clipToFrustumTest ( const s4DVertex * v  ) const
{
	f32 test[6];
	u32 flag;
	const f32 w = - v->Pos.w;

	// a conditional move is needed....FCOMI ( but we don't have it )
	// so let the fpu calculate and write it back.
	// cpu makes the compare, interleaving

	test[0] =  v->Pos.z + w;
	test[1] = -v->Pos.z + w;
	test[2] =  v->Pos.x + w;
	test[3] = -v->Pos.x + w;
	test[4] =  v->Pos.y + w;
	test[5] = -v->Pos.y + w;

	flag  = (IR ( test[0] )              ) >> 31;
	flag |= (IR ( test[1] ) & 0x80000000 ) >> 30;
	flag |= (IR ( test[2] ) & 0x80000000 ) >> 29;
	flag |= (IR ( test[3] ) & 0x80000000 ) >> 28;
	flag |= (IR ( test[4] ) & 0x80000000 ) >> 27;
	flag |= (IR ( test[5] ) & 0x80000000 ) >> 26;

/*
	flag  = F32_LOWER_EQUAL_0 ( test[0] );
	flag |= F32_LOWER_EQUAL_0 ( test[1] ) << 1;
	flag |= F32_LOWER_EQUAL_0 ( test[2] ) << 2;
	flag |= F32_LOWER_EQUAL_0 ( test[3] ) << 3;
	flag |= F32_LOWER_EQUAL_0 ( test[4] ) << 4;
	flag |= F32_LOWER_EQUAL_0 ( test[5] ) << 5;
*/
	return flag;
}

#else


REALINLINE u32 CBurningVideoDriver::clipToFrustumTest ( const s4DVertex * v  ) const
{
	u32 flag = 0;

	if ( v->Pos.z <= v->Pos.w ) flag |= 1;
	if (-v->Pos.z <= v->Pos.w ) flag |= 2;

	if ( v->Pos.x <= v->Pos.w ) flag |= 4;
	if (-v->Pos.x <= v->Pos.w ) flag |= 8;

	if ( v->Pos.y <= v->Pos.w ) flag |= 16;
	if (-v->Pos.y <= v->Pos.w ) flag |= 32;

/*
	for ( u32 i = 0; i!= 6; ++i )
	{
		core::setbit_cond( flag, v->Pos.dotProduct ( NDCPlane[i] ) <= 0.f, 1 << i );
	}
*/
	return flag;
}

#endif // _MSC_VER

u32 CBurningVideoDriver::clipToHyperPlane ( s4DVertex * dest, const s4DVertex * source, u32 inCount, const sVec4 &plane )
{
	u32 outCount = 0;
	s4DVertex * out = dest;

	const s4DVertex * a;
	const s4DVertex * b = source;

	f32 bDotPlane;

	bDotPlane = b->Pos.dotProduct ( plane );

	for( u32 i = 1; i < inCount + 1; ++i)
	{
		const s32 condition = i - inCount;
		const s32 index = (( ( condition >> 31 ) & ( i ^ condition ) ) ^ condition ) << 1;

		a = &source[ index ];

		// current point inside
		if ( a->Pos.dotProduct ( plane ) <= 0.f )
		{
			// last point outside
			if ( F32_GREATER_0 ( bDotPlane ) )
			{
				// intersect line segment with plane
				out->interpolate ( *b, *a, bDotPlane / (b->Pos - a->Pos).dotProduct ( plane ) );
				out += 2;
				outCount += 1;
			}

			// copy current to out
			//*out = *a;
			irr::memcpy32_small ( out, a, SIZEOF_SVERTEX * 2 );
			b = out;

			out += 2;
			outCount += 1;
		}
		else
		{
			// current point outside

			if ( F32_LOWER_EQUAL_0 (  bDotPlane ) )
			{
				// previous was inside
				// intersect line segment with plane
				out->interpolate ( *b, *a, bDotPlane / (b->Pos - a->Pos).dotProduct ( plane ) );
				out += 2;
				outCount += 1;
			}
			// pointer
			b = a;
		}

		bDotPlane = b->Pos.dotProduct ( plane );

	}

	return outCount;
}


u32 CBurningVideoDriver::clipToFrustum ( s4DVertex *v0, s4DVertex * v1, const u32 vIn )
{
	u32 vOut = vIn;

	vOut = clipToHyperPlane ( v1, v0, vOut, NDCPlane[0] ); if ( vOut < vIn ) return vOut;
	vOut = clipToHyperPlane ( v0, v1, vOut, NDCPlane[1] ); if ( vOut < vIn ) return vOut;
	vOut = clipToHyperPlane ( v1, v0, vOut, NDCPlane[2] ); if ( vOut < vIn ) return vOut;
	vOut = clipToHyperPlane ( v0, v1, vOut, NDCPlane[3] ); if ( vOut < vIn ) return vOut;
	vOut = clipToHyperPlane ( v1, v0, vOut, NDCPlane[4] ); if ( vOut < vIn ) return vOut;
	vOut = clipToHyperPlane ( v0, v1, vOut, NDCPlane[5] );
	return vOut;
}

/*!
 Part I:
	apply Clip Scale matrix
	From Normalized Device Coordiante ( NDC ) Space to Device Coordinate Space ( DC )

 Part II:
	Project homogeneous vector
	homogeneous to non-homogenous coordinates ( dividebyW )

	Incoming: ( xw, yw, zw, w, u, v, 1, R, G, B, A )
	Outgoing: ( xw/w, yw/w, zw/w, w/w, u/w, v/w, 1/w, R/w, G/w, B/w, A/w )


	replace w/w by 1/w
*/
inline void CBurningVideoDriver::ndc_2_dc_and_project ( s4DVertex *dest,s4DVertex *source, u32 vIn ) const
{
	u32 g;

	for ( g = 0; g != vIn; g += 2 )
	{
		if ( (dest[g].flag & VERTEX4D_PROJECTED ) == VERTEX4D_PROJECTED )
			continue;

		dest[g].flag = source[g].flag | VERTEX4D_PROJECTED;

		const f32 w = source[g].Pos.w;
		const f32 iw = core::reciprocal ( w );

		// to device coordinates
		dest[g].Pos.x = iw * ( source[g].Pos.x * Transformation [ ETS_CLIPSCALE ][ 0] + w * Transformation [ ETS_CLIPSCALE ][12] );
		dest[g].Pos.y = iw * ( source[g].Pos.y * Transformation [ ETS_CLIPSCALE ][ 5] + w * Transformation [ ETS_CLIPSCALE ][13] );

#ifndef SOFTWARE_DRIVER_2_USE_WBUFFER
		dest[g].Pos.z = iw * source[g].Pos.z;
#endif

	#ifdef SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
		#ifdef SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT
			dest[g].Color[0] = source[g].Color[0] * iw;
		#else
			dest[g].Color[0] = source[g].Color[0];
		#endif

	#endif

		dest[g].Pos.w = iw;
	}
}


inline void CBurningVideoDriver::ndc_2_dc_and_project2 ( const s4DVertex **v, const u32 size ) const
{
	u32 g;

	for ( g = 0; g != size; g += 1 )
	{
		s4DVertex * a = (s4DVertex*) v[g];

		if ( (a[1].flag & VERTEX4D_PROJECTED ) == VERTEX4D_PROJECTED )
			continue;

		a[1].flag = a->flag | VERTEX4D_PROJECTED;

		// project homogenous vertex, store 1/w
		const f32 w = a->Pos.w;
		const f32 iw = core::reciprocal ( w );

		// to device coordinates
		const f32 * p = Transformation [ ETS_CLIPSCALE ].pointer();
		a[1].Pos.x = iw * ( a->Pos.x * p[ 0] + w * p[12] );
		a[1].Pos.y = iw * ( a->Pos.y * p[ 5] + w * p[13] );

#ifndef SOFTWARE_DRIVER_2_USE_WBUFFER
		a[1].Pos.z = a->Pos.z * iw;
#endif

	#ifdef SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
		#ifdef SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT
			a[1].Color[0] = a->Color[0] * iw;
		#else
			a[1].Color[0] = a->Color[0];
		#endif
	#endif

		a[1].Pos.w = iw;

	}

}


/*!
	crossproduct in projected 2D -> screen area triangle
*/
inline f32 CBurningVideoDriver::screenarea ( const s4DVertex *v ) const
{
	return	( ( v[3].Pos.x - v[1].Pos.x ) * ( v[5].Pos.y - v[1].Pos.y ) ) -
			( ( v[3].Pos.y - v[1].Pos.y ) * ( v[5].Pos.x - v[1].Pos.x ) );
}


/*!
*/
inline f32 CBurningVideoDriver::texelarea ( const s4DVertex *v, int tex ) const
{
	f32 z;

	z =		( (v[2].Tex[tex].x - v[0].Tex[tex].x ) * (v[4].Tex[tex].y - v[0].Tex[tex].y ) )
		 -	( (v[4].Tex[tex].x - v[0].Tex[tex].x ) * (v[2].Tex[tex].y - v[0].Tex[tex].y ) );

	return MAT_TEXTURE ( tex )->getLODFactor ( z );
}

/*!
	crossproduct in projected 2D
*/
inline f32 CBurningVideoDriver::screenarea2 ( const s4DVertex **v ) const
{
	return	( (( v[1] + 1 )->Pos.x - (v[0] + 1 )->Pos.x ) * ( (v[2] + 1 )->Pos.y - (v[0] + 1 )->Pos.y ) ) -
			( (( v[1] + 1 )->Pos.y - (v[0] + 1 )->Pos.y ) * ( (v[2] + 1 )->Pos.x - (v[0] + 1 )->Pos.x ) );
}

/*!
*/
inline f32 CBurningVideoDriver::texelarea2 ( const s4DVertex **v, s32 tex ) const
{
	f32 z;
	z =		( (v[1]->Tex[tex].x - v[0]->Tex[tex].x ) * (v[2]->Tex[tex].y - v[0]->Tex[tex].y ) )
		 -	( (v[2]->Tex[tex].x - v[0]->Tex[tex].x ) * (v[1]->Tex[tex].y - v[0]->Tex[tex].y ) );

	return MAT_TEXTURE ( tex )->getLODFactor ( z );
}


/*!
*/
inline void CBurningVideoDriver::select_polygon_mipmap ( s4DVertex *v, u32 vIn, u32 tex, const core::dimension2du& texSize )
{
	f32 f[2];

	f[0] = (f32) texSize.Width - 0.25f;
	f[1] = (f32) texSize.Height - 0.25f;

#ifdef SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT
	for ( u32 g = 0; g != vIn; g += 2 )
	{
		(v + g + 1 )->Tex[tex].x	= (v + g + 0)->Tex[tex].x * ( v + g + 1 )->Pos.w * f[0];
		(v + g + 1 )->Tex[tex].y	= (v + g + 0)->Tex[tex].y * ( v + g + 1 )->Pos.w * f[1];
	}
#else
	for ( u32 g = 0; g != vIn; g += 2 )
	{
		(v + g + 1 )->Tex[tex].x	= (v + g + 0)->Tex[tex].x * f[0];
		(v + g + 1 )->Tex[tex].y	= (v + g + 0)->Tex[tex].y * f[1];
	}
#endif
}

inline void CBurningVideoDriver::select_polygon_mipmap2 ( s4DVertex **v, u32 tex, const core::dimension2du& texSize ) const
{
	f32 f[2];

	f[0] = (f32) texSize.Width - 0.25f;
	f[1] = (f32) texSize.Height - 0.25f;

#ifdef SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT
	(v[0] + 1 )->Tex[tex].x	= v[0]->Tex[tex].x * ( v[0] + 1 )->Pos.w * f[0];
	(v[0] + 1 )->Tex[tex].y	= v[0]->Tex[tex].y * ( v[0] + 1 )->Pos.w * f[1];

	(v[1] + 1 )->Tex[tex].x	= v[1]->Tex[tex].x * ( v[1] + 1 )->Pos.w * f[0];
	(v[1] + 1 )->Tex[tex].y	= v[1]->Tex[tex].y * ( v[1] + 1 )->Pos.w * f[1];

	(v[2] + 1 )->Tex[tex].x	= v[2]->Tex[tex].x * ( v[2] + 1 )->Pos.w * f[0];
	(v[2] + 1 )->Tex[tex].y	= v[2]->Tex[tex].y * ( v[2] + 1 )->Pos.w * f[1];

#else
	(v[0] + 1 )->Tex[tex].x	= v[0]->Tex[tex].x * f[0];
	(v[0] + 1 )->Tex[tex].y	= v[0]->Tex[tex].y * f[1];

	(v[1] + 1 )->Tex[tex].x	= v[1]->Tex[tex].x * f[0];
	(v[1] + 1 )->Tex[tex].y	= v[1]->Tex[tex].y * f[1];

	(v[2] + 1 )->Tex[tex].x	= v[2]->Tex[tex].x * f[0];
	(v[2] + 1 )->Tex[tex].y	= v[2]->Tex[tex].y * f[1];
#endif
}

// Vertex Cache
const SVSize CBurningVideoDriver::vSize[] =
{
	{ VERTEX4D_FORMAT_TEXTURE_1 | VERTEX4D_FORMAT_COLOR_1, sizeof(S3DVertex), 1 },
	{ VERTEX4D_FORMAT_TEXTURE_2 | VERTEX4D_FORMAT_COLOR_1, sizeof(S3DVertex2TCoords),2 },
	{ VERTEX4D_FORMAT_TEXTURE_2 | VERTEX4D_FORMAT_COLOR_1, sizeof(S3DVertexTangents),2 },
	{ VERTEX4D_FORMAT_TEXTURE_2 | VERTEX4D_FORMAT_COLOR_1, sizeof(S3DVertex), 2 },
};



/*!
	fill a cache line with transformed, light and clipp test triangles
*/
void CBurningVideoDriver::VertexCache_fill(const u32 sourceIndex,
					const u32 destIndex)
{
	u8 * source;
	s4DVertex *dest;

	source = (u8*) VertexCache.vertices + ( sourceIndex * vSize[VertexCache.vType].Pitch );

	// it's a look ahead so we never hit it..
	// but give priority...
	//VertexCache.info[ destIndex ].hit = hitCount;

	// store info
	VertexCache.info[ destIndex ].index = sourceIndex;
	VertexCache.info[ destIndex ].hit = 0;

	// destination Vertex
	dest = (s4DVertex *) ( (u8*) VertexCache.mem.data + ( destIndex << ( SIZEOF_SVERTEX_LOG2 + 1  ) ) );

	// transform Model * World * Camera * Projection * NDCSpace matrix
	const S3DVertex *base = ((S3DVertex*) source );
	Transformation [ ETS_CURRENT].transformVect ( &dest->Pos.x, base->Pos );


#if defined (SOFTWARE_DRIVER_2_LIGHTING) || defined ( SOFTWARE_DRIVER_2_TEXTURE_TRANSFORM )

	// vertex normal in light space
	if ( Material.org.Lighting || (LightSpace.Flags & VERTEXTRANSFORM) )
	{
		if ( TransformationFlag[ETS_WORLD] & ETF_IDENTITY )
		{
			LightSpace.normal.set ( base->Normal.X, base->Normal.Y, base->Normal.Z, 1.f );
			LightSpace.vertex.set ( base->Pos.X, base->Pos.Y, base->Pos.Z, 1.f );
		}
		else
		{
			Transformation[ETS_WORLD].rotateVect ( &LightSpace.normal.x, base->Normal );
			if ( LightSpace.Flags & NORMALIZE )
				LightSpace.normal.normalize_xyz();

			// vertex in light space
			if ( LightSpace.Flags & ( POINTLIGHT | FOG | SPECULAR | VERTEXTRANSFORM) )
				Transformation[ETS_WORLD].transformVect ( &LightSpace.vertex.x, base->Pos );
		}

	}
#endif

#if defined ( SOFTWARE_DRIVER_2_USE_VERTEX_COLOR )
	// apply lighting model
	#if defined (SOFTWARE_DRIVER_2_LIGHTING)
		if ( Material.org.Lighting )
		{
			lightVertex ( dest, base->Color.color );
		}
		else
		{
			dest->Color[0].setA8R8G8B8 ( base->Color.color );
		}
	#else
		dest->Color[0].setA8R8G8B8 ( base->Color.color );
	#endif
#endif

	// Texture Transform
#if !defined ( SOFTWARE_DRIVER_2_TEXTURE_TRANSFORM )
	irr::memcpy32_small ( &dest->Tex[0],&base->TCoords,
					vSize[VertexCache.vType].TexSize << 3 //  * ( sizeof ( f32 ) * 2 )
				);
#else

	if ( 0 == (LightSpace.Flags & VERTEXTRANSFORM) )
	{
		irr::memcpy32_small ( &dest->Tex[0],&base->TCoords,
						vSize[VertexCache.vType].TexSize << 3 //  * ( sizeof ( f32 ) * 2 )
					);
	}
	else
	{
	/*
			Generate texture coordinates as linear functions so that:
				u = Ux*x + Uy*y + Uz*z + Uw
				v = Vx*x + Vy*y + Vz*z + Vw
			The matrix M for this case is:
				Ux  Vx  0  0
				Uy  Vy  0  0
				Uz  Vz  0  0
				Uw  Vw  0  0
	*/

		u32 t;
		sVec4 n;
		sVec2 srcT;

		for ( t = 0; t != vSize[VertexCache.vType].TexSize; ++t )
		{
			const core::matrix4& M = Transformation [ ETS_TEXTURE_0 + t ];

			// texgen
			if ( TransformationFlag [ ETS_TEXTURE_0 + t ] & (ETF_TEXGEN_CAMERA_NORMAL|ETF_TEXGEN_CAMERA_REFLECTION) )
			{
				n.x = LightSpace.campos.x - LightSpace.vertex.x;
				n.y = LightSpace.campos.x - LightSpace.vertex.y;
				n.z = LightSpace.campos.x - LightSpace.vertex.z;
				n.normalize_xyz();
				n.x += LightSpace.normal.x;
				n.y += LightSpace.normal.y;
				n.z += LightSpace.normal.z;
				n.normalize_xyz();

				const f32 *view = Transformation[ETS_VIEW].pointer();

				if ( TransformationFlag [ ETS_TEXTURE_0 + t ] & ETF_TEXGEN_CAMERA_REFLECTION )
				{
					srcT.x = 0.5f * ( 1.f + (n.x * view[0] + n.y * view[4] + n.z * view[8] )); 
					srcT.y = 0.5f * ( 1.f + (n.x * view[1] + n.y * view[5] + n.z * view[9] ));
				}
				else
				{
					srcT.x = 0.5f * ( 1.f + (n.x * view[0] + n.y * view[1] + n.z * view[2] )); 
					srcT.y = 0.5f * ( 1.f + (n.x * view[4] + n.y * view[5] + n.z * view[6] ));
				}
			}
			else
			{
				irr::memcpy32_small ( &srcT,(&base->TCoords) + t,
					sizeof ( f32 ) * 2 );
			}

			switch ( Material.org.TextureLayer[t].TextureWrap )
			{
				case ETC_REPEAT:
				default:
					dest->Tex[t].x = M[0] * srcT.x + M[4] * srcT.y + M[8];
					dest->Tex[t].y = M[1] * srcT.x + M[5] * srcT.y + M[9];
					break;
				case ETC_CLAMP:
				case ETC_CLAMP_TO_EDGE:
					dest->Tex[t].x = core::clamp ( (f32) ( M[0] * srcT.x + M[4] * srcT.y + M[8] ), 0.f, 1.f );
					dest->Tex[t].y = core::clamp ( (f32) ( M[1] * srcT.x + M[5] * srcT.y + M[9] ), 0.f, 1.f );
					break;
			}
		}

	}
#endif

	dest[0].flag = dest[1].flag = vSize[VertexCache.vType].Format;

	// test vertex
	dest[0].flag |= clipToFrustumTest ( dest);

	// to DC Space, project homogenous vertex
	if ( (dest[0].flag & VERTEX4D_CLIPMASK ) == VERTEX4D_INSIDE )
	{
		ndc_2_dc_and_project2 ( (const s4DVertex**) &dest, 1 );
	}

	//return dest;
}

//

REALINLINE s4DVertex * CBurningVideoDriver::VertexCache_getVertex ( const u32 sourceIndex )
{
	for ( s32 i = 0; i < VERTEXCACHE_ELEMENT; ++i )
	{
		if ( VertexCache.info[ i ].index == sourceIndex )
		{
			return (s4DVertex *) ( (u8*) VertexCache.mem.data + ( i << ( SIZEOF_SVERTEX_LOG2 + 1  ) ) );
		}
	}
	return 0;
}


/*
	Cache based on linear walk indices
	fill blockwise on the next 16(Cache_Size) unique vertices in indexlist
	merge the next 16 vertices with the current
*/
REALINLINE void CBurningVideoDriver::VertexCache_get ( s4DVertex ** face )
{
	SCacheInfo info[VERTEXCACHE_ELEMENT];

	// next primitive must be complete in cache
	if (	VertexCache.indicesIndex - VertexCache.indicesRun < 3 &&
			VertexCache.indicesIndex < VertexCache.indexCount
		)
	{
		// rewind to start of primitive
		VertexCache.indicesIndex = VertexCache.indicesRun;

		irr::memset32 ( info, VERTEXCACHE_MISS, sizeof ( info ) );

		// get the next unique vertices cache line
		u32 fillIndex = 0;
		u32 dIndex;
		u32 i;
		u32 sourceIndex;

		while ( VertexCache.indicesIndex < VertexCache.indexCount &&
				fillIndex < VERTEXCACHE_ELEMENT
				)
		{
			sourceIndex = VertexCache.iType == 1 ?
			((u16*)VertexCache.indices) [ VertexCache.indicesIndex ] :
			((u32*)VertexCache.indices) [ VertexCache.indicesIndex ];


			VertexCache.indicesIndex += 1;

			// if not exist, push back
			s32 exist = 0;
			for ( dIndex = 0;  dIndex < fillIndex; ++dIndex )
			{
				if ( info[ dIndex ].index == sourceIndex )
				{
					exist = 1;
					break;
				}
			}

			if ( 0 == exist )
			{
				info[fillIndex++].index = sourceIndex;
			}
		}

		// clear marks
		for ( i = 0; i!= VERTEXCACHE_ELEMENT; ++i )
		{
			VertexCache.info[i].hit = 0;
		}

		// mark all exisiting
		for ( i = 0; i!= fillIndex; ++i )
		{
			for ( dIndex = 0;  dIndex < VERTEXCACHE_ELEMENT; ++dIndex )
			{
				if ( VertexCache.info[ dIndex ].index == info[i].index )
				{
					info[i].hit = dIndex;
					VertexCache.info[ dIndex ].hit = 1;
					break;
				}
			}
		}

		// fill new
		for ( i = 0; i!= fillIndex; ++i )
		{
			if ( info[i].hit != VERTEXCACHE_MISS )
				continue;

			for ( dIndex = 0;  dIndex < VERTEXCACHE_ELEMENT; ++dIndex )
			{
				if ( 0 == VertexCache.info[dIndex].hit )
				{
					VertexCache_fill ( info[i].index, dIndex );
					VertexCache.info[dIndex].hit += 1;
					info[i].hit = dIndex;
					break;
				}
			}
		}
	}

	const u32 i0 = core::if_c_a_else_0 ( VertexCache.pType != scene::EPT_TRIANGLE_FAN, VertexCache.indicesRun );

	if ( VertexCache.iType == 1 )
	{
		const u16 *p = (const u16 *) VertexCache.indices;
		face[0] = VertexCache_getVertex ( p[ i0    ] );
		face[1] = VertexCache_getVertex ( p[ VertexCache.indicesRun + 1] );
		face[2] = VertexCache_getVertex ( p[ VertexCache.indicesRun + 2] );
	}
	else
	{
		const u32 *p = (const u32 *) VertexCache.indices;
		face[0] = VertexCache_getVertex ( p[ i0    ] );
		face[1] = VertexCache_getVertex ( p[ VertexCache.indicesRun + 1] );
		face[2] = VertexCache_getVertex ( p[ VertexCache.indicesRun + 2] );
	}

	VertexCache.indicesRun += VertexCache.primitivePitch;
}

/*!
*/
REALINLINE void CBurningVideoDriver::VertexCache_getbypass ( s4DVertex ** face )
{
	const u32 i0 = core::if_c_a_else_0 ( VertexCache.pType != scene::EPT_TRIANGLE_FAN, VertexCache.indicesRun );

	if ( VertexCache.iType == 1 )
	{
		const u16 *p = (const u16 *) VertexCache.indices;
		VertexCache_fill ( p[ i0    ], 0 );
		VertexCache_fill ( p[ VertexCache.indicesRun + 1], 1 );
		VertexCache_fill ( p[ VertexCache.indicesRun + 2], 2 );
	}
	else
	{
		const u32 *p = (const u32 *) VertexCache.indices;
		VertexCache_fill ( p[ i0    ], 0 );
		VertexCache_fill ( p[ VertexCache.indicesRun + 1], 1 );
		VertexCache_fill ( p[ VertexCache.indicesRun + 2], 2 );
	}

	VertexCache.indicesRun += VertexCache.primitivePitch;

	face[0] = (s4DVertex *) ( (u8*) VertexCache.mem.data + ( 0 << ( SIZEOF_SVERTEX_LOG2 + 1  ) ) );
	face[1] = (s4DVertex *) ( (u8*) VertexCache.mem.data + ( 1 << ( SIZEOF_SVERTEX_LOG2 + 1  ) ) );
	face[2] = (s4DVertex *) ( (u8*) VertexCache.mem.data + ( 2 << ( SIZEOF_SVERTEX_LOG2 + 1  ) ) );

}

/*!
*/
void CBurningVideoDriver::VertexCache_reset ( const void* vertices, u32 vertexCount,
											const void* indices, u32 primitiveCount,
											E_VERTEX_TYPE vType,scene::E_PRIMITIVE_TYPE pType,
											E_INDEX_TYPE iType)
{
	VertexCache.vertices = vertices;
	VertexCache.vertexCount = vertexCount;

	VertexCache.indices = indices;
	VertexCache.indicesIndex = 0;
	VertexCache.indicesRun = 0;

	if ( Material.org.MaterialType == video::EMT_REFLECTION_2_LAYER )
		VertexCache.vType = 3;
	else
		VertexCache.vType = vType;
	VertexCache.pType = pType;
	VertexCache.iType = iType == EIT_16BIT ? 1 : 2;

	switch ( VertexCache.pType )
	{
		case scene::EPT_TRIANGLES:
			VertexCache.indexCount = primitiveCount + primitiveCount + primitiveCount;
			VertexCache.primitivePitch = 3;
			break;
		case scene::EPT_TRIANGLE_FAN:
			VertexCache.indexCount = primitiveCount + 2;
			VertexCache.primitivePitch = 1;
			break;
	}

	irr::memset32 ( VertexCache.info, VERTEXCACHE_MISS, sizeof ( VertexCache.info ) );

}


void CBurningVideoDriver::drawVertexPrimitiveList(const void* vertices, u32 vertexCount,
				const void* indexList, u32 primitiveCount,
				E_VERTEX_TYPE vType, scene::E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType)

{
	if (!checkPrimitiveCount(primitiveCount))
		return;

	CNullDriver::drawVertexPrimitiveList(vertices, vertexCount, indexList, primitiveCount, vType, pType, iType);

	if ( 0 == CurrentShader )
		return;

	VertexCache_reset ( vertices, vertexCount, indexList, primitiveCount, vType, pType, iType );

	const s4DVertex * face[3];

	f32 dc_area;
	s32 lodLevel;
	u32 i;
	u32 g;
	video::CSoftwareTexture2* tex;

	for ( i = 0; i < (u32) primitiveCount; ++i )
	{
		VertexCache_get ( (s4DVertex**) face );

		// if fully outside or outside on same side
		if ( ( (face[0]->flag | face[1]->flag | face[2]->flag) & VERTEX4D_CLIPMASK )
				!= VERTEX4D_INSIDE
			)
			continue;

		// if fully inside
		if ( ( face[0]->flag & face[1]->flag & face[2]->flag & VERTEX4D_CLIPMASK ) == VERTEX4D_INSIDE )
		{
			dc_area = screenarea2 ( face );
			if ( Material.org.BackfaceCulling && F32_LOWER_EQUAL_0( dc_area ) )
				continue;
			if ( Material.org.FrontfaceCulling && F32_GREATER_EQUAL_0( dc_area ) )
				continue;

			dc_area = core::reciprocal ( dc_area );

			// select mipmap

			for ( g = 0; g != vSize[VertexCache.vType].TexSize; ++g )
			//for ( g = 0; g != BURNING_MATERIAL_MAX_TEXTURES; ++g )
			{
				if ( 0 == (tex = MAT_TEXTURE ( g )) )
				{
					CurrentShader->setTextureParam(g, 0, 0);
					continue;
				}

				lodLevel = s32_log2_f32 ( texelarea2 ( face, g ) * dc_area );
				CurrentShader->setTextureParam(g, tex, lodLevel);
				select_polygon_mipmap2 ( (s4DVertex**) face, g, tex->getSize() );

			}

			// rasterize
			CurrentShader->drawTriangle ( face[0] + 1, face[1] + 1, face[2] + 1 );
			continue;
		}

		// else if not complete inside clipping necessary
		irr::memcpy32_small ( ( (u8*) CurrentOut.data + ( 0 << ( SIZEOF_SVERTEX_LOG2 + 1 ) ) ), face[0], SIZEOF_SVERTEX * 2 );
		irr::memcpy32_small ( ( (u8*) CurrentOut.data + ( 1 << ( SIZEOF_SVERTEX_LOG2 + 1 ) ) ), face[1], SIZEOF_SVERTEX * 2 );
		irr::memcpy32_small ( ( (u8*) CurrentOut.data + ( 2 << ( SIZEOF_SVERTEX_LOG2 + 1 ) ) ), face[2], SIZEOF_SVERTEX * 2 );

		const u32 flag = CurrentOut.data->flag & VERTEX4D_FORMAT_MASK;

		for ( g = 0; g != CurrentOut.ElementSize; ++g )
		{
			CurrentOut.data[g].flag = flag;
			Temp.data[g].flag = flag;
		}

		u32 vOut;
		vOut = clipToFrustum ( CurrentOut.data, Temp.data, 3 );
/*
		if ( vOut < 3 )
		{
			char buf[256];
			struct SCheck
			{
				u32 flag;
				const char * name;
			};

			SCheck check[5];
			check[0].flag = face[0]->flag;
			check[0].name = "face0";
			check[1].flag = face[1]->flag;
			check[1].name = "face1";
			check[2].flag = face[2]->flag;
			check[2].name = "face2";
			check[3].flag = (face[0]->flag & face[1]->flag & face[2]->flag);
			check[3].name = "AND  ";
			check[4].flag = (face[0]->flag | face[1]->flag | face[2]->flag);
			check[4].name = "OR   ";

			for ( s32 h = 0; h!= 5; ++h )
			{
				sprintf ( buf, "%s: %d %d %d %d %d %d",
								check[h].name,
								( check[h].flag & 1 ),
								( check[h].flag & 2 ) >> 1,
								( check[h].flag & 4 ) >> 2,
								( check[h].flag & 8 ) >> 3,
								( check[h].flag & 16 ) >> 4,
								( check[h].flag & 32 ) >> 5
							);
				os::Printer::print ( buf );
			}

			sprintf ( buf, "Vout: %d\n", vOut );
			os::Printer::print ( buf );

			int hold = 1;
		}
*/
		if ( vOut < 3 )
			continue;

		vOut <<= 1;

		// to DC Space, project homogenous vertex
		ndc_2_dc_and_project ( CurrentOut.data + 1, CurrentOut.data, vOut );

/*
		// TODO: don't stick on 32 Bit Pointer
		#define PointerAsValue(x) ( (u32) (u32*) (x) ) 

		// if not complete inside clipping necessary
		if ( ( test & VERTEX4D_INSIDE ) != VERTEX4D_INSIDE )
		{
			u32 v[2] = { PointerAsValue ( Temp ) , PointerAsValue ( CurrentOut ) };
			for ( g = 0; g != 6; ++g )
			{
				vOut = clipToHyperPlane ( (s4DVertex*) v[0], (s4DVertex*) v[1], vOut, NDCPlane[g] );
				if ( vOut < 3 )
					break;

				v[0] ^= v[1];
				v[1] ^= v[0];
				v[0] ^= v[1];
			}

			if ( vOut < 3 )
				continue;

		}
*/

		// check 2d backface culling on first
		dc_area = screenarea ( CurrentOut.data );
		if ( Material.org.BackfaceCulling && F32_LOWER_EQUAL_0 ( dc_area ) )
			continue;
		if ( Material.org.FrontfaceCulling && F32_GREATER_EQUAL_0( dc_area ) )
			continue;

		// select mipmap
		//for ( g = 0; g != BURNING_MATERIAL_MAX_TEXTURES; ++g )
		for ( g = 0; g != vSize[VertexCache.vType].TexSize; ++g )
		{
			if ( 0 == (tex = MAT_TEXTURE ( g )) )
			{
				CurrentShader->setTextureParam(g, 0, 0);
				continue;
			}

			lodLevel = s32_log2_f32 ( texelarea ( CurrentOut.data, g ) / dc_area );
			CurrentShader->setTextureParam(g, tex, lodLevel);
			select_polygon_mipmap ( CurrentOut.data, vOut, g, tex->getSize() );
		}

		// re-tesselate ( triangle-fan, 0-1-2,0-2-3.. )
		for ( g = 0; g <= vOut - 6; g += 2 )
		{
			// rasterize
			CurrentShader->drawTriangle ( CurrentOut.data + 0 + 1,
							CurrentOut.data + g + 3,
							CurrentOut.data + g + 5);
		}

	}

	// dump statistics
/*
	char buf [64];
	sprintf ( buf,"VCount:%d PCount:%d CacheMiss: %d",
					vertexCount, primitiveCount,
					VertexCache.CacheMiss
				);
	os::Printer::print ( buf );
*/

}


//! Sets the dynamic ambient light color. The default color is
//! (0,0,0,0) which means it is dark.
//! \param color: New color of the ambient light.
void CBurningVideoDriver::setAmbientLight(const SColorf& color)
{
	LightSpace.Global_AmbientLight.setColorf ( color );
}


//! adds a dynamic light
s32 CBurningVideoDriver::addDynamicLight(const SLight& dl)
{
	(void) CNullDriver::addDynamicLight( dl );

	SBurningShaderLight l;
//	l.org = dl;
	l.Type = dl.Type;
	l.LightIsOn = true;

	l.AmbientColor.setColorf ( dl.AmbientColor );
	l.DiffuseColor.setColorf ( dl.DiffuseColor );
	l.SpecularColor.setColorf ( dl.SpecularColor );

	switch ( dl.Type )
	{
		case video::ELT_DIRECTIONAL:
			l.pos.x = -dl.Direction.X;
			l.pos.y = -dl.Direction.Y;
			l.pos.z = -dl.Direction.Z;
			l.pos.w = 1.f;
			break;
		case ELT_POINT:
		case ELT_SPOT:
			LightSpace.Flags |= POINTLIGHT;
			l.pos.x = dl.Position.X;
			l.pos.y = dl.Position.Y;
			l.pos.z = dl.Position.Z;
			l.pos.w = 1.f;
/*
			l.radius = (1.f / dl.Attenuation.Y) * (1.f / dl.Attenuation.Y);
			l.constantAttenuation = dl.Attenuation.X;
			l.linearAttenuation = dl.Attenuation.Y;
			l.quadraticAttenuation = dl.Attenuation.Z;
*/
			l.radius = dl.Radius * dl.Radius;
			l.constantAttenuation = dl.Attenuation.X;
			l.linearAttenuation = 1.f / dl.Radius;
			l.quadraticAttenuation = dl.Attenuation.Z;

			break;
	}

	LightSpace.Light.push_back ( l );
	return LightSpace.Light.size() - 1;
}

//! Turns a dynamic light on or off
void CBurningVideoDriver::turnLightOn(s32 lightIndex, bool turnOn)
{
	if(lightIndex > -1 && lightIndex < (s32)LightSpace.Light.size())
	LightSpace.Light[lightIndex].LightIsOn = turnOn;
}

//! deletes all dynamic lights there are
void CBurningVideoDriver::deleteAllDynamicLights()
{
	LightSpace.reset ();
	CNullDriver::deleteAllDynamicLights();

}

//! returns the maximal amount of dynamic lights the device can handle
u32 CBurningVideoDriver::getMaximalDynamicLightAmount() const
{
	return 8;
}


//! sets a material
void CBurningVideoDriver::setMaterial(const SMaterial& material)
{
	Material.org = material;

#ifdef SOFTWARE_DRIVER_2_TEXTURE_TRANSFORM
	for (u32 i = 0; i < 2; ++i)
	{
		setTransform((E_TRANSFORMATION_STATE) (ETS_TEXTURE_0 + i),
				material.getTextureMatrix(i));
	}
#endif

#ifdef SOFTWARE_DRIVER_2_LIGHTING
	Material.AmbientColor.setR8G8B8 ( Material.org.AmbientColor.color );
	Material.DiffuseColor.setR8G8B8 ( Material.org.DiffuseColor.color );
	Material.EmissiveColor.setR8G8B8 ( Material.org.EmissiveColor.color );
	Material.SpecularColor.setR8G8B8 ( Material.org.SpecularColor.color );

	core::setbit_cond ( LightSpace.Flags, Material.org.Shininess != 0.f, SPECULAR );
	core::setbit_cond ( LightSpace.Flags, Material.org.FogEnable, FOG );
	core::setbit_cond ( LightSpace.Flags, Material.org.NormalizeNormals, NORMALIZE );
#endif

	setCurrentShader();
}



#ifdef SOFTWARE_DRIVER_2_LIGHTING

//! Sets the fog mode.
void CBurningVideoDriver::setFog(SColor color, bool linearFog, f32 start,
	f32 end, f32 density, bool pixelFog, bool rangeFog)
{
	CNullDriver::setFog(color, linearFog, start, end, density, pixelFog, rangeFog);
	LightSpace.FogColor.setA8R8G8B8 ( color.color );
}

/*!
	Camera Position in World Space
*/
void CBurningVideoDriver::getCameraPosWorldSpace ()
{
	Transformation[ETS_VIEW_INVERSE] = Transformation[ ETS_VIEW ];
	Transformation[ETS_VIEW_INVERSE].makeInverse ();
	TransformationFlag[ETS_VIEW_INVERSE] = 0;

	const f32 *M = Transformation[ETS_VIEW_INVERSE].pointer ();

	/*	The  viewpoint is at (0., 0., 0.) in eye space.
		Turning this into a vector [0 0 0 1] and multiply it by
		the inverse of the view matrix, the resulting vector is the
		object space location of the camera.
	*/

	LightSpace.campos.x = M[12];
	LightSpace.campos.y = M[13];
	LightSpace.campos.z = M[14];
	LightSpace.campos.w = 1.f;
}

/*!
	applies lighting model
*/
void CBurningVideoDriver::lightVertex ( s4DVertex *dest, u32 vertexargb )
{
	sVec3 dColor;

	dColor = LightSpace.Global_AmbientLight;
	dColor.add ( Material.EmissiveColor );

	if ( Lights.size () == 0 )
	{
		dColor.saturate( dest->Color[0], vertexargb);
		return;
	}

	sVec3 ambient;
	sVec3 diffuse;
	sVec3 specular;


	// the universe started in darkness..
	ambient.set ( 0.f, 0.f, 0.f );
	diffuse.set ( 0.f, 0.f, 0.f );
	specular.set ( 0.f, 0.f, 0.f );


	u32 i;
	f32 dot;
	f32 len;
	f32 attenuation;
	sVec4 vp;			// unit vector vertex to light
	sVec4 lightHalf;	// blinn-phong reflection

	for ( i = 0; i!= LightSpace.Light.size (); ++i )
	{
		const SBurningShaderLight &light = LightSpace.Light[i];

		// accumulate ambient
		ambient.add ( light.AmbientColor );

		switch ( light.Type )
		{
			case video::ELT_SPOT:
			case video::ELT_POINT:
				// surface to light
				vp.x = light.pos.x - LightSpace.vertex.x;
				vp.y = light.pos.y - LightSpace.vertex.y;
				vp.z = light.pos.z - LightSpace.vertex.z;

				len = vp.get_length_xyz_square();
				if ( light.radius < len )
					continue;

				len = core::squareroot ( len );

				attenuation = light.constantAttenuation + ( 1.f - ( len * light.linearAttenuation ) );

				// build diffuse reflection

				//angle between normal and light vector
				vp.mulReciprocal ( len );
				dot = LightSpace.normal.dot_xyz ( vp );
				if ( dot < 0.f )
					continue;

				// diffuse component
				diffuse.mulAdd ( light.DiffuseColor, dot * attenuation );

				if ( !(LightSpace.Flags & SPECULAR) )
					continue;

				// build specular
				// surface to view
				lightHalf.x = LightSpace.campos.x - LightSpace.vertex.x;
				lightHalf.y = LightSpace.campos.y - LightSpace.vertex.y;
				lightHalf.z = LightSpace.campos.z - LightSpace.vertex.z;
				lightHalf.normalize_xyz();
				lightHalf += vp;
				lightHalf.normalize_xyz();

				// specular
				dot = LightSpace.normal.dot_xyz ( lightHalf );
				if ( dot < 0.f )
					continue;

				//specular += light.SpecularColor * ( powf ( Material.org.Shininess ,dot ) * attenuation );
				specular.mulAdd ( light.SpecularColor, dot * attenuation );
				break;

			case video::ELT_DIRECTIONAL:

				//angle between normal and light vector
				dot = LightSpace.normal.dot_xyz ( light.pos );
				if ( dot < 0.f )
					continue;

				// diffuse component
				diffuse.mulAdd ( light.DiffuseColor, dot );
				break;
		}

	}

	// sum up lights
	dColor.mulAdd (ambient, Material.AmbientColor );
	dColor.mulAdd (diffuse, Material.DiffuseColor);
	dColor.mulAdd (specular, Material.SpecularColor);

	dColor.saturate ( dest->Color[0], vertexargb );
}

#endif


//! draws an 2d image, using a color (if color is other then Color(255,255,255,255)) and the alpha channel of the texture if wanted.
void CBurningVideoDriver::draw2DImage(const video::ITexture* texture, const core::position2d<s32>& destPos,
					 const core::rect<s32>& sourceRect,
					 const core::rect<s32>* clipRect, SColor color,
					 bool useAlphaChannelOfTexture)
{
	if (texture)
	{
		if (texture->getDriverType() != EDT_BURNINGSVIDEO)
		{
			os::Printer::log("Fatal Error: Tried to copy from a surface not owned by this driver.", ELL_ERROR);
			return;
		}

		if (useAlphaChannelOfTexture)
			((CSoftwareTexture2*)texture)->getImage()->copyToWithAlpha(
				BackBuffer, destPos, sourceRect, color, clipRect);
		else
			((CSoftwareTexture2*)texture)->getImage()->copyTo(
				BackBuffer, destPos, sourceRect, clipRect);
	}
}



//! Draws a 2d line.
void CBurningVideoDriver::draw2DLine(const core::position2d<s32>& start,
					const core::position2d<s32>& end,
					SColor color)
{
	((CImage*)BackBuffer)->drawLine(start, end, color );
}

//! Draws a pixel
void CBurningVideoDriver::drawPixel(u32 x, u32 y, const SColor & color)
{
	((CImage*)BackBuffer)->setPixel(x, y, color, true);
} 

//! draw an 2d rectangle
void CBurningVideoDriver::draw2DRectangle(SColor color, const core::rect<s32>& pos,
									 const core::rect<s32>* clip)
{
	if (clip)
	{
		core::rect<s32> p(pos);

		p.clipAgainst(*clip);

		if(!p.isValid())
			return;

		BackBuffer->drawRectangle(p, color);
	}
	else
	{
		if(!pos.isValid())
			return;

		BackBuffer->drawRectangle(pos, color);
	}
}


//! Only used by the internal engine. Used to notify the driver that
//! the window was resized.
void CBurningVideoDriver::OnResize(const core::dimension2d<u32>& size)
{
	// make sure width and height are multiples of 2
	core::dimension2d<u32> realSize(size);

	if (realSize.Width % 2)
		realSize.Width += 1;

	if (realSize.Height % 2)
		realSize.Height += 1;

	if (ScreenSize != realSize)
	{
		if (ViewPort.getWidth() == (s32)ScreenSize.Width &&
			ViewPort.getHeight() == (s32)ScreenSize.Height)
		{
			ViewPort.UpperLeftCorner.X = 0;
			ViewPort.UpperLeftCorner.Y = 0;
			ViewPort.LowerRightCorner.X = realSize.Width;
			ViewPort.LowerRightCorner.X = realSize.Height;
		}

		ScreenSize = realSize;

		bool resetRT = (RenderTargetSurface == BackBuffer);

		if (BackBuffer)
			BackBuffer->drop();
		BackBuffer = new CImage(BURNINGSHADER_COLOR_FORMAT, realSize);

		if (resetRT)
			setRenderTarget(BackBuffer);
	}
}


//! returns the current render target size
const core::dimension2d<u32>& CBurningVideoDriver::getCurrentRenderTargetSize() const
{
	return RenderTargetSize;
}


//!Draws an 2d rectangle with a gradient.
void CBurningVideoDriver::draw2DRectangle(const core::rect<s32>& position,
	SColor colorLeftUp, SColor colorRightUp, SColor colorLeftDown, SColor colorRightDown,
	const core::rect<s32>* clip)
{
#ifdef SOFTWARE_DRIVER_2_USE_VERTEX_COLOR

	core::rect<s32> pos = position;

	if (clip)
		pos.clipAgainst(*clip);

	if (!pos.isValid())
		return;

	const core::dimension2d<s32> renderTargetSize ( ViewPort.getSize() );

	const s32 xPlus = -(renderTargetSize.Width>>1);
	const f32 xFact = 1.0f / (renderTargetSize.Width>>1);

	const s32 yPlus = renderTargetSize.Height-(renderTargetSize.Height>>1);
	const f32 yFact = 1.0f / (renderTargetSize.Height>>1);

	// fill VertexCache direct
	s4DVertex *v;

	VertexCache.vertexCount = 4;

	VertexCache.info[0].index = 0;
	VertexCache.info[1].index = 1;
	VertexCache.info[2].index = 2;
	VertexCache.info[3].index = 3;

	v = &VertexCache.mem.data [ 0 ];

	v[0].Pos.set ( (f32)(pos.UpperLeftCorner.X+xPlus) * xFact, (f32)(yPlus-pos.UpperLeftCorner.Y) * yFact, 0.f, 1.f );
	v[0].Color[0].setA8R8G8B8 ( colorLeftUp.color );

	v[2].Pos.set ( (f32)(pos.LowerRightCorner.X+xPlus) * xFact, (f32)(yPlus- pos.UpperLeftCorner.Y) * yFact, 0.f, 1.f );
	v[2].Color[0].setA8R8G8B8 ( colorRightUp.color );

	v[4].Pos.set ( (f32)(pos.LowerRightCorner.X+xPlus) * xFact, (f32)(yPlus-pos.LowerRightCorner.Y) * yFact, 0.f ,1.f );
	v[4].Color[0].setA8R8G8B8 ( colorRightDown.color );

	v[6].Pos.set ( (f32)(pos.UpperLeftCorner.X+xPlus) * xFact, (f32)(yPlus-pos.LowerRightCorner.Y) * yFact, 0.f, 1.f );
	v[6].Color[0].setA8R8G8B8 ( colorLeftDown.color );

	s32 i;
	u32 g;

	for ( i = 0; i!= 8; i += 2 )
	{
		v[i + 0].flag = clipToFrustumTest ( v + i );
		v[i + 1].flag = 0;
		if ( (v[i].flag & VERTEX4D_INSIDE ) == VERTEX4D_INSIDE )
		{
			ndc_2_dc_and_project ( v + i + 1, v + i, 2 );
		}
	}


	IBurningShader * render;

	render = BurningShader [ ETR_GOURAUD_ALPHA_NOZ ];
	render->setRenderTarget(RenderTargetSurface, ViewPort);

	static const s16 indexList[6] = {0,1,2,0,2,3};

	s4DVertex * face[3];

	for ( i = 0; i!= 6; i += 3 )
	{
		face[0] = VertexCache_getVertex ( indexList [ i + 0 ] );
		face[1] = VertexCache_getVertex ( indexList [ i + 1 ] );
		face[2] = VertexCache_getVertex ( indexList [ i + 2 ] );

		// test clipping
		u32 test = face[0]->flag & face[1]->flag & face[2]->flag & VERTEX4D_INSIDE;

		if ( test == VERTEX4D_INSIDE )
		{
			render->drawTriangle ( face[0] + 1, face[1] + 1, face[2] + 1 );
			continue;
		}
		// Todo: all vertices are clipped in 2d..
		// is this true ?
		u32 vOut = 6;
		memcpy ( CurrentOut.data + 0, face[0], sizeof ( s4DVertex ) * 2 );
		memcpy ( CurrentOut.data + 2, face[1], sizeof ( s4DVertex ) * 2 );
		memcpy ( CurrentOut.data + 4, face[2], sizeof ( s4DVertex ) * 2 );

		vOut = clipToFrustum ( CurrentOut.data, Temp.data, 3 );
		if ( vOut < 3 )
			continue;

		vOut <<= 1;
		// to DC Space, project homogenous vertex
		ndc_2_dc_and_project ( CurrentOut.data + 1, CurrentOut.data, vOut );

		// re-tesselate ( triangle-fan, 0-1-2,0-2-3.. )
		for ( g = 0; g <= vOut - 6; g += 2 )
		{
			// rasterize
			render->drawTriangle ( CurrentOut.data + 1, &CurrentOut.data[g + 3], &CurrentOut.data[g + 5] );
		}

	}
#else
	draw2DRectangle ( colorLeftUp, position, clip );
#endif
}


//! Draws a 3d line.
void CBurningVideoDriver::draw3DLine(const core::vector3df& start,
	const core::vector3df& end, SColor color)
{
	Transformation [ ETS_CURRENT].transformVect ( &CurrentOut.data[0].Pos.x, start );
	Transformation [ ETS_CURRENT].transformVect ( &CurrentOut.data[2].Pos.x, end );

	u32 g;
	u32 vOut;

	// no clipping flags
	for ( g = 0; g != CurrentOut.ElementSize; ++g )
	{
		CurrentOut.data[g].flag = 0;
		Temp.data[g].flag = 0;
	}

	// vertices count per line
	vOut = clipToFrustum ( CurrentOut.data, Temp.data, 2 );
	if ( vOut < 2 )
		return;

	vOut <<= 1;

	IBurningShader * line;
	line = BurningShader [ ETR_TEXTURE_GOURAUD_WIRE ];
	line->setRenderTarget(RenderTargetSurface, ViewPort);

	// to DC Space, project homogenous vertex
	ndc_2_dc_and_project ( CurrentOut.data + 1, CurrentOut.data, vOut );

	// unproject vertex color
#ifdef SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
	for ( g = 0; g != vOut; g+= 2 )
	{
		CurrentOut.data[ g + 1].Color[0].setA8R8G8B8 ( color.color );
	}
#endif


	for ( g = 0; g <= vOut - 4; g += 2 )
	{
		// rasterize
		line->drawLine ( CurrentOut.data + 1, CurrentOut.data + g + 3 );
	}
}


//! \return Returns the name of the video driver. Example: In case of the DirectX8
//! driver, it would return "Direct3D8.1".
const wchar_t* CBurningVideoDriver::getName() const
{
#ifdef BURNINGVIDEO_RENDERER_BEAUTIFUL
	return L"Burning's Video 0.44 beautiful";
#elif defined ( BURNINGVIDEO_RENDERER_ULTRA_FAST )
	return L"Burning's Video 0.44 ultra fast";
#elif defined ( BURNINGVIDEO_RENDERER_FAST )
	return L"Burning's Video 0.44 fast";
#else
	return L"Burning's Video 0.44";
#endif
}

//! Returns the graphics card vendor name.
core::stringc CBurningVideoDriver::getVendorInfo()
{
	return "Burning's Video: Ing. Thomas Alten (c) 2006-2009";
}


//! Returns type of video driver
E_DRIVER_TYPE CBurningVideoDriver::getDriverType() const
{
	return EDT_BURNINGSVIDEO;
}


//! returns color format
ECOLOR_FORMAT CBurningVideoDriver::getColorFormat() const
{
	return BURNINGSHADER_COLOR_FORMAT;
}


//! Returns the transformation set by setTransform
const core::matrix4& CBurningVideoDriver::getTransform(E_TRANSFORMATION_STATE state) const
{
	return Transformation[state];
}


//! Creates a render target texture.
ITexture* CBurningVideoDriver::addRenderTargetTexture(const core::dimension2d<u32>& size,
		const core::string<c16>& name)
{
	CImage* img = new CImage(BURNINGSHADER_COLOR_FORMAT, size);
	ITexture* tex = new CSoftwareTexture2(img, name, CSoftwareTexture2::IS_RENDERTARGET );
	img->drop();
	addTexture(tex);
	tex->drop();
	return tex;
}


//! Clears the DepthBuffer.
void CBurningVideoDriver::clearZBuffer()
{
	if (DepthBuffer)
		DepthBuffer->clear();
}


//! Returns an image created from the last rendered frame.
IImage* CBurningVideoDriver::createScreenShot()
{
	if (BackBuffer)
		return new CImage(BackBuffer->getColorFormat(), BackBuffer);
	else
		return 0;

}


//! returns a device dependent texture from a software surface (IImage)
//! THIS METHOD HAS TO BE OVERRIDDEN BY DERIVED DRIVERS WITH OWN TEXTURES
ITexture* CBurningVideoDriver::createDeviceDependentTexture(IImage* surface, const core::string<c16>& name)
{
	return new CSoftwareTexture2(
		surface, name, 
		(getTextureCreationFlag(ETCF_CREATE_MIP_MAPS) ? CSoftwareTexture2::GEN_MIPMAP : 0 ) |
		(getTextureCreationFlag(ETCF_ALLOW_NON_POWER_2) ? 0 : CSoftwareTexture2::NP2_SIZE )
	);

}


//! Returns the maximum amount of primitives (mostly vertices) which
//! the device is able to render with one drawIndexedTriangleList
//! call.
u32 CBurningVideoDriver::getMaximalPrimitiveCount() const
{
	return 0xFFFFFFFF;
}


//! Draws a shadow volume into the stencil buffer. To draw a stencil shadow, do
//! this: First, draw all geometry. Then use this method, to draw the shadow
//! volume. Then, use IVideoDriver::drawStencilShadow() to visualize the shadow.
void CBurningVideoDriver::drawStencilShadowVolume(const core::vector3df* triangles, s32 count, bool zfail)
{
/*
	if (!StencilBuffer || !count)
		return;

	setRenderStatesStencilShadowMode(zfail);

	if (!zfail)
	{
		// ZPASS Method

		// Draw front-side of shadow volume in stencil/z only
		pID3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW );
		pID3DDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);
		pID3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, count / 3, triangles, sizeof(core::vector3df));

		// Now reverse cull order so front sides of shadow volume are written.
		pID3DDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CW );
		pID3DDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_DECRSAT);
		pID3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, count / 3, triangles, sizeof(core::vector3df));
	}
	else
	{
		// ZFAIL Method

		// Draw front-side of shadow volume in stencil/z only
		pID3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW );
		pID3DDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCRSAT );
		pID3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, count / 3, triangles, sizeof(core::vector3df));

		// Now reverse cull order so front sides of shadow volume are written.
		pID3DDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW );
		pID3DDevice->SetRenderState( D3DRS_STENCILZFAIL,  D3DSTENCILOP_DECRSAT );
		pID3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, count / 3, triangles, sizeof(core::vector3df));
	}
*/
}



//! Fills the stencil shadow with color. After the shadow volume has been drawn
//! into the stencil buffer using IVideoDriver::drawStencilShadowVolume(), use this
//! to draw the color of the shadow.
void CBurningVideoDriver::drawStencilShadow(bool clearStencilBuffer, video::SColor leftUpEdge,
			video::SColor rightUpEdge, video::SColor leftDownEdge, video::SColor rightDownEdge)
{
/*
	if (!StencilBuffer)
		return;

	S3DVertex vtx[4];
	vtx[0] = S3DVertex(1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, leftUpEdge, 0.0f, 0.0f);
	vtx[1] = S3DVertex(1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, rightUpEdge, 0.0f, 1.0f);
	vtx[2] = S3DVertex(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, leftDownEdge, 1.0f, 0.0f);
	vtx[3] = S3DVertex(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, rightDownEdge, 1.0f, 1.0f);

	s16 indices[6] = {0,1,2,1,3,2};

	setRenderStatesStencilFillMode(
		leftUpEdge.getAlpha() < 255 ||
		rightUpEdge.getAlpha() < 255 ||
		leftDownEdge.getAlpha() < 255 ||
		rightDownEdge.getAlpha() < 255);

	setTexture(0,0);

	setVertexShader(EVT_STANDARD);

	pID3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, &indices[0],
		D3DFMT_INDEX16, &vtx[0], sizeof(S3DVertex));

	if (clearStencilBuffer)
		pID3DDevice->Clear( 0, NULL, D3DCLEAR_STENCIL,0, 1.0, 0);
*/
}


} // end namespace video
} // end namespace irr

#endif // _IRR_COMPILE_WITH_BURNINGSVIDEO_

namespace irr
{
namespace video
{

//! creates a video driver
IVideoDriver* createSoftwareDriver2(const core::dimension2d<u32>& windowSize, bool fullscreen, io::IFileSystem* io, video::IImagePresenter* presenter)
{
	#ifdef _IRR_COMPILE_WITH_BURNINGSVIDEO_
	return new CBurningVideoDriver(windowSize, fullscreen, io, presenter);
	#else
	return 0;
	#endif // _IRR_COMPILE_WITH_BURNINGSVIDEO_
}



} // end namespace video
} // end namespace irr
