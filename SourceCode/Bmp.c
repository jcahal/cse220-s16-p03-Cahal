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
	return pPixel;
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

	byte headerBuffer[cSizeofBmpHeader];
	byte infoHeaderBuffer[cSizeofBmpInfoHeader];

	if(FileRead(pFile, &headerBuffer, sizeof(byte), sizeof(headerBuffer)))
		return ErrorFileRead;
	if(FileRead(pFile, &infoHeaderBuffer, sizeof(byte), sizeof(infoHeaderBuffer)))
		return ErrorFileRead;

	printf("%d %d\n", sizeof(headerBuffer), sizeof(infoHeaderBuffer));
	BmpCalcFileSize(4,5);
	// copy headerBuffer to pBmp->header
	pBmp->header.sigB = headerBuffer[0];
	pBmp->header.sigM = headerBuffer[1];

	if(pBmp->header.sigB != 'B' || pBmp->header.sigM != 'M')
		return ErrorBmpInv;

	memcpy(&pBmp->header.fileSize, &headerBuffer[2], sizeof(pBmp->header.fileSize));

	memcpy(&pBmp->header.resv1, &headerBuffer[6], sizeof(pBmp->header.resv1));
	memcpy(&pBmp->header.resv2, &headerBuffer[8], sizeof(pBmp->header.resv2));

	memcpy(&pBmp->header.pixelOffset, &headerBuffer[10], sizeof(pBmp->header.pixelOffset));

	// copy infoHeaderBuffer to pBmp->infoHeader
	memcpy(&pBmp->infoHeader.size, &infoHeaderBuffer[0], sizeof(pBmp->infoHeader.size));

	memcpy(&pBmp->infoHeader.width, &infoHeaderBuffer[4], sizeof(pBmp->infoHeader.width));
	memcpy(&pBmp->infoHeader.height, &infoHeaderBuffer[8], sizeof(pBmp->infoHeader.height));

	memcpy(&pBmp->infoHeader.colorPlanes, &infoHeaderBuffer[12], sizeof(pBmp->infoHeader.colorPlanes));
	memcpy(&pBmp->infoHeader.bitsPerPixel, &infoHeaderBuffer[14], sizeof(pBmp->infoHeader.bitsPerPixel));

	memcpy(&pBmp->infoHeader.zeros, &infoHeaderBuffer[16], sizeof(pBmp->infoHeader.zeros));

	// copy pixelBuffer into pBmp->pixel
	pBmp->pixel = BmpPixelAlloc(pBmp->infoHeader.width, pBmp->infoHeader.height); // allocate for pixel

	int pixelPadding = BmpCalcPad(pBmp->infoHeader.width);
	int pixelRowWidth = (3 * pBmp->infoHeader.width) + pixelPadding;
	byte pixelBuffer[pixelRowWidth];

	printf("%d\n", pixelRowWidth);
	for(int row = 0; row < pBmp->infoHeader.height; row++) {
		
		if(FileRead(pFile, pixelBuffer, sizeof(byte), pixelRowWidth))
			return ErrorFileRead;
		for(int col = 0; col < pBmp->infoHeader.width; col++) {
			memcpy(&pBmp->pixel[row][col].blue, &pixelBuffer[3 * col], sizeof(byte));
			memcpy(&pBmp->pixel[row][col].green, &pixelBuffer[(3 * col) + 1], sizeof(byte));
			memcpy(&pBmp->pixel[row][col].red, &pixelBuffer[(3 * col) + 2], sizeof(byte));
		}

		// TODO: Check for corrupt BMP
	}

	FileClose(pFile);
	return ErrorNone;
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

	// copy pBmp->header to headerBuffer
	headerBuffer[0] = pBmp->header.sigB;
	headerBuffer[1] = pBmp->header.sigM;

	memcpy(&headerBuffer[2], &pBmp->header.fileSize, sizeof(pBmp->header.fileSize));

	memcpy(&headerBuffer[6], &pBmp->header.resv1, sizeof(pBmp->header.resv1));
	memcpy(&headerBuffer[8], &pBmp->header.resv2, sizeof(pBmp->header.resv2));

	memcpy(&headerBuffer[10], &pBmp->header.pixelOffset, sizeof(pBmp->header.pixelOffset));

	// copy pBmp->infoHeader to infoHeaderBuffer
	memcpy(&infoHeaderBuffer[0], &pBmp->infoHeader.size, sizeof(pBmp->infoHeader.size));

	memcpy(&infoHeaderBuffer[4], &pBmp->infoHeader.width, sizeof(pBmp->infoHeader.width));
	memcpy(&infoHeaderBuffer[8], &pBmp->infoHeader.height, sizeof(pBmp->infoHeader.height));

	memcpy(&infoHeaderBuffer[12], &pBmp->infoHeader.colorPlanes, sizeof(pBmp->infoHeader.colorPlanes));
	memcpy(&infoHeaderBuffer[14], &pBmp->infoHeader.bitsPerPixel, sizeof(pBmp->infoHeader.bitsPerPixel));

	memcpy(&infoHeaderBuffer[16], &pBmp->infoHeader.zeros, sizeof(pBmp->infoHeader.zeros));

	// write header and infoHeader
	if(FileWrite(pFile, &headerBuffer, sizeof(byte), sizeof(headerBuffer)))
		return ErrorFileWrite;
	if(FileWrite(pFile, &infoHeaderBuffer, sizeof(byte), sizeof(infoHeaderBuffer)))
		return ErrorFileWrite;

	int pixelPadding = BmpCalcPad(pBmp->infoHeader.width);
	int pixelRowWidth = (3 * pBmp->infoHeader.width) + pixelPadding;
	byte pixelBuffer[pixelRowWidth];

	for(int row = 0; row < pBmp->infoHeader.height; row++) {
		for(int col = 0; col < pBmp->infoHeader.width; col++) {
			memcpy(&pixelBuffer[3 * col], &pBmp->pixel[row][col].blue, sizeof(byte));
			memcpy(&pixelBuffer[(3 * col) + 1], &pBmp->pixel[row][col].green, sizeof(byte));
			memcpy(&pixelBuffer[(3 * col) + 2], &pBmp->pixel[row][col].red, sizeof(byte));
		}

		if(FileWrite(pFile, &pixelBuffer, sizeof(byte), sizeof(pixelBuffer)))
			return ErrorFileRead;
	}

	for(int row = 0; row < pBmp->infoHeader.height; row++) {
		// TODO: not sure if pixelRowWidth will work here as it is not type size_t
		memcpy(&pixelBuffer[row * pixelRowWidth], &pBmp->pixel[row], pixelRowWidth);
	}

	FileClose(pFile);
	return ErrorNone;
}
