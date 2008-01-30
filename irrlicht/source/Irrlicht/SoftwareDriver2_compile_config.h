// Copyright (C) 2002-2007 Nikolaus Gebhardt / Thomas Alten
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __S_VIDEO_2_SOFTWARE_COMPILE_CONFIG_H_INCLUDED__
#define __S_VIDEO_2_SOFTWARE_COMPILE_CONFIG_H_INCLUDED__

#include "IrrCompileConfig.h"


// Generic Render Flags for burning's video rasterizer
// defined now in irrlicht compile config


#ifdef BURNINGVIDEO_RENDERER_BEAUTIFUL
	#define SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT
	#define SOFTWARE_DRIVER_2_SUBTEXEL
	#define SOFTWARE_DRIVER_2_BILINEAR
	#define SOFTWARE_DRIVER_2_LIGHTING
	#define SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
	#define SOFTWARE_DRIVER_2_32BIT
	#define SOFTWARE_DRIVER_2_MIPMAPPING
	#define SOFTWARE_DRIVER_2_USE_WBUFFER
	#define SOFTWARE_DRIVER_2_TEXTURE_TRANSFORM
#endif

#ifdef BURNINGVIDEO_RENDERER_FAST
	#define SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT
	#define SOFTWARE_DRIVER_2_SUBTEXEL
	//#define SOFTWARE_DRIVER_2_BILINEAR
	//#define SOFTWARE_DRIVER_2_LIGHTING
	//#define SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
	#define SOFTWARE_DRIVER_2_32BIT
	#define SOFTWARE_DRIVER_2_MIPMAPPING
	#define SOFTWARE_DRIVER_2_USE_WBUFFER
#endif

#ifdef BURNINGVIDEO_RENDERER_ULTRA_FAST
	#define BURNINGVIDEO_RENDERER_FAST

	//#define SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT
	#define SOFTWARE_DRIVER_2_SUBTEXEL
	//#define SOFTWARE_DRIVER_2_BILINEAR
	//#define SOFTWARE_DRIVER_2_LIGHTING
	//#define SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
	//#define SOFTWARE_DRIVER_2_32BIT
	#define SOFTWARE_DRIVER_2_MIPMAPPING
	//#define SOFTWARE_DRIVER_2_USE_WBUFFER
#endif

// Derivate flags

// texture format
#ifdef SOFTWARE_DRIVER_2_32BIT
	#define	ECF_SOFTWARE2	ECF_A8R8G8B8
#else
	#define	ECF_SOFTWARE2	ECF_A1R5G5B5
#endif

// mip mapping
#ifdef SOFTWARE_DRIVER_2_MIPMAPPING
	#ifdef BURNINGVIDEO_RENDERER_BEAUTIFUL
		#define SOFTWARE_DRIVER_2_MIPMAPPING_MAX		8
		#define SOFTWARE_DRIVER_2_MIPMAPPING_LOD_BIAS	-1
	#else
		#define SOFTWARE_DRIVER_2_MIPMAPPING_MAX		8
		#define SOFTWARE_DRIVER_2_MIPMAPPING_LOD_BIAS	-1
	#endif
#else
	#define SOFTWARE_DRIVER_2_MIPMAPPING_MAX		1
	#define SOFTWARE_DRIVER_2_MIPMAPPING_LOD_BIAS	0
#endif



#ifndef REALINLINE
	#ifdef _MSC_VER
		#define REALINLINE __forceinline
	#else
		#define REALINLINE inline
	#endif
#endif

#endif
