// #include <stdio.h>
// #include <stdlib.h>
// #include <memory.h>
// #include <math.h>

// typedef struct
// {
// 	int width, height;
// 	unsigned char* pImg;
// 	int imgSize;
// 	unsigned long widthInBytes;
// } imgInfo;

// typedef struct
// {
// 	unsigned short bfType; 
// 	unsigned long  bfSize; 
// 	unsigned short bfReserved1; 
// 	unsigned short bfReserved2; 
// 	unsigned long  bfOffBits; 
// 	unsigned long  biSize; 
// 	unsigned long  biWidth; 
// 	unsigned long  biHeight; 
// 	short biPlanes; 
// 	short biBitCount; 
// 	unsigned long  biCompression; 
// 	unsigned long  biSizeImage; 
// 	long biXPelsPerMeter; 
// 	long biYPelsPerMeter; 
// 	unsigned long  biClrUsed; 
// 	unsigned long  biClrImportant;
// }  __attribute__((packed)) bmpHdr; //for header to be exact 54 bytes

// void* freeResources(FILE* pFile, void* pFirst, void* pSnd)
// {
// 	if (pFile != 0)
// 		fclose(pFile);
// 	if (pFirst != 0)
// 		free(pFirst);
// 	if (pSnd !=0)
// 		free(pSnd);
// 	return 0;
// }

// imgInfo* InitImage (int w, int h)
// {
// 	imgInfo *pImg;
// 	if ( (pImg = (imgInfo *) malloc(sizeof(imgInfo))) == 0)
// 		return 0;
// 	pImg->height = h;
// 	pImg->width = w;
// 	pImg->widthInBytes = w * 3  + (w * 3) % 4;
// 	pImg->pImg = (unsigned char*) malloc(pImg->widthInBytes * h);
// 	if (pImg->pImg == 0)
// 	{
// 		free(pImg);
// 		return 0;
// 	}
// 	memset(pImg->pImg, 0, widthInBytes* h);
// 	return pImg;
// }

// imgInfo * copyImage(const imgInfo* pImg)
// {
// 	imgInfo *pNew = InitImage(pImg->width, pImg->height);
// 	if (pNew != 0)
// 		memcpy(pNew->pImg, pImg->pImg, pNew->imgSize);
// 	return pNew;
// }

// void FreeImage(imgInfo* pInfo)
// {
// 	if (pInfo && pInfo->pImg)
// 		free(pInfo->pImg);
// 	if (pInfo)
// 		free(pInfo);
// }


// imgInfo* readBMP(const char* fname)
// {
// 	imgInfo* pInfo = 0;
// 	FILE* fbmp = 0;
// 	bmpHdr bmpHead;
// 	int lineBytes, y;
// 	unsigned char* ptr;

// 	pInfo = 0;
// 	fbmp = fopen(fname, "rb");
// 	if (fbmp == 0){
// 		printf("Couldn't open a file\n");
// 		return 0;
// 	}
// 	fread((void *) &bmpHead, sizeof(bmpHead), 1, fbmp);
// 	// parê sprawdzeñ
// 	if (bmpHead.bfType != 0x4D42 || bmpHead.biPlanes != 1 ||
// 		bmpHead.biBitCount != 24 || 
// 		(pInfo = (imgInfo *) malloc(sizeof(imgInfo))) == 0)
// 		return (imgInfo*) freeResources(fbmp, pInfo ? pInfo->pImg : 0, pInfo);

// 	pInfo->width = bmpHead.biWidth;
// 	pInfo->height = bmpHead.biHeight;
// 	if ((pInfo->pImg = (unsigned char*) malloc(bmpHead.biSizeImage)) == 0)
// 		return (imgInfo*) freeResources(fbmp, pInfo->pImg, pInfo);

// 	ptr = pInfo->pImg;
// 	pInfo->widthInBytes = (pInfo->width * 3) + ((pInfo->width * 3) % 4); // linde width in bytes

// 	// moving pointer to begining of picture
// 	if (fseek(fbmp, bmpHead.bfOffBits, SEEK_SET) != 0)
// 		return (imgInfo*) freeResources(fbmp, pInfo->pImg, pInfo);

// 	for (y=0; y<pInfo->height; ++y)
// 	{
// 		fread(ptr, 1, pInfo->widthInBytes, fbmp);
// 		ptr += pInfo->widthInBytes;
// 	}
// 	fclose(fbmp);
// 	return pInfo;
// }

// void putPixel(imgInfo *pInfo, int x, int y, int *rgb)
// {
// 	int lineBytes = pInfo->widthInBytes;
// 	unsigned char *pAddr = pInfo->pImg + y * lineBytes + x * 3;
// 	*pAddr = rgb[0] >> 16;
// 	*(pAddr+1) = rgb[1] >> 16;
// 	*(pAddr+2) = rgb[2] >> 16;
// }


// int main(int argc, char* argv[])
// {

// 	imgInfo* pInfo = readBMP("te32st.bmp");
// 	if (sizeof(bmpHdr) != 54)
// 	{
// 		printf("You need to change compile options to header be 54 bytes.\n");
// 		return 1;
// 	}



// 	FreeImage(pInfo);
// 	return 0;
// }

