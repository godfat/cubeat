// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_FILE_SYSTEM_H_INCLUDED__
#define __C_FILE_SYSTEM_H_INCLUDED__

#include "IFileSystem.h"
#include "irrArray.h"

namespace irr
{
namespace io
{

	class CZipReader;
	class CPakReader;
	class CMountPointReader;

/*!
	FileSystem which uses normal files and one zipfile
*/
class CFileSystem : public IFileSystem
{
public:

	//! constructor
	CFileSystem();

	//! destructor
	virtual ~CFileSystem();

	//! opens a file for read access
	virtual IReadFile* createAndOpenFile(const core::string<c16>& filename);

	//! Creates an IReadFile interface for accessing memory like a file.
	virtual IReadFile* createMemoryReadFile(void* memory, s32 len, const core::string<c16>& fileName, bool deleteMemoryWhenDropped = false);

	//! Creates an IWriteFile interface for accessing memory like a file.
	virtual IWriteFile* createMemoryWriteFile(void* memory, s32 len, const core::string<c16>& fileName, bool deleteMemoryWhenDropped=false);

	//! Opens a file for write access.
	virtual IWriteFile* createAndWriteFile(const core::string<c16>& filename, bool append=false);

	//! Adds an archive to the file system.
	virtual bool registerFileArchive( const core::string<c16>& filename, bool ignoreCase = true, bool ignorePaths = true);

	//! move the hirarchy of the filesystem. moves sourceIndex relative up or down
	virtual bool moveFileArchive( u32 sourceIndex, s32 relative );

	//! Adds an external archive loader to the engine.
	virtual void addArchiveLoader(IArchiveLoader* loader);

	//! gets an archive
	virtual u32 getFileArchiveCount();
	virtual IFileArchive* getFileArchive( u32 index );
	//! removes an archive to the file system.
	virtual bool unregisterFileArchive( u32 index );

	//! Returns the string of the current working directory
	virtual const core::string<c16>& getWorkingDirectory();

	//! Changes the current Working Directory to the string given.
	//! The string is operating system dependent. Under Windows it will look
	//! like this: "drive:\directory\sudirectory\"
	virtual bool changeWorkingDirectoryTo(const core::string<c16>& newDirectory);

	//! Converts a relative path to an absolute (unique) path, resolving symbolic links
	virtual core::string<c16> getAbsolutePath(const core::string<c16>& filename) const;

	//! Returns the directory a file is located in.
	/** \param filename: The file to get the directory from */
	virtual core::string<c16> getFileDir(const core::string<c16>& filename) const;

	//! Returns the base part of a filename, i.e. the name without the directory
	//! part. If no directory is prefixed, the full name is returned.
	/** \param filename: The file to get the basename from */
	virtual core::string<c16> getFileBasename(const core::string<c16>& filename, bool keepExtension=true) const;

	//! flatten a path and file name for example: "/you/me/../." becomes "/you"
	virtual core::string<c16>& flattenFilename( core::string<c16>& directory, const core::string<c16>& root = "/" ) const;

	//! Creates a list of files and directories in the current working directory 
	//! and returns it.
	virtual eFileSystemType setFileListSystem( eFileSystemType listType);
	virtual IFileList* createFileList();

	//! determinates if a file exists and would be able to be opened.
	virtual bool existFile(const core::string<c16>& filename) const;

	//! Creates a XML Reader from a file.
	virtual IXMLReader* createXMLReader(const core::string<c16>& filename);

	//! Creates a XML Reader from a file.
	virtual IXMLReader* createXMLReader(IReadFile* file);

	//! Creates a XML Reader from a file.
	virtual IXMLReaderUTF8* createXMLReaderUTF8(const core::string<c16>& filename);

	//! Creates a XML Reader from a file.
	virtual IXMLReaderUTF8* createXMLReaderUTF8(IReadFile* file);

	//! Creates a XML Writer from a file.
	virtual IXMLWriter* createXMLWriter(const core::string<c16>& filename);

	//! Creates a XML Writer from a file.
	virtual IXMLWriter* createXMLWriter(IWriteFile* file);

	//! Creates a new empty collection of attributes, usable for serialization and more.
	virtual IAttributes* createEmptyAttributes(video::IVideoDriver* driver);

private:

	eFileSystemType FileSystemType;					// Currently used FileSystemType
	core::string<c16> WorkingDirectory [2];			// WorkingDirectory for Native/Virtual

	core::array< IArchiveLoader* > ArchiveLoader;	// currently attached ArchiveLoaders
	core::array< IFileArchive*> FileArchive;		// currently attached Archives

};


} // end namespace irr
} // end namespace io

#endif

