#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <stdint.h>

const char *keywords[] = {
   "auto",     "break",    "case",     "char",
   "const",    "continue", "default",  "double",
   "do",       "else",     "enum",     "extern",
   "float",    "for",      "goto",     "if",
   "int",      "long",     "register", "return",
   "short",    "signed",   "sizeof",   "static",
   "struct",   "switch",   "typedef",  "union",
   "unsigned", "void",     "volatile", "while",
   NULL};

struct image {
   int width;
   int height;
   struct image *font;
   int x,y;
   uint8_t r,g,b;
   uint8_t **data;
};
typedef struct image image_t;

image_t *image_new(int w, int h) {
   image_t *img;
   int i;

   img = malloc(sizeof(image_t));
   if(img == NULL)
      return NULL;

   img->width  = w;
   img->height = h;

   img->x = 0;
   img->y = 0;

   img->r = 0;
   img->g = 0;
   img->b = 0;

   img->font = NULL;
   img->data = malloc(sizeof(char *)*h);
   if(img->data == NULL) {
      free(img);
      return NULL;
   }
 
   memset(img->data,0,sizeof(char *)*h);

   for(i = 0; i < h; i++) {
      img->data[i] = malloc(w*3);
      if(img->data[i] == NULL)
         break;
      memset(img->data[i],0,w*3);
   }
   if(i != h) {
      for(i = 0; i < h; i++) {
         if(img->data[i] != NULL) {
            free(img->data[i]);
         }
      }
      free(img->data);
      free(img);
      return NULL;
   }
   return img;
}

void image_set_font(image_t *img, image_t *font) {
   img->font = font;
}

void image_set_pos(image_t *img, int x, int y) {
   img->x = x;
   img->y = y;
}

void image_set_colour(image_t *img, uint8_t r, uint8_t g, uint8_t b) {
   img->r = r;
   img->g = g;
   img->b = b;
}

void image_merge(image_t *img, image_t *img2, int x, int y, int w, int h) {
   int i,j;
   if(w > img2->width)  w = img2->width;
   if(h > img2->height) h = img2->height;
   if(y+h <= 0)        return;
   if(x+w <= 0)        return;
   if(x > img->width)  return; 
   if(y > img->height) return; 

   if(x < 0) {
      w += x;
      x = 0;
   }
   if(x+w > img->width) {
     w = img->width -x;
   }
   if(y+h > img->height) {
     h = img->height -y;
   }

   if(y < 0) {
      h += y;
      y = 0;
   }

   for(j = 0; j < h; j++) {
      for(i = 0; i < w && i < img2->width; i++) {
         img->data[j+y][(x+i)*3+0] = img2->data[j][i*3+0];
         img->data[j+y][(x+i)*3+1] = img2->data[j][i*3+1];
         img->data[j+y][(x+i)*3+2] = img2->data[j][i*3+2];
      }
   }
}

void image_rectangle(image_t *img, int x, int y, int w, int h) {
   int i,j;

   if(y+h <= 0) return;
   if(x+w <= 0) return;
   if(x > img->width) return; 
   if(y > img->height) return; 

   if(x < 0) {
      w += x;
      x = 0;
   }
   if(x+w > img->width) {
     w = img->width -x;
   }
   if(y+h > img->height) {
     h = img->height -y;
   }

   if(y < 0) {
      h += y;
      y = 0;
   }

   for(j = 0; j < h; j++) {
      for(i = 0; i < w; i++) {
         img->data[j+y][(x+i)*3+0] = img->r;
         img->data[j+y][(x+i)*3+1] = img->g;
         img->data[j+y][(x+i)*3+2] = img->b;
      }
   }
}

int image_write(image_t *img, char *fname) {
  int i;
  FILE *f;

  if(img == NULL) {
     return 0;
  }

  if(img->data == NULL) {
     return 0;
  }

  for(i = 0;i < img->height; i++) {
     if(img->data[i] == NULL) {
        return 0;
     }
  }

  f = fopen(fname,"w");
  if(f == NULL) {
     return 0;
  }

  fprintf(f,"P6\n%i %i\n255\n", img->width, img->height);
  for(i = 0;i < img->height; i++) {
     if(fwrite(img->data[i],3,img->width,f) != img->width) {
        fclose(f);
        return 0;
     }
  }

  fclose(f);
  return 1;
}
void image_free(image_t *img) {
   int i;
   if(img->data != NULL) {
      for(i = 0; i < img->height; i++) {
         if(img->data[i] != NULL) {
            free(img->data[i]);
         }
      }
      free(img->data);
   }
}

static int whitespace(char c) {
  if(c == ' ') return 1;
  if(c == '\t') return 1;
  if(c == '\r') return 1;
  if(c == '\n') return 1;
  return 0;
}

static int digit(char c) {
   return (c >= '0' && c <= '9');
}

