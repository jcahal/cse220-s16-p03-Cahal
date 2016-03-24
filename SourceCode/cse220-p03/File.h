/***************************************************************************************************************
 * FILE: File.h
 *
 * DESCRIPTION
 * Functions for performing file I/O.
 *
 * CSE 220
 **************************************************************************************************************/
#ifndef FILE_H
#define FILE_H

#include <stdio.h>

//==============================================================================================================
// FUNCTION DEFINITIONS
//==============================================================================================================

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: FileClose()
 *
 * DESCRIPTION
 * Closes the file stream (if it isn't stdin or stdout).
 *------------------------------------------------------------------------------------------------------------*/
void FileClose(FILE *);

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: FileOpen()
 *
 * DESCRIPTION
 * Open the file with name pFilename for reading or writing. pMode should be "rt" for reading a text file, "wt"
 * for writing a text file, "rb" for reading a binary file, or "wb" for writing a binary file. If pFilename is
 * the empty string, then we either reading from stdin or writing to stdout.
 *
 * RETURNS
 * A pointer to the FILE struct on success, NULL on failure.
 *------------------------------------------------------------------------------------------------------------*/
FILE *FileOpen(char *pFilename, char *pMode);

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: FileRead()
 *
 * DESCRIPTION
 * Reads from the file stream pStream.
 *
 * RETURNS
 * 0 on success, -1 on failure.
 *------------------------------------------------------------------------------------------------------------*/
int FileRead(FILE *pStream, void *pBlock, size_t pSize, size_t pCount);

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: FileSize()
 *
 * DESCRIPTION
 * Determines the size of a file.
 *
 * RETURNS
 * The size of the file named by pFilename, as a long. -1 on error.
 *------------------------------------------------------------------------------------------------------------*/
long FileSize(char *pFilename);

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: FileWrite()
 *
 * DESCRIPTION
 * Writes a block of data to the file stream pStream.
 *
 * RETURNS
 * 0 on success, -1 on failure.
 *------------------------------------------------------------------------------------------------------------*/
int FileWrite(FILE *pStream, void *pBlock, size_t pSize, size_t pCount);

#endif
