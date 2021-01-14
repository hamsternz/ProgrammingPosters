// This code creates the image to the right >>
//
// (c) 2021 Mike Field <hamster@snap.net.nz>
// Based on work by Zodiam on Discord

#include <stdio.h>
#include <math.h>

#define WIDTH  4660
#define HEIGHT 4660

unsigned char frame[HEIGHT][WIDTH][3] = { 0 };
double        z_buffer[HEIGHT][WIDTH] = { 0 };

const double A  = -0.5, B  = 0.4;   // Angles of torus
const double R1 =  0.4, R2 = 1.0;   // Torus parameters
const double depth = 5.0;           // Depth offset

typedef struct {
   double x, y, z;
} Point ;

typedef struct {
   double xx, xy, xz;
   double yx, yy, yz;
   double zx, zy, zz;
} Rotation ;

void rotate(Point *out,  Point *in,  Rotation *r ) {
   out->x = in->x * r->xx + in->y * r->xy + in->z * r->xz;
   out->y = in->x * r->yx + in->y * r->yy + in->z * r->yz;
   out->z = in->x * r->zx + in->y * r->zy + in->z * r->zz;
}

int main(int argc, char *argv[]) {
   if(argc != 2) {
       fprintf(stderr, "No file name supplied\n");
       return 1;
   }

   for ( int j = 0; j < HEIGHT; j++ )
       for ( int i = 0; i < WIDTH; i++ )
           z_buffer[ j ][ i ] = -10.0;

   Rotation rz = {  cos(B),  sin(B),      0,
                   -sin(B),  cos(B),      0,
                         0,       0,    1.0};
   Rotation rx = {     1.0,       0,      0,
                         0,  cos(A), sin(A),
                         0, -sin(A), cos(A)};

   for ( int i = 0.0; i < WIDTH; i++) {
      for (int j = 0.0; j < WIDTH*2; j++) {
         Point temp1, temp2, rotated, normal;
         double theta = i*2*M_PI/WIDTH, phi = j*M_PI/WIDTH;
         Point circle    = {R2+R1*cos(theta), R1*sin(theta), 0};
         Point fornormal = {      cos(theta),    sin(theta), 0};

         Rotation ry = {  cos(phi),   0.0, sin(phi),
                               0.0,   1.0,      0.0,
                         -sin(phi),   0.0, cos(phi)};

         rotate(&temp1,   &circle, &ry);
         rotate(&temp2,   &temp1,  &rx);
         rotate(&rotated, &temp2,  &rz);
         rotated.z -= depth;

         int xp = (int) (rotated.x/rotated.z  *WIDTH) + WIDTH/2;
         int yp = (int) (rotated.y/rotated.z  *WIDTH) + HEIGHT/2;
         if(yp < 0 || yp > HEIGHT -1 || xp < 0 || xp > WIDTH-1)
            continue;

         rotate(&temp1,  &fornormal, &ry);
         rotate(&temp2,  &temp1,     &rx);
         rotate(&normal, &temp2,     &rz);
         if ( rotated.z > z_buffer[ yp ][ xp ] ){
             double luminance = (normal.y-normal.x)/sqrt(2.0);
             frame[yp][xp][0] = (luminance > 0 ? luminance*220+32 : 32);
             luminance = normal.y; 
             frame[yp][xp][1] = (luminance > 0 ? luminance*220+32 : 32);
             luminance = (normal.y+normal.x)/sqrt(2.0);
             frame[yp][xp][2] = (luminance > 0 ? luminance*220+32 : 32);
             z_buffer[ yp ][ xp ] = rotated.z;
         }
      }
   }

   FILE *f = fopen(argv[1],"wb");
   if(f != NULL) {
      fprintf(f, "P6\n%i %i\n255\n", WIDTH, HEIGHT);
      fwrite(frame, sizeof(frame), 1, f);
      fclose(f);
   }
}
