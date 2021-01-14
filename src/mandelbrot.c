// This code creates the image to the right >>
//
// (c) 2020 Mike Field <hamster@snap.net.nz>

#include <stdio.h>

const int max_loops = 1024;

double center_r     = -0.83;
double center_i     = 0.19;
double scale        = 0.01;

int main(int argc, char *argv[])
{
   int width  = 4660;
   int height = 4660;
   FILE *file;

   if(argc != 2) {
      printf("You need to supply the file name\n");
      return -1;
   }

   printf("Creating file %s\n", argv[1]);
 
   file = fopen(argv[1],"w");
   if(file == NULL) {
      printf("Unable to open output image file\n");
      return -1;
   }

   fprintf(file, "P6\n");
   fprintf(file, "%i %i\n",width, height);
   fprintf(file, "255\n");

   int i;
   for(i = 0; i < height; i++ ) {
      int j;
      for(j = 0; j < width; j++) {
         double c_r = center_r + scale/width*(j - width/2);
         double c_i = center_i + scale/width*(i - height/2);
         double a_r = 0.0;

         double a_i = 0.0;
         int    loops = 1;
         while((a_r*a_r+a_i*a_i < 4.0) && loops < max_loops) {
            double t_r = (a_r*a_r - a_i * a_i);
            double t_i = (2 * a_r * a_i);
            a_r = t_r + c_r;
            a_i = t_i + c_i;
            loops++;
         }
         if(loops == max_loops)
             loops = 0;
               
         putc(loops,      file);
         putc(loops << 3, file);
         putc(loops << 6, file);
      }
   }
   fclose(file);
   return 0;
}
