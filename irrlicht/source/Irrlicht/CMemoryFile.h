// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_MEMORY_READ_FILE_H_INCLUDED__
#define __C_MEMORY_READ_FILE_H_INCLUDED__

#include "IReadFile.h"
#include "IWriteFile.h"
#include "irrString.h"

namespace irr
{

namespace io
{

	/*!
		Class for reading and writing from memory.
	*/
	class CMemoryFile : public IReadFile, public IWriteFile
	{
	public:

		//! Constructor
		CMemoryFile(void* memory, long len, const core::string<c16>& fileName, bool deleteMemoryWhenDropped);

		//! Destructor
		virtual ~CMemoryFile();

		//! returns how much was read
		virtual s32 read(void* buffer, u32 sizeToRead);

		//! returns how much was written
		virtual s32 write(const void* buffer, u32 sizeToWrite);

		//! changes position in file, returns true if successful
		virtual bool seek(long finalPos, bool relativeMovement = false);

		//! returns size of file
		virtual long getSize() const;

		//! returns where in the file we are.
		virtual long getPos() const;

		//! returns name of file
		virtual const core::string<c16>& getFileName() const;

	private:

		void *Buffer;
		long Len;
		long Pos;
		core::string<c16> Filename;
		bool deleteMemoryWhenDropped;
	};

} // end namespace io
} // end namespace irr

#endif

