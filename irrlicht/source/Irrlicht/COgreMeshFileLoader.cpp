// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
// orginally written by Christian Stehno, modified by Nikolaus Gebhardt

#include "IrrCompileConfig.h" 
#ifdef _IRR_COMPILE_WITH_OGRE_LOADER_

#include "COgreMeshFileLoader.h"
#include "os.h"
#include "SMeshBuffer.h"
#include "SAnimatedMesh.h"
#include "IReadFile.h"
#include "fast_atof.h"
#include "coreutil.h"

#ifdef _DEBUG
#define IRR_OGRE_LOADER_DEBUG
#endif

namespace irr
{
namespace scene
{

	enum OGRE_CHUNKS
	{
		// Main Chunks
		COGRE_HEADER= 0x1000,
		COGRE_MESH= 0x3000,

		// sub chunks of COGRE_MESH
		COGRE_SUBMESH= 0x4000,
		COGRE_GEOMETRY= 0x5000,
		COGRE_SKELETON_LINK= 0x6000,
		COGRE_BONE_ASSIGNMENT= 0x7000,
		COGRE_MESH_LOD= 0x8000,
		COGRE_MESH_BOUNDS= 0x9000,
		COGRE_MESH_SUBMESH_NAME_TABLE= 0xA000,
		COGRE_MESH_EDGE_LISTS= 0xB000,

		// sub chunks of COGRE_SUBMESH
		COGRE_SUBMESH_OPERATION= 0x4010,
		COGRE_SUBMESH_BONE_ASSIGNMENT= 0x4100,
		COGRE_SUBMESH_TEXTURE_ALIAS= 0x4200,

