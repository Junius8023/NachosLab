// filehdr.h
//	Data structures for managing a disk file header.
//
//	A file header describes where on disk to find the data in a file,
//	along with other information about the file (for instance, its
//	length, owner, etc.)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#ifndef FILEHDR_H
#define FILEHDR_H

#include "disk.h"
#include "bitmap.h"

/*
  采取二级索引的方式，文件索引表的长度为30， 将其前29个块作为一级索引，第30个块作为2级索引

*/
#define NumIndex ((SectorSize - 2 * sizeof(int)) / sizeof(int))
#define MaxFileSize ((29 + 32) * SectorSize)
#define NumDirectIndex (NumIndex - 1)
#define Num2Index (NumDirectIndex + 32)
// The following class defines the Nachos "file header" (in UNIX terms,
// the "i-node"), describing where on disk to find all of the data in the file.
// The file header is organized as a simple table of pointers to
// data blocks.
//
// The file header data structure can be stored in memory or on disk.
// When it is on disk, it is stored in a single sector -- this means
// that we assume the size of this data structure to be the same
// as one disk sector.  Without indirect addressing, this
// limits the maximum file length to just under 4K bytes.
//
// There is no constructor; rather the file header can be initialized
// by allocating blocks for the file (if it is a new file), or by
// reading it from disk.
class FileSystem;

class FileHeader
{
public:
	FileHeader();
    bool Allocate(BitMap *bitMap, int fileSize); // Initialize a file header,
                                                 //  including allocating space
                                                 //  on disk for the file disk
    void Deallocate(BitMap *bitMap);             // De-allocate this file's
                                                 //  data blocks

    void FetchFrom(int sectorNumber); // Initialize file header from disk
    void WriteBack(int sectorNumber); // Write modifications to file header
                                      //  back to disk

    int ByteToSector(int offset, FileSystem* filesys); // Convert a byte offset into the file
                                  // to the disk sector containing
                                  // the byte

    int FileLength(); // Return the length of the file
                      // in bytes

    void Print(); // Print the contents of the file.
    void setFileLength(int len){this->numBytes = len;};
    bool init(FileSystem* fileSystem);   // 初始化为一个目录文件，需要分配一个磁盘块
private:
    int numBytes;              // Number of bytes in the file
    int numSectors;            // Number of data sectors in the file
    int dataSectors[NumIndex]; // Disk sector numbers for each data
                               // block in the file
friend class Directory;
};

#endif // FILEHDR_H
