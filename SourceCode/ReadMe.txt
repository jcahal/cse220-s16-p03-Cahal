You are supposed to complete all functions in File.c, Bmp.c and Image.c.

In File.c, you need to fill:

	void FileClose(FILE *pStream)
	FILE *FileOpen(char *pFilename, char *pMode)
	int FileRead(FILE *pStream, void *pBlock, size_t pSize, size_t pCount)
	long FileSize(char *pFilename)
	int FileWrite(FILE *pStream, void *pBlock, size_t pSize, size_t pCount)

In Bmp.c, you need to fill:

	static int BmpCalcFileSize(int pWidth, int pHeight)
	static int BmpCalcPad(int pWidth) 
	tPixel **BmpPixelAlloc(int pWidth, int pHeight) 
	void BmpPixelFree(tPixel **pPixel, int pHeight)
	tError BmpRead(char *pFilename, tBmp *pBmp)
	tError BmpWrite(char *pFilename, tBmp *pBmp) 

Similarly, in Image.c:

	void ImageFlipHoriz(tBmp *pBmp) 
	void ImageRotRight(tBmp *pBmp)
	void ImageRotRightMult(tBmp *pBmp, int pTimes) 
	void ImageFlipVert(tBmp *pBmp)

Once you have done everything, use Makefile command to compile the program and test.

Here is how you test your program.

----------------------------------------------------------------------------------------------------------------
These four images are the base images.
----------------------------------------------------------------------------------------------------------------
testimg0.bmp   Width = 800, Height = 450, Padding = 0
testimg1.bmp   Width = 413, Height = 232, Padding = 1
testimg2.bmp   Width = 514, Height = 426, Padding = 2
testimg3.bmp   Width = 639, Height = 506, Padding = 3

----------------------------------------------------------------------------------------------------------------
These images were created by applying operations to the base images.
----------------------------------------------------------------------------------------------------------------
testimg0-fliph.bmp                testimg0.bmp with --fliph
testimg0-fliph-flipv.bmp          testimg0.bmp with --fliph then --flipv
testimg0-fliph-rotr2-flipv.bmp    testimg0.bmp with --fliph then --rotr 2 then --flipv
testimg0-fliph-rotr3.bmp          testimg0.bmp with --fliph then --rotr 3
testimg0-flipv.bmp                testimg0.bmp with --flipv
testimg0-flipv-fliph.bmp          testimg0.bmp with --flipv then --fliph
testimg0-flipv-rotr1-fliph.bmp    testimg0.bmp with --flipv then --rotr 1 then --fliph
testimg0-flipv-rotr3.bmp          testimg0.bmp with --flipv then --rotr 3
testimg0-rotr1.bmp                testimg0.bmp with --rotr 1
testimg0-rotr1-fliph.bmp          testimg0.bmp with --rotr 1 then --fliph
testimg0-rotr1-flipv.bmp          testimg0.bmp with --rotr 1 then --flipv
testimg0-rotr2.bmp                testimg0.bmp with --rotr 2
testimg0-rotr3.bmp                testimg0.bmp with --rotr 3

testimg1-fliph.bmp                testimg1.bmp with --fliph
testimg1-flipv.bmp                testimg1.bmp with --flipv
testimg1-rotr1.bmp                testimg1.bmp with --rotr 1
testimg1-rotr2.bmp                testimg1.bmp with --rotr 2
testimg1-rotr3.bmp                testimg1.bmp with --rotr 3

testimg2-fliph.bmp                testimg2.bmp with --fliph
testimg2-flipv.bmp                testimg2.bmp with --flipv
testimg2-rotr1.bmp                testimg2.bmp with --rotr 1
testimg2-rotr2.bmp                testimg2.bmp with --rotr 2
testimg2-rotr3.bmp                testimg2.bmp with --rotr 3

testimg3-fliph.bmp                testimg3.bmp with --fliph
testimg3-flipv.bmp                testimg3.bmp with --flipv
testimg3-rotr1.bmp                testimg3.bmp with --rotr 1
testimg3-rotr2.bmp                testimg3.bmp with --rotr 2
testimg3-rotr3.bmp                testimg3.bmp with --rotr 3

----------------------------------------------------------------------------------------------------------------
These images are either invalid or corrupted. See Section 4.4 in the lab project document.
----------------------------------------------------------------------------------------------------------------
invalid01.bmp    Validity Test 1: File size is 57 bytes which is too small to be a BMP.
invalid02.bmp    Validity Test 2: First signature byte in BMPHEADER struct is not 'B'.
invalid03.bmp    Validity Test 2: Second signature byte in BMPHEADER struct is not 'M'.
invalid04.bmp    Validity Test 2: File size in BMPHEADER struct is invalid.
invalid05.bmp    Validity Test 2: Reserved bytes in the BMPHEADER struct are not zero.
invalid06.bmp    Validity Test 2: Pixel array offset in the BMPHEADER struct is not 0x36.
invalid07.bmp    Validity Test 3: Size field in the BMPINFOHEADER struct is not 0x28.
invalid08.bmp    Validity Test 3: Color planes field in the BMPINFOHEADER struct is not 1.
invalid09.bmp    Validity Test 3: Bits per pixel field in the BMPINFOHEADER struct is not 0x18.

corrupted01.bmp  Corrupted Test 1: Size of the file on disk does not match the calculated size.
corrupted02.bmp  Corrupted Test 2: One or more padding bytes are nonzero.