		// sub chunks of COGRE_GEOMETRY
		COGRE_GEOMETRY_VERTEX_DECLARATION= 0x5100,
		COGRE_GEOMETRY_VERTEX_ELEMENT= 0x5110,
		COGRE_GEOMETRY_VERTEX_BUFFER= 0x5200,
		COGRE_GEOMETRY_VERTEX_BUFFER_DATA= 0x5210
	};

//! Constructor
COgreMeshFileLoader::COgreMeshFileLoader(io::IFileSystem* fs, video::IVideoDriver* driver)
: FileSystem(fs), Driver(driver), SwapEndian(false), Mesh(0), NumUV(0)
{

	#ifdef _DEBUG
	setDebugName("COgreMeshFileLoader");
	#endif

	if (FileSystem)
		FileSystem->grab();

	if (Driver)
		Driver->grab();
}


//! destructor
COgreMeshFileLoader::~COgreMeshFileLoader()
{
	clearMeshes();

	if (FileSystem)
		FileSystem->drop();

	if (Driver)
		Driver->drop();

	if (Mesh)
		Mesh->drop();
}


//! returns true if the file maybe is able to be loaded by this class
//! based on the file extension (e.g. ".bsp")
bool COgreMeshFileLoader::isALoadableFileExtension(const core::string<c16>& filename) const
{
	return core::hasFileExtension ( filename, "mesh" );
}


//! creates/loads an animated mesh from the file.
//! \return Pointer to the created mesh. Returns 0 if loading failed.
//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
//! See IReferenceCounted::drop() for more information.
IAnimatedMesh* COgreMeshFileLoader::createMesh(io::IReadFile* file)
{
	s16 id;

	file->read(&id, 2);

	if (id == COGRE_HEADER)
		SwapEndian=false;
	else if (id == 0x0010)
		SwapEndian=true;
	else
		return 0;
	ChunkData data;
	readString(file, data, Version);
	if ((Version != "[MeshSerializer_v1.30]") && (Version != "[MeshSerializer_v1.40]"))
		return 0;

	clearMeshes();
	if (Mesh)
		Mesh->drop();

	CurrentlyLoadingFromPath = FileSystem->getFileDir(file->getFileName());
	loadMaterials(file);

	Mesh = new SMesh();
	if (readChunk(file))
	{
		// success
		SAnimatedMesh* am = new SAnimatedMesh();
		am->Type = EAMT_3DS;

		for (u32 i=0; i<Mesh->getMeshBufferCount(); ++i)
			((SMeshBuffer*)Mesh->getMeshBuffer(i))->recalculateBoundingBox();

		Mesh->recalculateBoundingBox();

		am->addMesh(Mesh);
		am->recalculateBoundingBox();
		Mesh->drop();
		Mesh = 0;
        	return am;
	}

	Mesh->drop();
	Mesh = 0;

	return 0;
}


bool COgreMeshFileLoader::readChunk(io::IReadFile* file)
{
	while(file->getPos() < file->getSize())
	{
		ChunkData data;
		readChunkData(file, data);

		switch(data.header.id)
		{
		case COGRE_MESH:
			{
				Meshes.push_back(OgreMesh());
				readObjectChunk(file, data, Meshes.getLast());
				composeObject();
			}
			break;
		default:
			return true;
		}
	}

	return true;
}


bool COgreMeshFileLoader::readObjectChunk(io::IReadFile* file, ChunkData& parent, OgreMesh& mesh)
{
#ifdef IRR_OGRE_LOADER_DEBUG
	os::Printer::log("Read Object Chunk");
#endif
	readBool(file, parent, mesh.SkeletalAnimation);
	while ((parent.read < parent.header.length)&&(file->getPos() < file->getSize()))
	{
		ChunkData data;
		readChunkData(file, data);

		switch(data.header.id)
		{
			case COGRE_GEOMETRY:
			{
				readGeometry(file, data, mesh.Geometry);
			}
			break;
			case COGRE_SUBMESH: 
				mesh.SubMeshes.push_back(OgreSubMesh());
				readSubMesh(file, data, mesh.SubMeshes.getLast());
			break;
			case COGRE_MESH_BOUNDS: 
			{
				readVector(file, data, mesh.BBoxMinEdge);
				readVector(file, data, mesh.BBoxMaxEdge);
				readFloat(file, data, &mesh.BBoxRadius);
			}
			break;
			case COGRE_SKELETON_LINK: 
			case COGRE_BONE_ASSIGNMENT: 
			case COGRE_MESH_LOD: 
			case COGRE_MESH_SUBMESH_NAME_TABLE:
			case COGRE_MESH_EDGE_LISTS:
				// ignore chunk
				file->seek(data.header.length-data.read, true);
				data.read += data.header.length-data.read;
				break;
			default:
				parent.read=parent.header.length;
				file->seek(-(long)sizeof(ChunkHeader), true);
				return true;
		}
		parent.read += data.read;
	}
	return true;
}


bool COgreMeshFileLoader::readGeometry(io::IReadFile* file, ChunkData& parent, OgreGeometry& geometry)
{
#ifdef IRR_OGRE_LOADER_DEBUG
	os::Printer::log("Read Geometry");
#endif
	readInt(file, parent, &geometry.NumVertex);
	while(parent.read < parent.header.length)
	{
		ChunkData data;
		readChunkData(file, data);

		switch(data.header.id)
		{
		case COGRE_GEOMETRY_VERTEX_DECLARATION:
			readVertexDeclaration(file, data, geometry);
			break;
		case COGRE_GEOMETRY_VERTEX_BUFFER:
			readVertexBuffer(file, data, geometry);
			break;
		default:
			// ignore chunk
			file->seek(data.header.length-data.read, true);
			data.read += data.header.length-data.read;
		}
		parent.read += data.read;
	}
	return true;
}


bool COgreMeshFileLoader::readVertexDeclaration(io::IReadFile* file, ChunkData& parent, OgreGeometry& geometry)
{
#ifdef IRR_OGRE_LOADER_DEBUG
	os::Printer::log("Read Vertex Declaration");
#endif
	NumUV = 0;
	while(parent.read < parent.header.length)
	{
		ChunkData data;
		readChunkData(file, data);

		switch(data.header.id)
		{
		case COGRE_GEOMETRY_VERTEX_ELEMENT:
		{
			geometry.Elements.push_back(OgreVertexElement());
			OgreVertexElement& elem = geometry.Elements.getLast();
			readShort(file, data, &elem.Source);
			readShort(file, data, &elem.Type);
			readShort(file, data, &elem.Semantic);
			if (elem.Semantic == 7) //Tex coords
			{
				++NumUV;
			}
			readShort(file, data, &elem.Offset);
			elem.Offset /= sizeof(f32);
			readShort(file, data, &elem.Index);
		}
			break;
		default:
			// ignore chunk
			file->seek(data.header.length-data.read, true);
			data.read += data.header.length-data.read;
		}
		parent.read += data.read;
	}
	return true;
}


bool COgreMeshFileLoader::readVertexBuffer(io::IReadFile* file, ChunkData& parent, OgreGeometry& geometry)
{
#ifdef IRR_OGRE_LOADER_DEBUG
	os::Printer::log("Read Vertex Buffer");
#endif
	OgreVertexBuffer buf;
	readShort(file, parent, &buf.BindIndex);
	readShort(file, parent, &buf.VertexSize);
	buf.VertexSize /= sizeof(f32);
	ChunkData data;
	readChunkData(file, data);

	if (data.header.id == COGRE_GEOMETRY_VERTEX_BUFFER_DATA)
	{
		buf.Data.set_used(geometry.NumVertex*buf.VertexSize);
		readFloat(file, data, buf.Data.pointer(), geometry.NumVertex*buf.VertexSize);
	}

	geometry.Buffers.push_back(buf);
	parent.read += data.read;
	return true;
}


bool COgreMeshFileLoader::readSubMesh(io::IReadFile* file, ChunkData& parent, OgreSubMesh& subMesh)
{
#ifdef IRR_OGRE_LOADER_DEBUG
	os::Printer::log("Read Submesh");
#endif
	readString(file, parent, subMesh.Material);
	readBool(file, parent, subMesh.SharedVertices);

	s32 numIndices;
	readInt(file, parent, &numIndices);
	subMesh.Indices.set_used(numIndices);

	readBool(file, parent, subMesh.Indices32Bit);

	if (subMesh.Indices32Bit)
		readInt(file, parent, subMesh.Indices.pointer(), numIndices);
	else
		for (s32 i=0; i<numIndices; ++i)
		{
			u16 num;
			readShort(file, parent, &num);
			subMesh.Indices[i]=num;
		}

	if (!subMesh.SharedVertices)
	{
		ChunkData data;
		readChunkData(file, data);

		if (data.header.id==COGRE_GEOMETRY)
		{
			readGeometry(file, data, subMesh.Geometry);
		}
		parent.read += data.read;
	}

	while(parent.read < parent.header.length)
	{
		ChunkData data;
		readChunkData(file, data);

		switch(data.header.id)
		{
		case COGRE_SUBMESH_OPERATION:
			readShort(file, data, &subMesh.Operation);
			break;
		case COGRE_SUBMESH_TEXTURE_ALIAS:
		{
			core::stringc texture, alias;
			readString(file, data, texture);
			readString(file, data, alias);
			subMesh.TextureAliases.push_back(OgreTextureAlias(texture,alias));
		}
			break;
		case COGRE_SUBMESH_BONE_ASSIGNMENT:
			// currently ignore chunk
			file->seek(data.header.length-data.read, true);
			data.read += data.header.length-data.read;
			break;
		default:
			parent.read=parent.header.length;
			file->seek(-(long)sizeof(ChunkHeader), true);
			return true;
		}
		parent.read += data.read;
	}
	return true;
}


void COgreMeshFileLoader::composeMeshBufferMaterial(scene::IMeshBuffer* mb, const core::stringc& materialName)
{
	video::SMaterial& material=mb->getMaterial();
	for (u32 k=0; k<Materials.size(); ++k)
	{
		if ((materialName==Materials[k].Name)&&(Materials[k].Techniques.size())&&(Materials[k].Techniques[0].Passes.size()))
		{
			material=Materials[k].Techniques[0].Passes[0].Material;
			if (Materials[k].Techniques[0].Passes[0].Texture.Filename.size())
			{
				material.setTexture(0, Driver->getTexture(Materials[k].Techniques[0].Passes[0].Texture.Filename));
				if (!material.getTexture(0))
				{
					// retry with relative path
					core::stringc relative = Materials[k].Techniques[0].Passes[0].Texture.Filename;
					s32 idx = relative.findLast('\\');
					if (idx != -1)
						relative = relative.subString(idx+1, relative.size()-idx-1);
					idx = relative.findLast('/');
					if (idx != -1)
						relative = relative.subString(idx+1, relative.size()-idx-1);
					material.setTexture(0, Driver->getTexture((CurrentlyLoadingFromPath+"/"+relative)));
				}
			}
			break;
		}
	}
}


scene::SMeshBuffer* COgreMeshFileLoader::composeMeshBuffer(const core::array<s32>& indices, const OgreGeometry& geom)
{
	scene::SMeshBuffer *mb=new scene::SMeshBuffer();

	u32 i;
	mb->Indices.set_used(indices.size());
	for (i=0; i<indices.size(); ++i)
		mb->Indices[i]=indices[i];

	mb->Vertices.set_used(geom.NumVertex);
	for (i=0; i<geom.Elements.size(); ++i)
	{
		if (geom.Elements[i].Semantic==1) //Pos
		{
			for (u32 j=0; j<geom.Buffers.size(); ++j)
			{
				if (geom.Elements[i].Source==geom.Buffers[j].BindIndex)
				{
					u32 eSize=geom.Buffers[j].VertexSize;
					u32 ePos=geom.Elements[i].Offset;
					for (s32 k=0; k<geom.NumVertex; ++k)
					{
						mb->Vertices[k].Color=mb->Material.DiffuseColor;
						mb->Vertices[k].Pos.set(geom.Buffers[j].Data[ePos],geom.Buffers[j].Data[ePos+1],geom.Buffers[j].Data[ePos+2]);
						ePos += eSize;
					}
				}
			}
		}

		if (geom.Elements[i].Semantic==4) //Normal
		{
			for (u32 j=0; j<geom.Buffers.size(); ++j)
			{
				if (geom.Elements[i].Source==geom.Buffers[j].BindIndex)
				{
					u32 eSize=geom.Buffers[j].VertexSize;
					u32 ePos=geom.Elements[i].Offset;
					for (s32 k=0; k<geom.NumVertex; ++k)
					{
						mb->Vertices[k].Normal.set(geom.Buffers[j].Data[ePos],geom.Buffers[j].Data[ePos+1],geom.Buffers[j].Data[ePos+2]);
						ePos += eSize;
					}
				}
			}
		}

		if (geom.Elements[i].Semantic==7) //TexCoord
		{
			for (u32 j=0; j<geom.Buffers.size(); ++j)
			{
				if (geom.Elements[i].Source==geom.Buffers[j].BindIndex)
				{
					u32 eSize=geom.Buffers[j].VertexSize;
					u32 ePos=geom.Elements[i].Offset;
					for (s32 k=0; k<geom.NumVertex; ++k)
					{
						mb->Vertices[k].TCoords.set(geom.Buffers[j].Data[ePos],geom.Buffers[j].Data[ePos+1]);
						ePos += eSize;
					}
				}
			}
		}
	}
	return mb;
}


scene::SMeshBufferLightMap* COgreMeshFileLoader::composeMeshBufferLightMap(const core::array<s32>& indices, const OgreGeometry& geom)
{
	scene::SMeshBufferLightMap *mb=new scene::SMeshBufferLightMap();

	u32 i;
	mb->Indices.set_used(indices.size());
	for (i=0; i<indices.size(); ++i)
		mb->Indices[i]=indices[i];

	mb->Vertices.set_used(geom.NumVertex);

	for (i=0; i<geom.Elements.size(); ++i)
	{
		if (geom.Elements[i].Semantic==1) //Pos
		{
			for (u32 j=0; j<geom.Buffers.size(); ++j)
			{
				if (geom.Elements[i].Source==geom.Buffers[j].BindIndex)
				{
					u32 eSize=geom.Buffers[j].VertexSize;
					u32 ePos=geom.Elements[i].Offset;
					for (s32 k=0; k<geom.NumVertex; ++k)
					{
						mb->Vertices[k].Color=mb->Material.DiffuseColor;
						mb->Vertices[k].Pos.set(geom.Buffers[j].Data[ePos],geom.Buffers[j].Data[ePos+1],geom.Buffers[j].Data[ePos+2]);
						ePos += eSize;
					}
				}
			}
		}

		if (geom.Elements[i].Semantic==4) //Normal
		{
			for (u32 j=0; j<geom.Buffers.size(); ++j)
			{
				if (geom.Elements[i].Source==geom.Buffers[j].BindIndex)
				{
					u32 eSize=geom.Buffers[j].VertexSize;
					u32 ePos=geom.Elements[i].Offset;
					for (s32 k=0; k<geom.NumVertex; ++k)
					{
						mb->Vertices[k].Normal.set(geom.Buffers[j].Data[ePos],geom.Buffers[j].Data[ePos+1],geom.Buffers[j].Data[ePos+2]);
						ePos += eSize;
					}
				}
			}
		}

		if (geom.Elements[i].Semantic==7) //TexCoord
		{
			for (u32 j=0; j<geom.Buffers.size(); ++j)
			{
				if (geom.Elements[i].Source==geom.Buffers[j].BindIndex)
				{
					u32 eSize=geom.Buffers[j].VertexSize;
					u32 ePos=geom.Elements[i].Offset;
					for (s32 k=0; k<geom.NumVertex; ++k)
					{
						mb->Vertices[k].TCoords.set(geom.Buffers[j].Data[ePos], geom.Buffers[j].Data[ePos+1]);
						mb->Vertices[k].TCoords2.set(geom.Buffers[j].Data[ePos+2], geom.Buffers[j].Data[ePos+3]);
						
						ePos += eSize;
					}
				}
			}
		}
	}

	return mb;
}


void COgreMeshFileLoader::composeObject(void)
{
	for (u32 i=0; i<Meshes.size(); ++i)
	{
		for (u32 j=0; j<Meshes[i].SubMeshes.size(); ++j)
		{
			IMeshBuffer* mb;
			if (Meshes[i].SubMeshes[j].SharedVertices)
			{
				if (NumUV < 2)
				{
					mb = composeMeshBuffer(Meshes[i].SubMeshes[j].Indices, Meshes[i].Geometry);
				}
				else
				{
					mb = composeMeshBufferLightMap(Meshes[i].SubMeshes[j].Indices, Meshes[i].Geometry);
				}
			}
			else
			{
				if (NumUV < 2)
				{
					mb = composeMeshBuffer(Meshes[i].SubMeshes[j].Indices, Meshes[i].SubMeshes[j].Geometry);
				}
				else
				{
					mb = composeMeshBufferLightMap(Meshes[i].SubMeshes[j].Indices, Meshes[i].SubMeshes[j].Geometry);
				}
			}

			if (mb != 0)
			{
				composeMeshBufferMaterial(mb, Meshes[i].SubMeshes[j].Material);
				Mesh->addMeshBuffer(mb);
				mb->drop();
			}
		}
	}
}


core::stringc COgreMeshFileLoader::getTextureFileName(const core::stringc& texture, 
						 core::stringc& model)
{
	s32 idx = -1;
	idx = model.findLast('/');

	if (idx == -1)
		idx = model.findLast('\\');

	if (idx == -1)
		return core::stringc();

	core::stringc p = model.subString(0, idx+1);
	p.append(texture);
	return p;
}


void COgreMeshFileLoader::getMaterialToken(io::IReadFile* file, core::stringc& token, bool noNewLine)
{
	bool parseString=false;
	c8 c=0;
	token = "";

	if (file->getPos() >= file->getSize())
		return;

	file->read(&c, sizeof(c8));
	// search for word beginning
	while ( core::isspace(c) && (file->getPos() < file->getSize()))
	{
		if (noNewLine && c=='\n')
		{
			file->seek(-1, true);
			return;
		}
		file->read(&c, sizeof(c8));
	}
	// check if we read a string
	if (c=='"')
	{
		parseString = true;
		file->read(&c, sizeof(c8));
	}
	do
	{
		if (c=='/')
		{
			file->read(&c, sizeof(c8));
			// check for comments, cannot be part of strings
			if (!parseString && (c=='/'))
			{
				// skip comments
				while(c!='\n')
					file->read(&c, sizeof(c8));
				if (!token.size())
				{
					// if we start with a comment we need to skip
					// following whitespaces, so restart
					getMaterialToken(file, token, noNewLine);
					return;
				}
				else
				{
					// else continue with next character
					file->read(&c, sizeof(c8));
					continue;
				}
			}
			else
			{
				// else append first slash and check if second char
				// ends this token
				token.append('/');
				if ((!parseString && core::isspace(c)) ||
						(parseString && (c=='"')))
					return;
			}
		}
		token.append(c);
		file->read(&c, sizeof(c8));
		// read until a token delimiter is found
	}
	while (((!parseString && !core::isspace(c)) || (parseString && (c!='"'))) &&
			(file->getPos() < file->getSize()));
	// we want to skip the last quotes of a string , but other chars might be the next
	// token already.
	if (!parseString)
		file->seek(-1, true);
}


bool COgreMeshFileLoader::readColor(io::IReadFile* file, video::SColor& col)
{
	core::stringc token;

	getMaterialToken(file, token);
	if (token!="vertexcolour")
	{
		video::SColorf col_f;
		col_f.r=core::fast_atof(token.c_str());
		getMaterialToken(file, token);
		col_f.g=core::fast_atof(token.c_str());
		getMaterialToken(file, token);
		col_f.b=core::fast_atof(token.c_str());
		getMaterialToken(file, token, true);
		if (token.size())
			col_f.a=core::fast_atof(token.c_str());
		else
			col_f.a=1.0f;
		if ((col_f.r==0.0f)&&(col_f.g==0.0f)&&(col_f.b==0.0f))
			col.set(255,255,255,255);
		else
			col=col_f.toSColor();
		return false;
	}
	return true;
}


void COgreMeshFileLoader::readPass(io::IReadFile* file, OgreTechnique& technique)
{
#ifdef IRR_OGRE_LOADER_DEBUG
	os::Printer::log("Read Pass");
#endif
	core::stringc token;
	technique.Passes.push_back(OgrePass());
	OgrePass& pass=technique.Passes.getLast();

	getMaterialToken(file, token); //open brace or name
	if (token != "{")
		getMaterialToken(file, token); //open brace

	getMaterialToken(file, token);
	u32 inBlocks=1;
	u32 textureUnit=0;
	while(inBlocks)
	{
		if (token=="ambient")
			pass.AmbientTokenColor=readColor(file, pass.Material.AmbientColor);
		else if (token=="diffuse")
			pass.DiffuseTokenColor=readColor(file, pass.Material.DiffuseColor);
		else if (token=="specular")
		{
			pass.SpecularTokenColor=readColor(file, pass.Material.SpecularColor);
			getMaterialToken(file, token);
			pass.Material.Shininess=core::fast_atof(token.c_str());
		}
		else if (token=="emissive")
			pass.EmissiveTokenColor=readColor(file, pass.Material.EmissiveColor);
		else if (token=="scene_blend")
		{ // TODO: Choose correct values
			getMaterialToken(file, token);
			if (token=="add")
				pass.Material.MaterialType=video::EMT_TRANSPARENT_ADD_COLOR;
			else if (token=="modulate")
				pass.Material.MaterialType=video::EMT_SOLID;
			else if (token=="alpha_blend")
				pass.Material.MaterialType=video::EMT_TRANSPARENT_ALPHA_CHANNEL;
			else if (token=="colour_blend")
				pass.Material.MaterialType=video::EMT_TRANSPARENT_VERTEX_ALPHA;
			else
				getMaterialToken(file, token);
		}
		else if (token=="depth_check")
		{
			getMaterialToken(file, token);
			if (token!="on")
				pass.Material.ZBuffer=video::ECFN_NEVER;
		}
		else if (token=="depth_write")
		{
			getMaterialToken(file, token);
			pass.Material.ZWriteEnable=(token=="on");
		}
		else if (token=="depth_func")
		{
			getMaterialToken(file, token); // Function name
			if (token=="always_fail")
				pass.Material.ZBuffer=video::ECFN_NEVER;
			else if (token=="always_pass")
				pass.Material.ZBuffer=video::ECFN_ALWAYS;
			else if (token=="equal")
				pass.Material.ZBuffer=video::ECFN_EQUAL;
			else if (token=="greater")
				pass.Material.ZBuffer=video::ECFN_GREATER;
			else if (token=="greater_equal")
				pass.Material.ZBuffer=video::ECFN_GREATEREQUAL;
			else if (token=="less")
				pass.Material.ZBuffer=video::ECFN_LESS;
			else if (token=="less_equal")
				pass.Material.ZBuffer=video::ECFN_LESSEQUAL;
			else if (token=="not_equal")
				pass.Material.ZBuffer=video::ECFN_NOTEQUAL;
		}
		else if (token=="normalise_normals")
		{
			getMaterialToken(file, token);
			pass.Material.NormalizeNormals=(token=="on");
		}
		else if (token=="depth_bias")
		{
			getMaterialToken(file, token); // bias value
		}
		else if (token=="alpha_rejection")
		{
			getMaterialToken(file, token); // function name
			getMaterialToken(file, token); // value
			pass.Material.MaterialTypeParam=core::fast_atof(token.c_str());
		}
		else if (token=="alpha_to_coverage")
		{
			getMaterialToken(file, token);
			if (token=="on")
				pass.Material.AntiAliasing |= video::EAAM_ALPHA_TO_COVERAGE;
		}
		else if (token=="colour_write")
		{
			getMaterialToken(file, token);
			pass.Material.ColorMask = (token=="on")?video::ECP_ALL:video::ECP_NONE;
		}
		else if (token=="cull_hardware")
		{
			getMaterialToken(file, token); // rotation name
		}
		else if (token=="cull_software")
		{
			getMaterialToken(file, token); // culling side
		}
		else if (token=="lighting")
		{
			getMaterialToken(file, token);
			pass.Material.Lighting=(token=="on");
		}
		else if (token=="shading")
		{
			getMaterialToken(file, token);
			// We take phong as gouraud
			pass.Material.GouraudShading=(token!="flat");
		}
		else if (token=="polygon_mode")
		{
			getMaterialToken(file, token);
			pass.Material.Wireframe=(token=="wireframe");
			pass.Material.PointCloud=(token=="points");
		}
		else if (token=="max_lights")
		{
			getMaterialToken(file, token);
			pass.MaxLights=strtol(token.c_str(),NULL,10);
		}
		else if (token=="point_size")
		{
			getMaterialToken(file, token);
			pass.PointSize=core::fast_atof(token.c_str());
		}
		else if (token=="point_sprites")
		{
			getMaterialToken(file, token);
			pass.PointSprites=(token=="on");
		}
		else if (token=="point_size_min")
		{
			getMaterialToken(file, token);
			pass.PointSizeMin=strtol(token.c_str(),NULL,10);
		}
		else if (token=="point_size_max")
		{
			getMaterialToken(file, token);
			pass.PointSizeMax=strtol(token.c_str(),NULL,10);
		}
		else if (token=="texture_unit")
		{
#ifdef IRR_OGRE_LOADER_DEBUG
			os::Printer::log("Read Texture unit");
#endif
			getMaterialToken(file, token); //open brace
			getMaterialToken(file, token);
			while(token != "}")
			{
				if (token=="texture")
				{
					getMaterialToken(file, pass.Texture.Filename);
#ifdef IRR_OGRE_LOADER_DEBUG
					os::Printer::log("Read Texture", pass.Texture.Filename.c_str());
#endif
					getMaterialToken(file, pass.Texture.CoordsType, true);
					getMaterialToken(file, pass.Texture.MipMaps, true);
					getMaterialToken(file, pass.Texture.Alpha, true);
				}
				else if (token=="filtering")
				{
					getMaterialToken(file, token);
					pass.Material.TextureLayer[textureUnit].AnisotropicFilter=0;
					if (token=="point")
					{
						pass.Material.TextureLayer[textureUnit].BilinearFilter=false;
						pass.Material.TextureLayer[textureUnit].TrilinearFilter=false;
						getMaterialToken(file, token);
						getMaterialToken(file, token);
					}
					else if (token=="linear")
					{
						getMaterialToken(file, token);
						if (token=="point")
						{
							pass.Material.TextureLayer[textureUnit].BilinearFilter=false;
							pass.Material.TextureLayer[textureUnit].TrilinearFilter=false;
							getMaterialToken(file, token);
						}
						else
						{
							pass.Material.TextureLayer[textureUnit].BilinearFilter=true;
							getMaterialToken(file, token);
							pass.Material.TextureLayer[textureUnit].TrilinearFilter=(token=="linear");
						}
					}
					else
					{
						pass.Material.TextureLayer[textureUnit].BilinearFilter=(token=="bilinear");
						pass.Material.TextureLayer[textureUnit].TrilinearFilter=(token=="trilinear");
						pass.Material.TextureLayer[textureUnit].AnisotropicFilter=(token=="anisotropic")?2:1;
					}
				}
				else if (token=="max_anisotropy")
				{
					getMaterialToken(file, token);
					pass.Material.TextureLayer[textureUnit].AnisotropicFilter=(u8)core::strtol10(token.c_str());
				}
				else if (token=="texture_alias")
				{
					getMaterialToken(file, pass.Texture.Alias);
				}
				else if (token=="mipmap_bias")
				{
					getMaterialToken(file, token);
					pass.Material.TextureLayer[textureUnit].LODBias=(s8)core::fast_atof(token.c_str());
				}
				else if (token=="colour_op")
				{ // TODO: Choose correct values
					getMaterialToken(file, token);
					if (token=="add")
						pass.Material.MaterialType=video::EMT_TRANSPARENT_ADD_COLOR;
					else if (token=="modulate")
						pass.Material.MaterialType=video::EMT_SOLID;
					else if (token=="alpha_blend")
						pass.Material.MaterialType=video::EMT_TRANSPARENT_ALPHA_CHANNEL;
					else if (token=="colour_blend")
						pass.Material.MaterialType=video::EMT_TRANSPARENT_VERTEX_ALPHA;
					else
						getMaterialToken(file, token);
				}
				getMaterialToken(file, token);
			}
			++textureUnit;
		}
		else if (token=="shadow_caster_program_ref")
		{
			do
			{
				getMaterialToken(file, token);
			} while (token != "}");
			getMaterialToken(file, token);
		}
		else if (token=="vertex_program_ref")
		{
			do
			{
				getMaterialToken(file, token);
			} while (token != "}");
			getMaterialToken(file, token);
		}
		//fog_override, iteration, point_size_attenuation
		//not considered yet!
		getMaterialToken(file, token);
		if (token=="{")
			++inBlocks;
		else if (token=="}")
			--inBlocks;
	}
}


void COgreMeshFileLoader::readTechnique(io::IReadFile* file, OgreMaterial& mat)
{
#ifdef IRR_OGRE_LOADER_DEBUG
	os::Printer::log("Read Technique");
#endif
	core::stringc token;
	mat.Techniques.push_back(OgreTechnique());
	OgreTechnique& technique=mat.Techniques.getLast();

	getMaterialToken(file, technique.Name); //open brace or name
	if (technique.Name != "{")
		getMaterialToken(file, token); //open brace
	else
		technique.Name=core::stringc((int)mat.Techniques.size());

	getMaterialToken(file, token);
	while (token != "}")
	{
		if (token == "pass")
			readPass(file, technique);
		else if (token == "scheme")
			getMaterialToken(file, token);
		else if (token == "lod_index")
			getMaterialToken(file, token);
		getMaterialToken(file, token);
	}
}



void COgreMeshFileLoader::loadMaterials(io::IReadFile* meshFile)
{
#ifdef IRR_OGRE_LOADER_DEBUG
	os::Printer::log("Load Materials");
#endif
	core::stringc token = meshFile->getFileName();
	core::string<c16> filename = token.subString(0, token.size()-4) + L"material";
	io::IReadFile* file = FileSystem->createAndOpenFile(filename.c_str());

	if (!file)
	{
		os::Printer::log("Could not load OGRE material", filename.c_str());
		return;
	}

	getMaterialToken(file, token);

	while (file->getPos() < file->getSize())
	{
		if ((token == "fragment_program") || (token == "vertex_program"))
		{
			// skip whole block
			u32 blocks=1;
			do
			{
				getMaterialToken(file, token);
			} while (token != "{");
			do
			{
				getMaterialToken(file, token);
				if (token == "{")
					++blocks;
				else if (token == "}")
					--blocks;
			} while (blocks);
			getMaterialToken(file, token);
			continue;
		}
		if (token != "material")
		{
			if (token.trim().size())
				os::Printer::log("Unknown material group", token.c_str());
			break;
		}

		Materials.push_back(OgreMaterial());
		OgreMaterial& mat = Materials.getLast();

		getMaterialToken(file, mat.Name);
#ifdef IRR_OGRE_LOADER_DEBUG
	os::Printer::log("Load Material", mat.Name.c_str());
#endif
		getMaterialToken(file, token); //open brace
		getMaterialToken(file, token);
		while(token != "}")
		{
			if (token=="lod_distances") // can have several items
				getMaterialToken(file, token);
			else if (token=="receive_shadows")
			{
				getMaterialToken(file, token);
				mat.ReceiveShadows=(token=="on");
			}
			else if (token=="transparency_casts_shadows")
			{
				getMaterialToken(file, token);
				mat.TransparencyCastsShadows=(token=="on");
			}
			else if (token=="set_texture_alias")
			{
				getMaterialToken(file, token);
				getMaterialToken(file, token);
			}
			else if (token=="technique")
				readTechnique(file, mat);
			getMaterialToken(file, token);
		}
		getMaterialToken(file, token);
	}

	file->drop();
#ifdef IRR_OGRE_LOADER_DEBUG
	os::Printer::log("Finished loading Materials");
#endif
}


void COgreMeshFileLoader::readChunkData(io::IReadFile* file, ChunkData& data)
{
	file->read(&data.header, sizeof(ChunkHeader));
	if (SwapEndian)
	{
		data.header.id = os::Byteswap::byteswap(data.header.id);
		data.header.length = os::Byteswap::byteswap(data.header.length);
	}
	data.read += sizeof(ChunkHeader);
}


void COgreMeshFileLoader::readString(io::IReadFile* file, ChunkData& data, core::stringc& out)
{
	c8 c = 0;
	out = "";

	while (c!='\n')
	{
		file->read(&c, sizeof(c8));
		if (c!='\n')
			out.append(c);

	}
	data.read+=out.size()+1;
}


void COgreMeshFileLoader::readBool(io::IReadFile* file, ChunkData& data, bool& out)
{
	// normal C type because we read a bit string
	char c = 0;
	file->read(&c, sizeof(char));
	out=(c!=0);
	++data.read;
}


void COgreMeshFileLoader::readInt(io::IReadFile* file, ChunkData& data, s32* out, u32 num)
{
	// normal C type because we read a bit string
	file->read(out, sizeof(int));
	if (SwapEndian)
	{
		for (u32 i=0; i<num; ++i)
			out[i] = os::Byteswap::byteswap(out[i]);
	}
	data.read+=sizeof(int)*num;
}


void COgreMeshFileLoader::readShort(io::IReadFile* file, ChunkData& data, u16* out, u32 num)
{
	// normal C type because we read a bit string
	file->read(out, sizeof(short)*num);
	if (SwapEndian)
	{
		for (u32 i=0; i<num; ++i)
			out[i] = os::Byteswap::byteswap(out[i]);
	}
	data.read+=sizeof(short)*num;
}


void COgreMeshFileLoader::readFloat(io::IReadFile* file, ChunkData& data, f32* out, u32 num)
{
	// normal C type because we read a bit string
	file->read(out, sizeof(float)*num);
	if (SwapEndian)
	{
		for (u32 i=0; i<num; ++i)
			out[i] = os::Byteswap::byteswap(out[i]);
	}
	data.read+=sizeof(float)*num;
}


void COgreMeshFileLoader::readVector(io::IReadFile* file, ChunkData& data, core::vector3df& out)
{
	readFloat(file, data, &out.X);
	readFloat(file, data, &out.Y);
	readFloat(file, data, &out.Z);
}


void COgreMeshFileLoader::clearMeshes()
{
	for (u32 i=0; i<Meshes.size(); ++i)
	{
		for (int k=0; k<(int)Meshes[i].Geometry.Buffers.size(); ++k)
			Meshes[i].Geometry.Buffers[k].Data.clear();

		for (u32 j=0; j<Meshes[i].SubMeshes.size(); ++j)
		{
			for (int h=0; h<(int)Meshes[i].SubMeshes[j].Geometry.Buffers.size(); ++h)
				Meshes[i].SubMeshes[j].Geometry.Buffers[h].Data.clear();
		}
	}

	Meshes.clear();
}


} // end namespace scene
} // end namespace irr

#endif // _IRR_COMPILE_WITH_OGRE_LOADER_
