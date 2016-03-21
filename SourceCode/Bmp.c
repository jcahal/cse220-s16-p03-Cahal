/***************************************************************************************************************
 * FILE: Bmp.c
 *
 * DESCRIPTION
 * See comments in Bmp.h.
 *
 * CSE 220
 **************************************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "Bmp.h"
#include "Error.h"
#include "File.h"

//==============================================================================================================
// PREPROCESSOR MACROS
//==============================================================================================================

// Asserts that 'cond' is true. If it is not, then we close the file stream 'stream' and return from the
// BmpRead() function with the return value 'error'.
#define BmpAssert(cond, stream, error) if (!(cond)) { if ((stream)) FileClose((stream)); return error; }

//==============================================================================================================
// PUBLIC CONSTANT DEFINTIONS
//==============================================================================================================

const size_t cSizeofBmpHeader     = 14;  // Size of the BMPHEADER struct.
const size_t cSizeofBmpInfoHeader = 40;  // Size of the BMPINFOHEADER struct.

//==============================================================================================================
// PRIVATE CONSTANT DEFINTIONS
//==============================================================================================================

// A valid BMP file has to be at least 58 bytes in size.
const size_t cBmpMinFileSize  = 58;

//==============================================================================================================
// PRIVATE FUNCTION DECLARATIONS
//==============================================================================================================

static int BmpCalcFileSize(int pWidth, int pHeight);
static int BmpCalcPad(int pWidth);

//==============================================================================================================
// FUNCTION DEFINITIONS
//==============================================================================================================

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: BmpCalcFileSize()
 *
 * DESCRIPTION
 * Calculate the file size based on the image width and height. This value is written to the BMPHEADER structure
 *------------------------------------------------------------------------------------------------------------*/