image_t *image_from_ppm(char *file_name) {
   FILE *file;
   image_t *img;
   int c, last_c = 0;
   int width = 0, height = 0, maxval = 0;

   file = fopen(file_name, "r");
   if(file == NULL) {
     fprintf(stderr,"File %s not able to be opened\n", file_name);
     return 0;
   }

   c = getc(file);
   if(c != 'P') {
     goto format_error;
   }

   c = getc(file);
   if(c != '6') {
     goto format_error;
   }

   c = getc(file);
   while(whitespace(c)) {
     last_c = c;
     c = getc(file);
     if(c == '#' && last_c == '\n') {
       c = getc(file);
       while(c != '\n' && c != EOF) {
         c = getc(file);
       }
     }
   }

   if(!digit(c)) {
     goto format_error;
   }

   while(digit(c)) {
     width = (width*10)+(c-'0');
     c = getc(file);
   }

   while(whitespace(c)) {
     last_c = c;
     c = getc(file);
     if(c == '#' && last_c == '\n') {
       c = getc(file);
       while(c != '\n' && c != EOF) {
         c = getc(file);
       }
     }
   }

   if(!digit(c)) {
     goto format_error;
   }

   while(digit(c)) {
     height = (height*10)+(c-'0');
     c = getc(file);
   }

   while(whitespace(c)) {
     last_c = c;
     c = getc(file);
     if(c == '#' && last_c == '\n') {
       c = getc(file);
       while(c != '\n' && c != EOF) {
         c = getc(file);
       }
     }
   }

   if(!digit(c)) {
     goto format_error;
   }

   while(digit(c)) {
     maxval = (maxval*10)+(c-'0');
     c = getc(file);
   }

   if(!whitespace(c)) {
     goto format_error;
   }
   
   if(maxval != 255) {
     goto format_error;
   }
   img = image_new(width,height);

   if(img == NULL) {
     goto img_error;
   }
   for(int i = 0; i < height; i++) {
      if(fread(img->data[i],3, width,file) != width) {
         goto read_error;
      }
   }
   fclose(file);
   return img;

read_error:
   image_free(img);
   fprintf(stderr,"Error reading data in %s\n", file_name);
   fclose(file);
   return 0;

img_error:
   fprintf(stderr,"Unable to create image %s\n", file_name);
   fclose(file);
   return NULL;

format_error:
   fprintf(stderr,"File format error\n"); 
   fclose(file);
   return 0;
}

int char_write(image_t *img, char c) {
   int char_width, char_height;
   int fx, fy, dx, dy;

   if(img->font == NULL) {
      return 1;
   }
   char_width  = img->font->width/16;
   char_height = img->font->height/6;
   // Test to see if off the page //
   if(img->y >= img->height || img->x >= img->width || img->y <= -char_height || img->x <= -char_width) {
      return 1;
   }
   if(c < 32) c = ' ';
   if(c >= 127) c = ' ';

   c -=32;
   fx = (c%16)*char_width;
   fy = (c/16)*char_height;

   for(dy = 0; dy < img->font->height/6 && img->y+dy < img->height; dy++) {
      if(img->y+dy >= 0) {
         int copy_w = char_width;
         int offset = 0;
     
         if(img->x+copy_w > img->width) {
            copy_w = img->width - img->x;
         }
         if(img->x < 0) { 
            offset = -img->x;
            copy_w -= offset;
         }
         for(dx = 0; dx < copy_w; dx++) {
           uint8_t mix_r = img->font->data[fy+dy][3*(fx+offset+dx)+0];
           uint8_t mix_g = img->font->data[fy+dy][3*(fx+offset+dx)+1];
           uint8_t mix_b = img->font->data[fy+dy][3*(fx+offset+dx)+2];
           int tx = img->x+offset+dx;
           img->data[img->y+dy][3*tx+0] = (img->data[img->y+dy][3*tx+0] * mix_r + img->r * (255-mix_r))/255;
           img->data[img->y+dy][3*tx+1] = (img->data[img->y+dy][3*tx+1] * mix_g + img->g * (255-mix_g))/255;
           img->data[img->y+dy][3*tx+2] = (img->data[img->y+dy][3*tx+2] * mix_b + img->b * (255-mix_b))/255;
         }
      }
   }
   return 1;
}

