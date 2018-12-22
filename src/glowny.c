#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_BITMAP *image = NULL;
ALLEGRO_LOCKED_REGION *region = NULL;

extern long filter_image(unsigned char *output_buffer, unsigned char *input_buffer, int *kernel, int width, int height);

void init()
{
   al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_24_NO_ALPHA);
   if (!al_init())
   {
      al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      exit(1);
   }

   if (!al_init_image_addon())
   {
      al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      exit(1);
   }

   display = al_create_display(1920, 1080);

   if (!display)
   {
      al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      exit(1);
   }
}

void read_image_from_user()
{
   al_destroy_bitmap(image);
   image = NULL;
   char name[100];
   do
   {
      printf("Please give name of the image: ");
      scanf("%s", name);
      image = al_load_bitmap(name);
   } while (!image);
}


void end_program()
{
   al_destroy_display(display);
   al_destroy_bitmap(image);
   al_shutdown_image_addon();
}

void readKernel(int *kernel)
{
   int value;
   printf("Please give a values for a kernel starting with bottom left corner:\n");
   for (int i = 0; i < 9; ++i)
   {
      printf("Tile number %d: ", i + 1);
      scanf("%d", &kernel[i]);
   }
}

int main(int argc, char *argv[])
{
   init();

   char control[10];
   while (true)
   {
      read_image_from_user();
      int width = al_get_bitmap_width(image);
      int height = al_get_bitmap_height(image);
      int widthInBytes = width * 3;
      int kernel[9];
      unsigned char *input_buffer = (unsigned char *)malloc(height * widthInBytes);

      region = al_lock_bitmap(image, ALLEGRO_PIXEL_FORMAT_ANY_24_NO_ALPHA, ALLEGRO_LOCK_READWRITE);
      memcpy(input_buffer, ((unsigned char *)(region->data)) - (height - 1) * widthInBytes, height * widthInBytes);
      al_unlock_bitmap(image);

      while (true)
      {
         readKernel(kernel);
         region = al_lock_bitmap(image, ALLEGRO_PIXEL_FORMAT_ANY_24_NO_ALPHA, ALLEGRO_LOCK_READWRITE);

         filter_image((unsigned char *)(region->data) - 1, input_buffer + (height - 1) * widthInBytes - 1, kernel, width, height);
         al_unlock_bitmap(image);

         al_draw_bitmap(image, 0, 0, 0);
         al_flip_display();

         printf("Do you want to try another kernel?(y/n) ");
         scanf("%s", control);
         if (control[0] != 'y')
            break;
      }

      printf("Do you want to try another image?(y/n) ");
      scanf("%s", control);
      if (control[0] != 'y')
      {
         free(input_buffer);
         break;
      }
   }

   end_program();

   return 0;
}
// int clamp(int toClamp, int low, int high)
// {
//    if(toClamp < low)
//       return low;
//    if(toClamp > high)
//       return high;
//    return toClamp;
// }

// void filter(unsigned char* output_buffer, unsigned char* input_buffer, int* kernel, int width, int height)
// {
//    int widthInBytes = width * 3;
//    int c_height;
//    int c_width;
//    int accumulator;
//    int kernel_sum;
//    int i;
//    for(i = 0; i < 9; i++)
//       kernel_sum+=kernel[i];

//    for(c_height = 2; c_height < height; ++c_height)
//    {
//       for(c_width = 0; c_width < widthInBytes; ++c_width, --input_buffer, --output_buffer)
//       {
//          accumulator = 0;

//          accumulator = accumulator + (input_buffer[-widthInBytes + 3] * kernel[8]);
//          accumulator = accumulator + (input_buffer[-widthInBytes] * kernel[7]);
//          accumulator = accumulator + (input_buffer[-widthInBytes - 3] * kernel[6]);

//          accumulator = accumulator + (input_buffer[3] * kernel[5]);
//          accumulator = accumulator + (input_buffer[0] * kernel[4]);
//          accumulator = accumulator + (input_buffer[-3] * kernel[3]);

//          accumulator = accumulator + (input_buffer[widthInBytes + 3] * kernel[2]);
//          accumulator = accumulator + (input_buffer[widthInBytes] * kernel[1]);
//          accumulator = accumulator + (input_buffer[widthInBytes - 3] * kernel[0]);

//          output_buffer[0] = clamp(accumulator / kernel_sum, 0, 255);
//       }
//    }

// }
