// Copyright (C) 2007 Christian Stehno
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h" 

#ifdef _IRR_COMPILE_WITH_STL_LOADER_

#include "CSTLMeshFileLoader.h"
#include "SMesh.h"
#include "SMeshBuffer.h"
#include "SAnimatedMesh.h"
#include "IReadFile.h"
#include "fast_atof.h"
#include "coreutil.h"
#include "os.h"

namespace irr
{
namespace scene
{


//! returns true if the file maybe is able to be loaded by this class
//! based on the file extension (e.g. ".bsp")
bool CSTLMeshFileLoader::isALoadableFileExtension(const c8* filename) const
{
	return strstr(filename, ".stl")!=0;
}



//! creates/loads an animated mesh from the file.
//! \return Pointer to the created mesh. Returns 0 if loading failed.
//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
//! See IReferenceCounted::drop() for more information.
IAnimatedMesh* CSTLMeshFileLoader::createMesh(io::IReadFile* file)
{
	const long filesize = file->getSize();
	if (filesize < 6) // we need a header
		return 0;

	const u32 WORD_BUFFER_LENGTH = 512;

	SMesh* mesh = new SMesh();
	mesh->addMeshBuffer( new SMeshBuffer() );

	core::vector3df vertex[3];
	core::vector3df normal;

	c8 buffer[WORD_BUFFER_LENGTH];

	bool binary = false;
	file->read(buffer, 5);
	if (strncmp("solid", buffer, 5))
		binary = true;
	// read/skip header
	u32 binFaceCount = 0;
	if (binary)
	{
		file->seek(80);
		file->read(&binFaceCount, 4);
#ifdef __BIG_ENDIAN__
		binFaceCount = os::Byteswap::byteswap(binFaceCount);
#endif
	}
	else
		goNextLine(file);

	u16 attrib=0;
	core::stringc token;
	token.reserve(32);

	while (file->getPos() < filesize)
	{
		if (!binary)
		{
			if (getNextToken(file, token) != "facet")
			{
				if (token=="endsolid")
					break;
				mesh->drop();
				return 0;
			}
			if (getNextToken(file, token) != "normal")
			{
				mesh->drop();
				return 0;
			}
		}
		getNextVector(file, normal, binary);
		if (!binary)
		{
			if (getNextToken(file, token) != "outer")
			{
				mesh->drop();
				return 0;
			}
			if (getNextToken(file, token) != "loop")
			{
				mesh->drop();
				return 0;
			}
		}
		for (u32 i=0; i<3; ++i)
		{
			if (!binary)
			{
				if (getNextToken(file, token) != "vertex")
				{
					mesh->drop();
					return 0;
				}
			}
			getNextVector(file, vertex[i], binary);
		}
		if (!binary)
		{
			if (getNextToken(file, token) != "endloop")
			{
				mesh->drop();
				return 0;
			}
			if (getNextToken(file, token) != "endfacet")
			{
				mesh->drop();
				return 0;
			}
		}
		else
		{
			file->read(&attrib, 2);
#ifdef __BIG_ENDIAN__
			attrib = os::Byteswap::byteswap(attrib);
#endif
		}

		SMeshBuffer* mb = reinterpret_cast<SMeshBuffer*>(mesh->getMeshBuffer(mesh->getMeshBufferCount()-1));
		u32 vCount = mb->getVertexCount();
		video::SColor color(0xffffffff);
		if (attrib & 0x8000)
			color = video::A1R5G5B5toA8R8G8B8(attrib);
		mb->Vertices.push_back(video::S3DVertex(vertex[0],normal,color, core::vector2df()));
		mb->Vertices.push_back(video::S3DVertex(vertex[1],normal,color, core::vector2df()));
		mb->Vertices.push_back(video::S3DVertex(vertex[2],normal,color, core::vector2df()));
		mb->Indices.push_back(vCount);
		mb->Indices.push_back(vCount+1);
		mb->Indices.push_back(vCount+2);
	}	// end while (file->getPos() < filesize)

	// Create the Animated mesh if there's anything in the mesh
	SAnimatedMesh* pAM = 0;
	if ( 0 != mesh->getMeshBufferCount() )
	{
		mesh->recalculateBoundingBox();
		pAM = new SAnimatedMesh();
		pAM->Type = EAMT_OBJ;
		pAM->addMesh(mesh);
		pAM->recalculateBoundingBox();
	}

	mesh->drop();

	return pAM;
}


//! Read RGB color
const c8* CSTLMeshFileLoader::readColor(const c8* bufPtr, video::SColor& color, const c8* const pBufEnd) const
{
	const u32 COLOR_BUFFER_LENGTH = 16;
	c8 colStr[COLOR_BUFFER_LENGTH];

	color.setAlpha(255);
	color.setRed((s32)(core::fast_atof(colStr) * 255.0f));
	color.setGreen((s32)(core::fast_atof(colStr) * 255.0f));
	color.setBlue((s32)(core::fast_atof(colStr) * 255.0f));
	return bufPtr;
}


//! Read 3d vector of floats
void CSTLMeshFileLoader::getNextVector(io::IReadFile* file, core::vector3df& vec, bool binary) const
{
	if (binary)
	{
		file->read(&vec.X, 4);
		file->read(&vec.Y, 4);
		file->read(&vec.Z, 4);
#ifdef __BIG_ENDIAN__
		vec.X = os::Byteswap::byteswap(vec.X);
		vec.Y = os::Byteswap::byteswap(vec.Y);
		vec.Z = os::Byteswap::byteswap(vec.Z);
#endif
	}
	else
	{
		goNextWord(file);
		core::stringc tmp;

		getNextToken(file, tmp);
		core::fast_atof_move(tmp.c_str(), vec.X);
		getNextToken(file, tmp);
		core::fast_atof_move(tmp.c_str(), vec.Y);
		getNextToken(file, tmp);
		core::fast_atof_move(tmp.c_str(), vec.Z);
	}
}


//! Read next word
const core::stringc& CSTLMeshFileLoader::getNextToken(io::IReadFile* file, core::stringc& token) const
{
	goNextWord(file);
	u8 c;
	token = "";
	while(file->getPos() != file->getSize())
	{
		file->read(&c, 1);
		// found it, so leave
		if (core::isspace(c))
			break;
		token.append(c);
	}
	return token;
}


//! skip to next word
void CSTLMeshFileLoader::goNextWord(io::IReadFile* file) const
{
	u8 c;
	while(file->getPos() != file->getSize())
	{
		file->read(&c, 1);
		// found it, so leave
		if (!core::isspace(c))
		{
			file->seek(-1, true);
			break;
		}
	}
}


//! Read until line break is reached and stop at the next non-space character
void CSTLMeshFileLoader::goNextLine(io::IReadFile* file) const
{
	u8 c;
	// look for newline characters
	while(file->getPos() != file->getSize())
	{
		file->read(&c, 1);
		// found it, so leave
		if (c=='\n' || c=='\r')
			break;
	}
}


} // end namespace scene
} // end namespace irr


#endif // _IRR_COMPILE_WITH_STL_LOADER_