static int BmpCalcFileSize(int pWidth, int pHeight)
{
	return pWidth * pHeight;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: BmpCalcPad()
 *
 * DESCRIPTION
 * Calculate the number of padding bytes that are at the end of each row of pixel data in the pixel array.
 *------------------------------------------------------------------------------------------------------------*/
static int BmpCalcPad(int pWidth)
{
	return (4 - (3 * (pWidth % 4))) % 4;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: BmpPixelAlloc()
 *------------------------------------------------------------------------------------------------------------*/
tPixel **BmpPixelAlloc(int pWidth, int pHeight)
{
	tPixel **pPixel = (tPixel **)malloc(pHeight * sizeof(tPixel *));

	int padding = BmpCalcPad(pWidth);
	for(int row = 0; row < pHeight; row++) {
		pPixel[row] = (tPixel *)malloc((pWidth * sizeof(tPixel)) + padding);
	}
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: BmpPixelFree()
 *------------------------------------------------------------------------------------------------------------*/
void BmpPixelFree(tPixel **pPixel, int pHeight)
{
	for(int row = 0; row < pHeight; row++) {
		free(pPixel[row]);
	}
	free(pPixel);
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: BmpRead()
 *
 * DESCRIPTION
 *------------------------------------------------------------------------------------------------------------*/
tError BmpRead(char *pFilename, tBmp *pBmp)
{
	// TODO: Handle any Errors, see 4.4 in the handbook
	FILE *pFile = FileOpen(pFilename, "rb");

	if(pFile == NULL)
		return ErrorFileOpen; // if file open fails return error

	byte headerBuffer[sizeof(pBmp->header)];
	byte infoHeaderBuffer[sizeof(pBmp->infoHeader)];
	byte pixelBuffer[FileSize(pFilename)];

	if(FileRead(pFile, &headerBuffer, sizeof(byte), sizeof(headerBuffer)))
		return ErrorFileRead;
	if(FileRead(pFile, &infoHeaderBuffer, sizeof(byte), sizeof(infoHeaderBuffer)))
		return ErrorFileRead;
	if(FileRead(pFile, &pixelBuffer, sizeof(byte), sizeof(pixelBuffer)))
		return ErrorFileRead;

	// copy headerBuffer to pBmp->header
	pBmp->header.sigB = headerBuffer[0];
	pBmp->header.sigM = headerBuffer[1];

	if(pBmp->header.sigB != 'B' || pBmp->header.sigM != 'M')
		return ErrorBmpInv;

	memcopy(&pBmp->header.fileSize, &headerBuffer[2], sizeof(pBmp->header.fileSize));

	memcopy(&pBmp->header.resv1, &headerBuffer[6], sizeof(pBmp->header.resv1));
	memcopy(&pBmp->header.resv2, &headerBuffer[8], sizeof(pBmp->header.resv2));

	memcopy(&pBmp->header.pixelOffset, &headerBuffer[10], sizeof(pBmp->header.pixelOffset));

	// copy infoHeaderBuffer to pBmp->infoHeader
	memcopy(&pBmp->infoHeader.size, &infoHeaderBuffer[0], sizeof(pBmp->infoHeader.size));

	memcopy(&pBmp->infoHeader.width, &infoHeaderBuffer[4], sizeof(pBmp->infoHeader.width));
	memcopy(&pBmp->infoHeader.height, &infoHeaderBuffer[8], sizeof(pBmp->infoHeader.height));

	memcopy(&pBmp->infoHeader.colorPlanes, &infoHeaderBuffer[12], sizeof(pBmp->infoHeader.colorPlanes));
	memcopy(&pBmp->infoHeader.bitsPerPixel, &infoHeaderBuffer[14], sizeof(pBmp->infoHeader.bitsPerPixel));

	memcopy(&pBmp->infoHeader.zeros, &infoHeaderBuffer[16], sizeof(pBmp->infoHeader.zeros));

	// copy pixelBuffer into pBmp->pixel
	pBmp->pixel = BmpPixelAlloc(pBmp->infoHeader.width, pBmp->infoHeader.height); // allocate for pixel

	int pixelPadding = BmpCalcPad(pBmp->infoHeader.width);
	int pixelRowWidth = pBmp->infoHeader.width + pixelPadding;
	for(int row = 0; row < pBmp->infoHeader.height; row++) {
		memcopy(&pBmp->pixel[row], &pixelBuffer[row * pixelRowWidth], pixelRowWidth);
	}
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: BmpWrite()
 *------------------------------------------------------------------------------------------------------------*/
tError BmpWrite(char *pFilename, tBmp *pBmp)
{
	// TODO: Handle any Errors, see 4.4 in the handbook
	FILE *pFile = FileOpen(pFilename, "wb");

	if(pFile == NULL)
		return ErrorFileOpen; // if file open fails return error

	byte headerBuffer[sizeof(pBmp->header)];
	byte infoHeaderBuffer[sizeof(pBmp->infoHeader)];
	byte pixelBuffer[FileSize(pFilename)];

	// copy pBmp->header to headerBuffer
	headerBuffer[0] = pBmp->header.sigB;
	headerBuffer[1] = pBmp->header.sigM;

	memcopy(&headerBuffer[2], &pBmp->header.fileSize, sizeof(pBmp->header.fileSize));

	memcopy(&headerBuffer[6], &pBmp->header.resv1, sizeof(pBmp->header.resv1));
	memcopy(&headerBuffer[8], &pBmp->header.resv2, sizeof(pBmp->header.resv2));

	memcopy(&headerBuffer[10], &pBmp->header.pixelOffset, sizeof(pBmp->header.pixelOffset));

	// copy pBmp->infoHeader to infoHeaderBuffer
	memcopy(&infoHeaderBuffer[0], &pBmp->infoHeader.size, sizeof(pBmp->infoHeader.size));

	memcopy(&infoHeaderBuffer[4], &pBmp->infoHeader.width, sizeof(pBmp->infoHeader.width));
	memcopy(&infoHeaderBuffer[8], &pBmp->infoHeader.height, sizeof(pBmp->infoHeader.height));

	memcopy(&infoHeaderBuffer[12], &pBmp->infoHeader.colorPlanes, sizeof(pBmp->infoHeader.colorPlanes));
	memcopy(&infoHeaderBuffer[14], &pBmp->infoHeader.bitsPerPixel, sizeof(pBmp->infoHeader.bitsPerPixel));

	memcopy(&infoHeaderBuffer[16], &pBmp->infoHeader.zeros, sizeof(pBmp->infoHeader.zeros));

	int pixelPadding = BmpCalcPad(pBmp->infoHeader.width);
	int pixelRowWidth = pBmp->infoHeader.width + pixelPadding;
	for(int row = 0; row < pBmp->infoHeader.height; row++) {
		// TODO: not sure if pixelRowWidth will work here as it is not type size_t
		memcopy(&pixelBuffer[row * pixelRowWidth], &pBmp->pixel[row], pixelRowWidth);
	}

	if(FileWrite(pFile, &headerBuffer, sizeof(byte), sizeof(headerBuffer)))
		return ErrorFileWrite;
	if(FileRead(pFile, &infoHeaderBuffer, sizeof(byte), sizeof(infoHeaderBuffer)))
		return ErrorFileWrite;
	if(FileRead(pFile, &pixelBuffer, sizeof(byte), sizeof(pixelBuffer)))
		return ErrorFileWrite;
}
