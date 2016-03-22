/***************************************************************************************************************
 * FILE: Image.c
 *
 * DESCRIPTION
 * See comments in Image.h.
 *
 * CSE 220
 **************************************************************************************************************/
#include "Bmp.h"

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
	tPixel pCol[pBmp->infoHeader.height];

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
void ImageRotRightMult(tBmp *pBmp, int nTimes)
{

}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: ImageFlipVert()
 *------------------------------------------------------------------------------------------------------------*/
void ImageFlipVert(tBmp *pBmp)
{
	tPixel *pPixel;
	for(int row = 0; row < pBmp->infoHeader.height / 2; row++) {
		pPixel = pBmp->pixel[row];
		pBmp->pixel[row] = pBmp->pixel[pBmp->infoHeader.height - row];
		pBmp->pixel[pBmp->infoHeader.height - row] = pPixel;
	}
}
