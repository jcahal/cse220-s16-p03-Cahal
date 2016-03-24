/***************************************************************************************************************
 * FILE: Image.c
 *
 * DESCRIPTION
 * See comments in Image.h.
 *
 * CSE 220
 **************************************************************************************************************/
#include "Bmp.h"
#include <stdio.h>

//==============================================================================================================
// FUNCTION DEFINITIONS
//==============================================================================================================

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: ImageFlipHoriz()
 *------------------------------------------------------------------------------------------------------------*/
void ImageFlipHoriz(tBmp *pBmp)
{
	tPixel pixel;
	for(int row = 0; row < pBmp->infoHeader.height; row++) {
		for(int col = 0; col < pBmp->infoHeader.width / 2; col++) {
			pixel = pBmp->pixel[row][col];
			pBmp->pixel[row][col] = pBmp->pixel[row][pBmp->infoHeader.width - col];
			pBmp->pixel[row][pBmp->infoHeader.width - col] = pixel;
		}
	}
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: ImageRotRight()
 *------------------------------------------------------------------------------------------------------------*/
void ImageRotRight(tBmp *pBmp)
{
	tPixel **pPixel = BmpPixelAlloc(pBmp->infoHeader.height, pBmp->infoHeader.width);

	// copy to tmp pPixel
	for(int row = 0; row < pBmp->infoHeader.width; row++) {
		for(int col = 0; col < pBmp->infoHeader.height; col++) {
			pPixel[row][col].blue = pBmp->pixel[(pBmp->infoHeader.height - 1) - col][row].blue;
			pPixel[row][col].green = pBmp->pixel[(pBmp->infoHeader.height - 1) - col][row].green;
			pPixel[row][col].red = pBmp->pixel[(pBmp->infoHeader.height - 1) - col][row].red;
		}

	}

	// free pixel
	BmpPixelFree(pBmp->pixel, pBmp->infoHeader.height);

	pBmp->pixel = pPixel;
	int32_t tmp = pBmp->infoHeader.width;
	pBmp->infoHeader.width = pBmp->infoHeader.height;
	pBmp->infoHeader.height = tmp;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: ImageRotRightMult()
 *
 * REMARKS
 * nTimes will be 0, 1, 2, or 3. If it is 0, we don't need to do anything. Rotate right one time,
 *
 *     1 2
 *                                 5 3 1
 *     3 4    rotate right 1 -->
 *                                 6 4 2
 *     5 6
 *
 * Rotate right twice,
 *
 *     1 2                         6 5
 *
 *     3 4    rotate right 2 -->   4 3
 *
 *     5 6                         2 1
 *
 * Rotate right three times,
 *
 *     1 2
 *                                 2 4 6
 *     3 4    rotate right 3 -->
 *                                 1 3 5
 *     5 6
 *------------------------------------------------------------------------------------------------------------*/
void ImageRotRightMult(tBmp *pBmp, int nTimes) {
	if(nTimes % 4 == 0) {
		return;
	} else {
		for(int i = 0; i < nTimes; i++) {
			ImageRotRight(pBmp);
		}
	}
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: ImageFlipVert()
 *------------------------------------------------------------------------------------------------------------*/
void ImageFlipVert(tBmp *pBmp)
{
	tPixel *pPixel;
	for(int row = 0; row < pBmp->infoHeader.height / 2; row++) {
		pPixel = pBmp->pixel[row];
		pBmp->pixel[row] = pBmp->pixel[(pBmp->infoHeader.height - 1)- row];
		pBmp->pixel[(pBmp->infoHeader.height - 1)- row] = pPixel;
	}
}
