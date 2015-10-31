#ifndef MRUBY_SDL2_GPU_H
#define MRUBY_SDL2_GPU_H

#include "sdl2.h"

extern struct RClass *mod_GPU;
#define SDL_GPU_NEW_IMAGE_FROM_STRING_BUG


extern GPU_Target * mrb_sdl2_gpu_target_get_ptr(mrb_state *mrb, mrb_value target);
extern mrb_value mrb_sdl2_gpu_target(mrb_state *mrb, GPU_Target *target);

extern GPU_Image * mrb_sdl2_gpu_image_get_ptr(mrb_state *mrb, mrb_value image);
extern mrb_value mrb_sdl2_gpu_image(mrb_state *mrb, GPU_Image *image);

GPU_Rect *mrb_sdl2_gpu_rect_get_ptr(mrb_state *mrb, mrb_value rect);
mrb_value mrb_sdl2_gpu_rect(mrb_state *mrb, GPU_Rect rect);
#endif /* end of MRUBY_SDL2_GPU_H */
