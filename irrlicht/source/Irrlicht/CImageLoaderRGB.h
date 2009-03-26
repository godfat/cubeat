// Copyright (C) 2009 Gary Conway
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h


/*
	Author:	Gary Conway (Viper) - co-author of the ZIP file format, Feb 1989,
						 see the story at http://www.idcnet.us/ziphistory.html
	Website:	http://idcnet.us
	Email:		codeslinger@vipergc.com
	Created:	March 1, 2009
	Version:	1.0
	Updated:
*/

#ifndef __C_IMAGE_LOADER_RGB_H_INCLUDED__
#define __C_IMAGE_LOADER_RGB_H_INCLUDED__

// define _IRR_RGB_FILE_INVERTED_IMAGE_ to preserve the inverted format of the RGB file
// commenting this out will invert the inverted image,resulting in the image being upright
#define _IRR_RGB_FILE_INVERTED_IMAGE_

#include "IrrCompileConfig.h"

#ifdef _IRR_COMPILE_WITH_RGB_LOADER_

#include "IImageLoader.h"

namespace irr
{
namespace video
{

// byte-align structures
#if defined(_MSC_VER) ||  defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#else
#	error compiler not supported
#endif

	// the RGB image file header structure

	struct SRGBHeader
	{
		u16	Magic;							// IRIS image file magic number
		u8  Storage;						// Storage format
		u8  BPC;							// Number of bytes per pixel channel
		u16 Dimension;						// Number of dimensions
		u16 Xsize;							// X size in pixels
		u16 Ysize;							// Y size in pixels
		u16 Zsize;							// Z size in pixels
		u32 Pixmin;							// Minimum pixel value
		u32 Pixmax;							// Maximum pixel value
		u32 Dummy1;							// ignored
		char Imagename[80];					// Image name
		u32 Colormap;						// Colormap ID
//		char Dummy2[404];					// Ignored
	} PACK_STRUCT;

// Default alignment
#if defined(_MSC_VER) ||  defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
#	pragma pack( pop, packing )
#endif

#undef PACK_STRUCT

	// this structure holds context specific data about the file being loaded.

	typedef struct _RGBdata
	{
		u8 *tmp,
		   *tmpR,
		   *tmpG,
		   *tmpB,
		   *tmpA;


		u32 *StartTable;	// compressed data table, holds file offsets
		u32 *LengthTable;	// length for the above data, hold lengths for above
		u32 TableLen;		// len of above tables

		SRGBHeader Header;	// define the .rgb file header
		u32 ImageSize;
		u8 *rgbData;

	public:
		_RGBdata() : tmp(0), tmpR(0), tmpG(0), tmpB(0), tmpA(0), 
			StartTable(0), LengthTable(0), TableLen(0), ImageSize(0), rgbData(0)
		{
		}

		~_RGBdata()
		{
			delete [] tmp;
			delete [] tmpR;
			delete [] tmpG;
			delete [] tmpB;
			delete [] tmpA;
			delete [] StartTable;
			delete [] LengthTable;
			delete [] rgbData;
		}

		bool allocateTemps()
		{
			tmp = tmpR = tmpG = tmpB = tmpA = 0;
			tmp = new u8 [Header.Xsize * 256 * Header.BPC];
			if (!tmp)
				return false;

			if (Header.Zsize >= 1)
			{
				if ( !(tmpR = new u8 [Header.Xsize * Header.BPC]) )
					return false;
			}
			if (Header.Zsize >= 2)
			{
				if ( !(tmpG = new u8 [Header.Xsize * Header.BPC]) )
					return false;
			}
			if (Header.Zsize >= 3)
			{
				if ( !(tmpB = new u8 [Header.Xsize * Header.BPC]) )
					return false;
			}
			if (Header.Zsize >= 4)
			{
				if ( !(tmpA = new u8 [Header.Xsize * Header.BPC]) )
					return false;
			}
			return true;
		}
	} rgbStruct;


//! Surface Loader for Silicon Graphics RGB files
class CImageLoaderRGB : public IImageLoader
{
public:

	//! constructor
	CImageLoaderRGB();

	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".tga")
	virtual bool isALoadableFileExtension(const irr::core::stringc &fileName) const;

	//! returns true if the file maybe is able to be loaded by this class
	virtual bool isALoadableFileFormat(io::IReadFile* file) const;

	//! creates a surface from the file
	virtual IImage* loadImage(io::IReadFile* file) const;

private:

	bool readHeader(io::IReadFile* file, rgbStruct& rgb) const;
	void readRGBrow(u8 *buf, int y, int z, io::IReadFile* file, rgbStruct& rgb) const;
	void processFile(io::IReadFile *file, rgbStruct& rgb) const;
	bool checkFormat(io::IReadFile *file, rgbStruct& rgb) const;
	bool readOffsetTables(io::IReadFile* file, rgbStruct& rgb) const;
	void converttoARGB(u32* in, const u32 size) const;
};

} // end namespace video
} // end namespace irr

#endif // _IRR_COMPILE_WITH_RGB_LOADER_
#endif // __C_IMAGE_LOADER_RGB_H_INCLUDED__
