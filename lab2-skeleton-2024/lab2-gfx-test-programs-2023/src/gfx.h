/* We assume no standard library access */
typedef unsigned char _u8;
typedef unsigned int  _u32;

enum display_modes{
  GFX_Y8 = 0,
  GFX_INDEXED,
  GFX_RGB332,
  GFX_RGB555,
  GFX_RGB24,
  GFX_RGBA32
};

/* Helps control the framebuffer */
struct ControlInterface{
  _u32 enable;
  _u32 mode;
  _u32 resx;
  _u32 resy;
};

/* Useful addresses for MMIO */
#define CONTROL_ADDR 0x800LU
#define PALETTE_ADDR (CONTROL_ADDR + sizeof(struct ControlInterface))
#define FB_ADDR      0x1000000LU

/* Passed to every function here for convenience */
typedef struct gfx_data{
  _u32 resx;
  _u32 resy;
  _u32 mode;
  volatile struct ControlInterface * control;
  volatile _u32 * palette;
  volatile _u8  * fb;
} gfx_context;

/* Initialise the framebuffer at a given resolution in a given mode */
static inline int gfx_init(gfx_context * ctx, _u32 resx, _u32 resy, _u32 mode){
  /* set up the context struct */
  ctx->resx = resx;
  ctx->resy = resy;
  ctx->mode = mode;
  ctx->control = (volatile struct ControlInterface *)CONTROL_ADDR;
  ctx->palette = (volatile _u32 *)PALETTE_ADDR;
  ctx->fb      = (volatile _u8 *)FB_ADDR;

  /* Actually initialise the framebuffer */
  ctx->control->mode = mode;
  ctx->control->resx = resx;
  ctx->control->resy = resy;
  ctx->control->enable = 1;
  if (ctx->control->enable == 0)
    return -1;
  return 0;
}

static inline void gfx_quit(gfx_context * ctx){
  ctx->control->enable = 0;
}

/* Set a colour in the palette based on three components */
static inline void set_palette(gfx_context * ctx, const _u8 ind,
                               const _u8 r, const _u8 g, const _u8 b){
  ctx->palette[ind] = r << 24 | g << 16 | b << 8;
}

/* Setting single byte balues for pixels (Y8/Indexed mode) */
static inline void set_pixel(gfx_context * ctx, const _u32 x, const _u32 y,
                             const _u8 v){
  if (ctx->mode > GFX_RGB332)
    return;
  if (x > ctx->resx || y > ctx->resy)
    return;
  ctx->fb[y * ctx->resx + x] = v;
}

/* Set two bytes for a pixel (RGB555) */
static inline void set_pixel_short(gfx_context * ctx, const _u32 x, const _u32 y,
                                   const unsigned short v){
  if (ctx->mode != GFX_RGB555)
    return;
  if (x > ctx->resx || y > ctx->resy)
    return;
  _u32 ind = (y * ctx->resx + x) * 2;
  *(unsigned short *)&ctx->fb[ind] = v;
}

/* Set rgb value for a pixel (RGB(A) modes) */
static inline void set_pixel_rgb(gfx_context * ctx, const _u32 x, const _u32 y,
                                 const _u8 r, const _u8 g, const _u8 b){
  if (ctx->mode < GFX_RGB24)
    return;
  if (x > ctx->resx || y > ctx->resy)
    return;
  _u32 ind = y * ctx->resx + x;
  switch(ctx->mode){
    case GFX_RGB24:
      ctx->fb[ind * 3] = r;
      ctx->fb[ind * 3 + 1] = g;
      ctx->fb[ind * 3 + 2] = b;
      break;
    case GFX_RGBA32:
      *(_u32*)&ctx->fb[ind * 4] = r << 24 | g << 16 | b << 8;
      break;
  }
}
