#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>

/*
// typedef struct
// {
// 	int width, height;
// 	unsigned char *pImg;
// 	int imgSize;
// 	unsigned int widthInBytes;
// } imgInfo;

// typedef struct
// {
// 	unsigned short bfType;
// 	unsigned int bfSize;
// 	unsigned short bfReserved1;
// 	unsigned short bfReserved2;
// 	unsigned int bfOffBits;
// 	unsigned int biSize;
// 	unsigned int biWidth;
// 	unsigned int biHeight;
// 	short biPlanes;
// 	short biBitCount;
// 	unsigned int biCompression;
// 	unsigned int biSizeImage;
// 	int biXPelsPerMeter;
// 	int biYPelsPerMeter;
// 	unsigned int biClrUsed;
// 	unsigned int biClrImportant;
// } __attribute__((packed)) bmpHdr; //for header to be exact 54 bytes

// void *freeResources(FILE *pFile, void *pFirst, void *pSnd)
// {
// 	if (pFile != 0)
// 		fclose(pFile);
// 	if (pFirst != 0)
// 		free(pFirst);
// 	if (pSnd != 0)
// 		free(pSnd);
// 	return 0;
// }

// imgInfo *InitImage(int w, int h)
// {
// 	imgInfo *pImg;
// 	if ((pImg = (imgInfo *)malloc(sizeof(imgInfo))) == 0)
// 		return 0;
// 	pImg->height = h;
// 	pImg->width = w;
// 	pImg->widthInBytes = w * 3 + (w * 3) % 4;
// 	pImg->pImg = (unsigned char *)malloc(pImg->widthInBytes * h);
// 	if (pImg->pImg == 0)
// 	{
// 		free(pImg);
// 		return 0;
// 	}
// 	memset(pImg->pImg, 0, pImg->widthInBytes * h);
// 	return pImg;
// }

// imgInfo *copyImage(const imgInfo *pImg)
// {
// 	imgInfo *pNew = InitImage(pImg->width, pImg->height);
// 	if (pNew != 0)
// 		memcpy(pNew->pImg, pImg->pImg, pNew->imgSize);
// 	return pNew;
// }

// void FreeImage(imgInfo *pInfo)
// {
// 	if (pInfo && pInfo->pImg)
// 		free(pInfo->pImg);
// 	if (pInfo)
// 		free(pInfo);
// }

// imgInfo *readBMP(const char *fname)
// {
// 	imgInfo *pInfo = 0;
// 	FILE *fbmp = 0;
// 	bmpHdr bmpHead;
// 	int lineBytes, y;
// 	unsigned char *ptr;

// 	pInfo = 0;
// 	fbmp = fopen(fname, "rb");
// 	if (fbmp == 0)
// 	{
// 		printf("Couldn't open a file\n");
// 		return 0;
// 	}
// 	fread((void *)&bmpHead, sizeof(bmpHead), 1, fbmp);
// 	// parê sprawdzeñ
// 	if (bmpHead.bfType != 0x4D42 || bmpHead.biPlanes != 1 ||
// 		bmpHead.biBitCount != 24 ||
// 		(pInfo = (imgInfo *)malloc(sizeof(imgInfo))) == 0)
// 		return (imgInfo *)freeResources(fbmp, pInfo ? pInfo->pImg : 0, pInfo);

// 	pInfo->width = bmpHead.biWidth;
// 	pInfo->height = bmpHead.biHeight;
// 	if ((pInfo->pImg = (unsigned char *)malloc(bmpHead.biSizeImage)) == 0)
// 		return (imgInfo *)freeResources(fbmp, pInfo->pImg, pInfo);

// 	ptr = pInfo->pImg;
// 	pInfo->widthInBytes = (pInfo->width * 3) + ((pInfo->width * 3) % 4); // linde width in bytes

// 	// moving pointer to begining of picture
// 	if (fseek(fbmp, bmpHead.bfOffBits, SEEK_SET) != 0)
// 		return (imgInfo *)freeResources(fbmp, pInfo->pImg, pInfo);

// 	for (y = 0; y < pInfo->height; ++y)
// 	{
// 		fread(ptr, 1, pInfo->widthInBytes, fbmp);
// 		ptr += pInfo->widthInBytes;
// 	}
// 	fclose(fbmp);
// 	return pInfo;
// }

*/