int syntax_highlight(char *text, char *colour_table) {
  while(*text) {
    if((*text >= 'a' && *text <= 'z') || (*text >= 'A' && *text <= 'Z') || *text == '_') {
       int i;
       for(i = 0; keywords[i] != NULL; i++) {
         int len = strlen(keywords[i]);
         if(memcmp(keywords[i], text, len) == 0)
           if(text[len] <= 'a' || text[len] >= 'z')
             break;
       } 
       if( keywords[i] == NULL) {
          while((*text >= 'a' && *text <= 'z') || (*text >= 'A' && *text <= 'Z') || *text == '_' || (*text >= '0' && *text <= '9')) {
             *colour_table = 2;
             text++;
             colour_table++;
          }
       } else {
          int len = strlen(keywords[i]);
          memset(colour_table,4,len);
          text+= len;
          colour_table+=len;
       }
    } else if(*text >= '0' && *text <= '9') {
       while((*text >= '0' && *text <= '9') || *text == '.') {
         *colour_table = 6;
         text++;
         colour_table++;
       }
    } else {
      switch(*text) {
        case '"': 
           *colour_table = 3;
           text++; 
           colour_table++;
           while(*text != '"' && *text != '\0') {
             *colour_table = 3;
             text++; 
             colour_table++;
           } 
           if(*text == '"') {
             *colour_table = 3;
             text++; 
             colour_table++;
           }
           break;

        case '/': 
           if(text[1] == '/') {
             while(*text != '\n' && *text != '\0') {
               *colour_table = 5;
               text++;
               colour_table++;
             }
           } else {
             *colour_table = 0;
             text++;
             colour_table++;
           }
           break;
        case '#': 
           while(*text != '\n' && *text != '\0') {
             *colour_table = 1;
             text++;
             colour_table++;
           }
           break;
        default: 
           text++; colour_table++;
           break;
      }

    }
  }
  return 0;
}
int image_text(image_t *img, int x, int y, char *text, char *colour_table) {
   int cur_x = x;
   if(img->font == NULL) {
      return 0;
   }
   while(*text) {
      if(*text == '\n') {
         cur_x = x;
         y += img->font->height/6;
      } else {
        switch(colour_table[0]) {
           case  0: image_set_colour(img,   0,   0,   0);  break;  // Unknown
           case  1: image_set_colour(img, 128,   0,   0);  break;  // #defines
           case  2: image_set_colour(img,   0, 128,   0);  break;  // Keywords
           case  3: image_set_colour(img,   0,   0, 128);  break;  // Text    
           case  4: image_set_colour(img,   0,   0,   0);  break;  // Misc text
           case  5: image_set_colour(img,   0, 192, 192);  break;  // Comments
           case  6: image_set_colour(img, 192,   0, 192);  break;  // Numbers
           default: image_set_colour(img,   0,   0,   0);  break;
        }
        image_set_pos(img, cur_x, y);
        char_write(img, *text);
        cur_x += img->font->width/16;
      }
      text++;
      colour_table++;
   }
   return 1;
}


int main(int argc, char *argv[])
{
   char buffer[16*1024];
   char colour_table[16*1024];
   int width = 7016;
   int height = 4960;
   int boarder = 150;
   int lines = 0;
   if(argc != 4) {
      fprintf(stderr,"Usage: %s code_file.c picture_file.ppm output_file.ppm",argv[0]);
      return 0;
   }
   FILE *f = fopen(argv[1],"r");
   int n;
   if(f == NULL) {
      fprintf(stderr, "Unable to open source\n");
      return 0;
   }
   n = fread(buffer,1,sizeof(buffer)-1,f);
   if(n < 1) {
      fprintf(stderr, "Unable to read source\n");
      fclose(f);  
      return 0;
   }
   buffer[n] = 0;
   memset(colour_table,0,n);
   syntax_highlight(buffer, colour_table);

   for(int i = 0; buffer[i]; i++) {
      if(buffer[i] == '\n')
         lines++;
   }

   fclose(f);
   image_t *font = image_from_ppm("font/font_ML.ppm");
   image_t *layout;
   if(font == NULL) {
      fprintf(stderr, "Unable to read font\n");
      return 0;
   }
   printf("Font loaded - %i x %i characters\n", font->width/16, font->height/6);

   layout = image_new(width, height);
   if(layout == NULL) {
      image_free(font);
      return 0;
   }

   image_set_colour(layout, 64, 64, 64);
   image_rectangle(layout, 0, 0, width, height);

   image_set_colour(layout, 255, 255, 255);
   image_rectangle(layout, boarder, boarder, width-height-boarder, height-boarder*2);

   image_set_colour(layout, 192,192,192);
   image_rectangle(layout, width-boarder-(height-boarder*2), boarder, height-boarder*2, height-boarder*2);

   // Black text
   image_set_colour(layout, 0, 0, 0);
   image_set_font(layout, font);
   image_text(layout, 3*boarder/2, height/2-(lines*font->height/6)/2, buffer, colour_table);

   image_t *art = image_from_ppm(argv[2]);
   if(art == NULL) {
      fprintf(stderr, "Unable to open merge image\n");
   } else {
      if(art->width != height-boarder*2 || art->height != height-boarder*2) {
        printf("Image should be %i x %i\n", height-boarder*2, height-boarder*2);
      }
      image_merge(layout, art, width-boarder-(height-boarder*2), boarder, height-boarder*2, height-boarder*2);
      image_free(art);
   }

   if(!image_write(layout, argv[3])) {
      fprintf(stderr,"Unable to write file\n");
   }
   image_free(layout);
   image_free(font);
}
