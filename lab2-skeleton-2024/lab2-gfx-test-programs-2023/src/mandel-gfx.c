/* Roughly based on the pseudo code on Wikipedia:
 *   https://en.wikipedia.org/wiki/Mandelbrot_set#Escape_time_algorithm
 * Ported to minimal C, modified to use only single precision, added
 * code to support outputting to the rv64-emu framebuffer
 */

#include "gfx.h"

/* Window resolution, remember every pixel takes a bunch of time to draw */
#define XRES 350
#define YRES 200

/* speed up drawing by limiting the amount of iterations (adds banding) */
#define IT_SHIFT 3
#define PIXEL_MAX 256
#define MAXIT (PIXEL_MAX >> IT_SHIFT)

/* Convert an iteration count to a pixel value, we invert the palette here */
#define IT_TO_PIX(i) ((MAXIT - i) << IT_SHIFT)

/* The window of the [-2.5:1,-1:1] mandelbrot we draw */
#define XS -2.5f
#define XE  1.f
#define YS -1.f
#define YE  1.f

/* Total range of the window we draw */
#define XT (XE - XS)
#define YT (YE - YS)

/* float increment per pixel in either direction */
#define XI (XT/XRES)
#define YI (YT/YRES)

int main(){
  gfx_context c;
  /* Y8 is 8-bit greyscale mode */
  gfx_init(&c, XRES, YRES, GFX_Y8);

  float cx = XS, cy = YS; //Current x/y position in the set
  for (_u32   py = 0; py < YRES; py++, cy+=YI, cx=XS)
    for (_u32 px = 0; px < XRES; px++, cx+=XI){
      int it = 0;
      float x,y,rs = 0.0f, is = 0.0f, zs = 0.0f;
      for (; rs + is <= 4.f && it < MAXIT; it++){
        x = rs - is + cx;
        y = zs - rs - is + cy;
        rs = x * x;
        is = y * y;
        zs = (x + y) * (x + y);
      }
      set_pixel(&c, px, py, IT_TO_PIX(it));
    }
  return 0;
}