extern long add_numbers(unsigned char* output_buffer, unsigned char* input_buffer, int* kernel, int width, int height);

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_BITMAP  *image   = NULL;
ALLEGRO_LOCKED_REGION * region = NULL;

void init()
{
  	al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_24_NO_ALPHA);
   if(!al_init()) {
      al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!", 
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      exit(1);
   }

   if(!al_init_image_addon()) {
      al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!", 
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      exit(1);
   }

   display = al_create_display(1920,1080);

   if(!display) {
      al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!", 
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      exit(1);
   }

   image = al_load_bitmap("ja.bmp");

   if(!image) {
      al_show_native_message_box(display, "Error", "Error", "Failed to load image!", 
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      al_destroy_display(display);
      exit(1);
   }
}

void init_input_buffer(unsigned char* input_buffer, int height, int widthInBytes)
{
   input_buffer = malloc(height * widthInBytes);
   region=al_lock_bitmap(image, ALLEGRO_PIXEL_FORMAT_ANY_24_NO_ALPHA, ALLEGRO_LOCK_READWRITE);
   memcpy(input_buffer, ((unsigned char*)(region->data)) - (height - 1 ) * widthInBytes, height * widthInBytes);
   al_unlock_bitmap(image);
}

void end_program()
{

	al_destroy_display(display);
	al_destroy_bitmap(image);
	al_shutdown_image_addon();
}

void filter(unsigned char* output_buffer, unsigned char* input_buffer, int kernel[], int width, int height);

int main(int argc, char *argv[])
{
	init();
	int width=al_get_bitmap_width(image);
   int height=al_get_bitmap_height(image);
   int widthInBytes = width * 3;

   unsigned char* input_buffer;
   input_buffer = malloc(height * widthInBytes);
   region=al_lock_bitmap(image, ALLEGRO_PIXEL_FORMAT_ANY_24_NO_ALPHA, ALLEGRO_LOCK_READWRITE);
   memcpy(input_buffer, ((unsigned char*)(region->data)) - (height - 1 ) * widthInBytes, height * widthInBytes);

   // int kernel[9] = { -1,-1,-1,-1,9,-1,-1,-1,-1}; 
   int kernel[9] = { 1,1,1,1,1,1,1,1,1}; 
   // int kernel[9] = { 1,2,1,2,-11,2,1,2,1}; 

   int x = sizeof(input_buffer);
   int z = input_buffer[(height - 200)* widthInBytes];
   // add_numbers((unsigned char*)(region->data) - 1,  input_buffer + (height - 1)* widthInBytes - 1, kernel, width, height); 
   printf("%ld\n",add_numbers((unsigned char*)(region->data) - 1,  input_buffer + (height - 1)* widthInBytes - 1, kernel, width, height));
   
	al_unlock_bitmap(image);
   al_save_bitmap("result.bmp", image);
	al_draw_bitmap(image, 0, 0,0);
	al_flip_display();

   al_rest(3);
   end_program();

	return 0;
}
int clamp(int toClamp, int low, int high)
{
   if(toClamp < low)
      return low;
   if(toClamp > high)
      return high;
   return toClamp;
}

void filter(unsigned char* output_buffer, unsigned char* input_buffer, int* kernel, int width, int height)
{
   int widthInBytes = width * 3;
   int c_height;
   int c_width;
   int accumulator;
   int kernel_sum;
   int i;
   for(i = 0; i < 9; i++)
      kernel_sum+=kernel[i];

   for(c_height = 2; c_height < height; ++c_height)
   {
      for(c_width = 0; c_width < widthInBytes; ++c_width, --input_buffer, --output_buffer)
      {
         accumulator = 0;
         
         accumulator = accumulator + (input_buffer[-widthInBytes + 3] * kernel[8]);
         accumulator = accumulator + (input_buffer[-widthInBytes] * kernel[7]);
         accumulator = accumulator + (input_buffer[-widthInBytes - 3] * kernel[6]);

         accumulator = accumulator + (input_buffer[3] * kernel[5]);
         accumulator = accumulator + (input_buffer[0] * kernel[4]);
         accumulator = accumulator + (input_buffer[-3] * kernel[3]);

         accumulator = accumulator + (input_buffer[widthInBytes + 3] * kernel[2]);
         accumulator = accumulator + (input_buffer[widthInBytes] * kernel[1]);
         accumulator = accumulator + (input_buffer[widthInBytes - 3] * kernel[0]);

         output_buffer[0] = clamp(accumulator / kernel_sum, 0, 255);
      }
   }

}
