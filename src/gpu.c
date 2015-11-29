/*Copyright 2015 <Daniel Kolev>
  mruby-sdl-gpu is a gem for SDL_gpu.h
  info can be found at:
  * git source:   https://github.com/grimfang4/sdl-gpu 
  * doxygen docs: http://dinomage.com/reference/SDL_gpu/index.html
  */

// mruby related includes
#include <SDL/SDL_gpu.h>
#include <SDL2/SDL_image.h>
#include "../include/mruby.h"
#include "mruby/data.h"
#include "mruby/class.h"
#include "mruby/variable.h"
#include "mruby/array.h"
#include "mruby/string.h"

// mruby-sdl2 related includes
#include "../include/sdl2_surface.h"
#include "../include/sdl2_render.h"
#include "../include/sdl2_rwops.h"

#include "glew/GL/glew.h"
#include "../include/gpu.h"

struct RClass *mod_GPU               = NULL;
// classes in mruby
struct RClass *class_Target          = NULL;
struct RClass *class_RendererID      = NULL;
struct RClass *class_Renderer        = NULL;
struct RClass *class_Context         = NULL;
struct RClass *class_Camera          = NULL;
struct RClass *class_BlendMode       = NULL;
struct RClass *class_Rect            = NULL;
struct RClass *class_Image           = NULL;
struct RClass *class_MatrixStack     = NULL;
struct RClass *class_Shader          = NULL;
struct RClass *class_Program         = NULL;
struct RClass *class_ShaderBlock     = NULL;
struct RClass *class_Attribute       = NULL;
struct RClass *class_AttributeFormat = NULL;


/*********************************
 * GPU_Target bindings starts here
 *********************************/

typedef struct mrb_sdl2_gpu_target_data_t {
  GPU_Target *target;
} mrb_sdl2_gpu_target_data_t;

static void
mrb_sdl2_gpu_target_data_free(mrb_state *mrb, void *p) {
  mrb_sdl2_gpu_target_data_t *data =
    (mrb_sdl2_gpu_target_data_t*)p;
  if (NULL != data->target) {
    GPU_FreeTarget(data->target);
  }
  mrb_free(mrb, data);
}

static struct mrb_data_type const mrb_sdl2_gpu_target_data_type = {
  "Target", mrb_sdl2_gpu_target_data_free
};

GPU_Target *
mrb_sdl2_gpu_target_get_ptr(mrb_state *mrb, mrb_value target) {
  mrb_sdl2_gpu_target_data_t *data;
  if (mrb_nil_p(target)) {
    return NULL;
  }
  data =
    (mrb_sdl2_gpu_target_data_t*)
      mrb_data_get_ptr(mrb, target, &mrb_sdl2_gpu_target_data_type);
  return data->target;
}


mrb_value
mrb_sdl2_gpu_target(mrb_state *mrb, GPU_Target *target) {
  mrb_sdl2_gpu_target_data_t *data =
    (mrb_sdl2_gpu_target_data_t*)
      mrb_malloc(mrb, sizeof(mrb_sdl2_gpu_target_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->target = target;
  return mrb_obj_value(
  Data_Wrap_Struct(mrb, class_Target, &mrb_sdl2_gpu_target_data_type, data));
}
/*******************************
 * GPU_Target bindings ends here
 *******************************/

/*********************************
 * Shaders bindings starts here
 *********************************/

typedef struct mrb_sdl2_gpu_shader_data_t {
  Uint32 shaderid;
} mrb_sdl2_gpu_shader_data_t;

static void
mrb_sdl2_gpu_shader_data_free(mrb_state *mrb, void *p) {
  mrb_sdl2_gpu_shader_data_t *data =
    (mrb_sdl2_gpu_shader_data_t*)p;
  if (NULL != data) {
    if (0 != data->shaderid) {
      GPU_FreeShader(data->shaderid);
    }
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_gpu_shader_data_type = {
  "Shader", mrb_sdl2_gpu_shader_data_free
};

Uint32
mrb_sdl2_gpu_shader_get_uint32(mrb_state *mrb, mrb_value shaderid) {
  mrb_sdl2_gpu_shader_data_t *data;
  if (mrb_nil_p(shaderid)) {
    return 0;
  }
  data =
    (mrb_sdl2_gpu_shader_data_t*)
      mrb_data_get_ptr(mrb, shaderid, &mrb_sdl2_gpu_shader_data_type);
  return data->shaderid;
}


mrb_value
mrb_sdl2_gpu_shader(mrb_state *mrb, Uint32 shaderid) {
  mrb_sdl2_gpu_shader_data_t *data =
    (mrb_sdl2_gpu_shader_data_t*)
      mrb_malloc(mrb, sizeof(mrb_sdl2_gpu_shader_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->shaderid = shaderid;
  return mrb_obj_value(
  Data_Wrap_Struct(mrb, class_Shader, &mrb_sdl2_gpu_shader_data_type, data));
}
/*******************************
 * Shader bindings ends here
 *******************************/

/*********************************
 * Program bindings starts here
 *********************************/

typedef struct mrb_sdl2_gpu_program_data_t {
  Uint32 programid;
} mrb_sdl2_gpu_program_data_t;

static void
mrb_sdl2_gpu_program_data_free(mrb_state *mrb, void *p) {
  mrb_sdl2_gpu_program_data_t *data =
    (mrb_sdl2_gpu_program_data_t*)p;
  if (0 != data->programid) {
    GPU_FreeShaderProgram(data->programid);
  }
  mrb_free(mrb, data);
}

static struct mrb_data_type const mrb_sdl2_gpu_program_data_type = {
  "Program", mrb_sdl2_gpu_program_data_free
};

Uint32
mrb_sdl2_gpu_program_get_uint32(mrb_state *mrb, mrb_value programid) {
  mrb_sdl2_gpu_program_data_t *data;
  if (mrb_nil_p(programid)) {
    return 0;
  }
  data =
    (mrb_sdl2_gpu_program_data_t*)
      mrb_data_get_ptr(mrb, programid, &mrb_sdl2_gpu_program_data_type);
  return data->programid;
}


mrb_value
mrb_sdl2_gpu_program(mrb_state *mrb, Uint32 programid) {
  mrb_sdl2_gpu_program_data_t *data =
    (mrb_sdl2_gpu_program_data_t*)
      mrb_malloc(mrb, sizeof(mrb_sdl2_gpu_program_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->programid = programid;
  return mrb_obj_value(
      Data_Wrap_Struct(mrb,
                       class_Program,
                       &mrb_sdl2_gpu_program_data_type,
                       data));
}
/*******************************
 * Program bindings ends here
 *******************************/

/*********************************
 * GPU_AttributeFormat bindings starts here
 *********************************/

typedef struct mrb_sdl2_gpu_attributeformat_data_t {
  GPU_AttributeFormat attributeformat;
} mrb_sdl2_gpu_attributeformat_data_t;

static void
mrb_sdl2_gpu_attributeformat_data_free(mrb_state *mrb, void *p) {
  mrb_sdl2_gpu_attributeformat_data_t *data =
    (mrb_sdl2_gpu_attributeformat_data_t*)p;
  if (NULL != data) {
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_gpu_attributeformat_data_type = {
  "AttributeFormat", mrb_sdl2_gpu_attributeformat_data_free
};

GPU_AttributeFormat *
mrb_sdl2_gpu_attributeformat_get_ptr(mrb_state *mrb,
                                     mrb_value attributeformat) {
  mrb_sdl2_gpu_attributeformat_data_t *data;
  if (mrb_nil_p(attributeformat)) {
    return NULL;
  }
  data =
    (mrb_sdl2_gpu_attributeformat_data_t*)
      mrb_data_get_ptr(mrb,
                       attributeformat,
                       &mrb_sdl2_gpu_attributeformat_data_type);
  return &data->attributeformat;
}

mrb_value
mrb_sdl2_gpu_attributeformat(mrb_state *mrb,
                             GPU_AttributeFormat attributeformat) {
  mrb_sdl2_gpu_attributeformat_data_t *data =
    (mrb_sdl2_gpu_attributeformat_data_t*)
      mrb_malloc(mrb, sizeof(mrb_sdl2_gpu_attributeformat_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->attributeformat.is_per_sprite = attributeformat.is_per_sprite;
  data->attributeformat.num_elems_per_value =
      attributeformat.num_elems_per_value;
  data->attributeformat.type = attributeformat.type;
  data->attributeformat.normalize = attributeformat.normalize;
  data->attributeformat.stride_bytes = attributeformat.stride_bytes;
  data->attributeformat.offset_bytes = attributeformat.offset_bytes;
  return mrb_obj_value(
      Data_Wrap_Struct(mrb, class_AttributeFormat,
                       &mrb_sdl2_gpu_attributeformat_data_type, data));
}
/*******************************
 * GPU_AttributeFormat bindings ends here
 *******************************/

/*********************************
 * GPU_Attribute bindings starts here
 *********************************/

typedef struct mrb_sdl2_gpu_attribute_data_t {
  GPU_Attribute *attribute;
} mrb_sdl2_gpu_attribute_data_t;

static void
mrb_sdl2_gpu_attribute_data_free(mrb_state *mrb, void *p) {
  mrb_sdl2_gpu_attribute_data_t *data =
    (mrb_sdl2_gpu_attribute_data_t*)p;
  if (NULL != data->attribute) {
    mrb_free(mrb, (void *) data->attribute);
  }
  mrb_free(mrb, data);
}

static struct mrb_data_type const mrb_sdl2_gpu_attribute_data_type = {
  "Attribute", mrb_sdl2_gpu_attribute_data_free
};

GPU_Attribute *
mrb_sdl2_gpu_attribute_get_ptr(mrb_state *mrb, mrb_value attribute) {
  mrb_sdl2_gpu_attribute_data_t *data;
  if (mrb_nil_p(attribute)) {
    return NULL;
  }
  data =
    (mrb_sdl2_gpu_attribute_data_t*)
      mrb_data_get_ptr(mrb, attribute, &mrb_sdl2_gpu_attribute_data_type);
  return data->attribute;
}

mrb_value
mrb_sdl2_gpu_attribute(mrb_state *mrb, GPU_Attribute *attribute) {
  mrb_sdl2_gpu_attribute_data_t *data =
    (mrb_sdl2_gpu_attribute_data_t*)
      mrb_malloc(mrb, sizeof(mrb_sdl2_gpu_attribute_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->attribute = attribute;
  return mrb_obj_value(
  Data_Wrap_Struct(mrb, class_Attribute,
                   &mrb_sdl2_gpu_attribute_data_type, data));
}
/*******************************
 * GPU_Attribute bindings ends here
 *******************************/

/*********************************
 * GPU_ShaderBlock bindings starts here
 *********************************/

typedef struct mrb_sdl2_gpu_shaderblock_data_t {
  GPU_ShaderBlock shaderblock;
} mrb_sdl2_gpu_shaderblock_data_t;

static void
mrb_sdl2_gpu_shaderblock_data_free(mrb_state *mrb, void *p) {
  mrb_sdl2_gpu_shaderblock_data_t *data =
    (mrb_sdl2_gpu_shaderblock_data_t*)p;
  if (NULL != data)
    mrb_free(mrb, data);
}

static struct mrb_data_type const mrb_sdl2_gpu_shaderblock_data_type = {
  "ShaderBlock", mrb_sdl2_gpu_shaderblock_data_free
};

GPU_ShaderBlock *
mrb_sdl2_gpu_shaderblock_get_ptr(mrb_state *mrb, mrb_value shaderblock) {
  mrb_sdl2_gpu_shaderblock_data_t *data;
  if (mrb_nil_p(shaderblock)) {
    return NULL;
  }
  data =
    (mrb_sdl2_gpu_shaderblock_data_t*)
      mrb_data_get_ptr(mrb, shaderblock, &mrb_sdl2_gpu_shaderblock_data_type);
  return &data->shaderblock;
}

mrb_value
mrb_sdl2_gpu_shaderblock(mrb_state *mrb, GPU_ShaderBlock shaderblock) {
  mrb_sdl2_gpu_shaderblock_data_t *data =
    (mrb_sdl2_gpu_shaderblock_data_t*)
      mrb_malloc(mrb, sizeof(mrb_sdl2_gpu_shaderblock_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->shaderblock.position_loc = shaderblock.position_loc;
  data->shaderblock.texcoord_loc = shaderblock.texcoord_loc;
  data->shaderblock.color_loc = shaderblock.color_loc;
  data->shaderblock.modelViewProjection_loc =
      shaderblock.modelViewProjection_loc;
  return mrb_obj_value(
  Data_Wrap_Struct(mrb, class_ShaderBlock,
                   &mrb_sdl2_gpu_shaderblock_data_type, data));
}
/*******************************
 * GPU_ShaderBlock bindings ends here
 *******************************/

/**************************************
 * GPU_MatrixStack bindings starts here
 **************************************/

typedef struct mrb_sdl2_gpu_matrixstack_data_t {
  GPU_MatrixStack matrixstack;
} mrb_sdl2_gpu_matrixstack_data_t;

static void
mrb_sdl2_gpu_matrixstack_data_free(mrb_state *mrb, void *p) {
  mrb_sdl2_gpu_matrixstack_data_t *data =
    (mrb_sdl2_gpu_matrixstack_data_t*)p;
  if (NULL != data) {
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_gpu_matrixstack_data_type = {
  "MatrixStack", mrb_sdl2_gpu_matrixstack_data_free
};

GPU_MatrixStack
mrb_sdl2_gpu_matrixstack_get(mrb_state *mrb, mrb_value matrixstack) {
  mrb_sdl2_gpu_matrixstack_data_t *data;
  data =
    (mrb_sdl2_gpu_matrixstack_data_t*)
      mrb_data_get_ptr(mrb, matrixstack, &mrb_sdl2_gpu_matrixstack_data_type);
  return data->matrixstack;
}


mrb_value
mrb_sdl2_gpu_matrixstack(mrb_state *mrb, GPU_MatrixStack matrixstack) {
  mrb_sdl2_gpu_matrixstack_data_t *data =
    (mrb_sdl2_gpu_matrixstack_data_t*)
      mrb_malloc(mrb, sizeof(mrb_sdl2_gpu_matrixstack_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->matrixstack = matrixstack;
  return mrb_obj_value(
      Data_Wrap_Struct(mrb,
                       class_MatrixStack,
                       &mrb_sdl2_gpu_matrixstack_data_type, data));
}
/************************************
 * GPU_MatrixStack bindings ends here
 ************************************/

/*********************************
 * GPU_Rect bindings starts here
 *********************************/

typedef struct mrb_sdl2_gpu_rect_data_t {
  GPU_Rect rect;
} mrb_sdl2_gpu_rect_data_t;

static void
mrb_sdl2_gpu_rect_data_free(mrb_state *mrb, void *p) {
  mrb_sdl2_gpu_rect_data_t *data =
    (mrb_sdl2_gpu_rect_data_t*)p;
  if (NULL != data) {
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_gpu_rect_data_type = {
  "Rect", mrb_sdl2_gpu_rect_data_free
};

GPU_Rect *
mrb_sdl2_gpu_rect_get_ptr(mrb_state *mrb, mrb_value rect) {
  mrb_sdl2_gpu_rect_data_t *data;
  if (mrb_nil_p(rect)) {
    return NULL;
  }
  data =
    (mrb_sdl2_gpu_rect_data_t*)
      mrb_data_get_ptr(mrb, rect, &mrb_sdl2_gpu_rect_data_type);
  return &data->rect;
}

mrb_value
mrb_sdl2_gpu_rect(mrb_state *mrb, GPU_Rect rect) {
  mrb_sdl2_gpu_rect_data_t *data =
    (mrb_sdl2_gpu_rect_data_t*)
      mrb_malloc(mrb, sizeof(mrb_sdl2_gpu_rect_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->rect = rect;
  return mrb_obj_value(
      Data_Wrap_Struct(mrb,
                       class_Rect,
                       &mrb_sdl2_gpu_rect_data_type,
                       data));
}
/*******************************
 * GPU_Rect bindings ends here
 *******************************/

/*********************************
 * GPU_Image bindings starts here
 *********************************/

typedef struct mrb_sdl2_gpu_image_data_t {
  GPU_Image *image;
} mrb_sdl2_gpu_image_data_t;

static void
mrb_sdl2_gpu_image_data_free(mrb_state *mrb, void *p) {
  mrb_sdl2_gpu_image_data_t *data =
    (mrb_sdl2_gpu_image_data_t*)p;
  if (NULL != data) {
    if (NULL != data->image) {
      GPU_FreeImage(data->image);
    }
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_gpu_image_data_type = {
  "Image", mrb_sdl2_gpu_image_data_free
};

GPU_Image *
mrb_sdl2_gpu_image_get_ptr(mrb_state *mrb, mrb_value image) {
mrb_sdl2_gpu_image_data_t *data;
  if (mrb_nil_p(image)) {
    return NULL;
  }
  data =
    (mrb_sdl2_gpu_image_data_t*)
      mrb_data_get_ptr(mrb,
                       image,
                       &mrb_sdl2_gpu_image_data_type);
  return data->image;
}

mrb_value
mrb_sdl2_gpu_image(mrb_state *mrb, GPU_Image *image) {
  mrb_sdl2_gpu_image_data_t *data =
    (mrb_sdl2_gpu_image_data_t*)
      mrb_malloc(mrb, sizeof(mrb_sdl2_gpu_image_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory for Image.");
  }
  data->image = image;
  return mrb_obj_value(
      Data_Wrap_Struct(mrb,
                       class_Image,
                       &mrb_sdl2_gpu_image_data_type,
                       data));
}
/*******************************
 * GPU_Image bindings ends here
 *******************************/

/*******************************
 * GPU_Target bindings starts here
 *******************************/
typedef struct mrb_sdl2_gpu_blendmode_data_t {
  GPU_BlendMode blendmode;
} mrb_sdl2_gpu_blendmode_data_t;

static void
mrb_sdl2_gpu_blendmode_data_free(mrb_state *mrb, void *p) {
  mrb_sdl2_gpu_blendmode_data_t *data =
    (mrb_sdl2_gpu_blendmode_data_t*)p;
  if (NULL != data) {
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_gpu_blendmode_data_type = {
  "BlendMode", mrb_sdl2_gpu_blendmode_data_free
};

GPU_BlendMode *
mrb_sdl2_gpu_blendmode_get_ptr(mrb_state *mrb, mrb_value blendmode) {
  mrb_sdl2_gpu_blendmode_data_t *data;
  if (mrb_nil_p(blendmode)) {
    return NULL;
  }
  data =
    (mrb_sdl2_gpu_blendmode_data_t*)
      mrb_data_get_ptr(mrb,
                       blendmode,
                       &mrb_sdl2_gpu_blendmode_data_type);
  return &data->blendmode;
}

mrb_value
mrb_sdl2_gpu_blendmode(mrb_state *mrb, GPU_BlendMode *blendmode) {
  mrb_sdl2_gpu_blendmode_data_t *data =
    (mrb_sdl2_gpu_blendmode_data_t*)
      mrb_malloc(mrb, sizeof(mrb_sdl2_gpu_blendmode_data_t));
  if (NULL != data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->blendmode = *blendmode;
  return mrb_obj_value(
      Data_Wrap_Struct(mrb,
                       class_BlendMode,
                       &mrb_sdl2_gpu_blendmode_data_type,
                       data));
}
/**********************************
 * GPU_BlendMode bindings ends here
 **********************************/

/**********************************
 * GPU_Context bindings starts here
 **********************************/

typedef struct mrb_sdl2_gpu_context_data_t {
  GPU_Context context;
} mrb_sdl2_gpu_context_data_t;

static void
mrb_sdl2_gpu_context_data_free(mrb_state *mrb, void *p) {
  mrb_sdl2_gpu_context_data_t *data =
    (mrb_sdl2_gpu_context_data_t*)p;
  if (NULL != data) {
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_gpu_context_data_type = {
  "Context", mrb_sdl2_gpu_context_data_free
};

GPU_Context *
mrb_sdl2_gpu_context_get_ptr(mrb_state *mrb, mrb_value context) {
  mrb_sdl2_gpu_context_data_t *data;
  if (mrb_nil_p(context)) {
    return NULL;
  }
  data =
    (mrb_sdl2_gpu_context_data_t*)
      mrb_data_get_ptr(mrb, context, &mrb_sdl2_gpu_context_data_type);
  return &data->context;
}


mrb_value
mrb_sdl2_gpu_context(mrb_state *mrb, GPU_Context *context) {
  mrb_sdl2_gpu_context_data_t *data =
    (mrb_sdl2_gpu_context_data_t*)
      mrb_malloc(mrb, sizeof(mrb_sdl2_gpu_context_data_t));
  if (NULL != data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->context = *context;
  return mrb_obj_value(
      Data_Wrap_Struct(mrb,
                       class_Context,
                       &mrb_sdl2_gpu_context_data_type, data));
}
/*****************************************
 * GPU_Context bindings renderer ends here
 *****************************************/

/*******************************************
 * GPU_Camera bindings renderer starts here
 *******************************************/
typedef struct mrb_sdl2_gpu_camera_data_t {
  GPU_Camera camera;
} mrb_sdl2_gpu_camera_data_t;

static void
mrb_sdl2_gpu_camera_data_free(mrb_state *mrb, void *p) {
  mrb_sdl2_gpu_camera_data_t *data =
    (mrb_sdl2_gpu_camera_data_t*)p;
  if (NULL != data) {
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_gpu_camera_data_type = {
  "Camera", mrb_sdl2_gpu_camera_data_free
};

GPU_Camera *
mrb_sdl2_gpu_camera_get_ptr(mrb_state *mrb, mrb_value camera) {
  mrb_sdl2_gpu_camera_data_t *data;
  if (mrb_nil_p(camera)) {
    return NULL;
  }
  data =
    (mrb_sdl2_gpu_camera_data_t*)
      mrb_data_get_ptr(mrb, camera, &mrb_sdl2_gpu_camera_data_type);
  return &data->camera;
}


mrb_value
mrb_sdl2_gpu_camera(mrb_state *mrb, GPU_Camera *camera) {
  mrb_sdl2_gpu_camera_data_t *data =
    (mrb_sdl2_gpu_camera_data_t*)
      mrb_malloc(mrb, sizeof(mrb_sdl2_gpu_camera_data_t));
  if (NULL != data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->camera = *camera;
  return mrb_obj_value(
      Data_Wrap_Struct(mrb,
                       class_Camera,
                       &mrb_sdl2_gpu_camera_data_type, data));
}
/********************************************
 * GPU_Camera bindings renderer ends here
 ********************************************/

/********************************************
 * GPU_Renderer bindings renderer starts here
 ********************************************/
typedef struct mrb_sdl2_gpu_renderer_data_t {
  GPU_Renderer *renderer;
} mrb_sdl2_gpu_renderer_data_t;

static void
mrb_sdl2_gpu_renderer_data_free(mrb_state *mrb, void *p) {
  mrb_sdl2_gpu_renderer_data_t *data =
    (mrb_sdl2_gpu_renderer_data_t*)p;
  if (NULL != data) {
    if (NULL != data->renderer) {
      GPU_FreeRenderer(data->renderer);
    }
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_gpu_renderer_data_type = {
  "Renderer", mrb_sdl2_gpu_renderer_data_free
};

GPU_Renderer *
mrb_sdl2_gpu_renderer_get_ptr(mrb_state *mrb, mrb_value renderer) {
  mrb_sdl2_gpu_renderer_data_t *data;
  if (mrb_nil_p(renderer)) {
    return NULL;
  }
  data =
    (mrb_sdl2_gpu_renderer_data_t*)
      mrb_data_get_ptr(mrb, renderer, &mrb_sdl2_gpu_renderer_data_type);
  return data->renderer;
}


mrb_value
mrb_sdl2_gpu_renderer(mrb_state *mrb, GPU_Renderer *renderer) {
  mrb_sdl2_gpu_renderer_data_t *data =
    (mrb_sdl2_gpu_renderer_data_t*)
      mrb_malloc(mrb, sizeof(mrb_sdl2_gpu_renderer_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->renderer = renderer;
  return mrb_obj_value(
      Data_Wrap_Struct(mrb,
                       class_Renderer,
                       &mrb_sdl2_gpu_renderer_data_type, data));
}
/********************************
  GPU_Renderer bindings ends here
*********************************/

/*************************************
 * GPU_RendererID bindings starts here
 *************************************/
typedef struct mrb_sdl2_gpu_rendererid_data_t {
  GPU_RendererID rendererid;
} mrb_sdl2_gpu_rendererid_data_t;

static void
mrb_sdl2_gpu_rendererid_data_free(mrb_state *mrb, void *p) {
  mrb_sdl2_gpu_rendererid_data_t *data =
    (mrb_sdl2_gpu_rendererid_data_t*)p;
  if (NULL != data) {
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_gpu_rendererid_data_type = {
  "RendererID", mrb_sdl2_gpu_rendererid_data_free
};

GPU_RendererID *
mrb_sdl2_gpu_rendererid_get_ptr(mrb_state *mrb, mrb_value rendererid) {
  mrb_sdl2_gpu_rendererid_data_t *data;
  if (mrb_nil_p(rendererid)) {
    return NULL;
  }
  data =
    (mrb_sdl2_gpu_rendererid_data_t*)
      mrb_data_get_ptr(mrb, rendererid, &mrb_sdl2_gpu_rendererid_data_type);
  return &data->rendererid;
}

mrb_value
mrb_sdl2_gpu_rendererid(mrb_state *mrb, GPU_RendererID *rendererid) {
  mrb_sdl2_gpu_rendererid_data_t *data =
    (mrb_sdl2_gpu_rendererid_data_t*)
      mrb_malloc(mrb,
                 sizeof(mrb_sdl2_gpu_rendererid_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->rendererid = *rendererid;
  return mrb_obj_value(
      Data_Wrap_Struct(mrb,
                       class_RendererID,
                       &mrb_sdl2_gpu_rendererid_data_type, data));
}
/**********************************
 * GPU_RendererID bindings end here
 **********************************/


/************************************************
 *  Binding initialization functions start's here
 ************************************************/

static mrb_value
mrb_sdl2_gpu_get_linked_ver(mrb_state *mrb, mrb_value self) {
  mrb_value array;
  SDL_version v;
  array = mrb_ary_new_capa(mrb, 3);
  v = GPU_GetLinkedVersion();
  mrb_ary_push(mrb, array, mrb_fixnum_value(v.major));
  mrb_ary_push(mrb, array, mrb_fixnum_value(v.minor));
  mrb_ary_push(mrb, array, mrb_fixnum_value(v.patch));
  return array;
}

static mrb_value
mrb_sdl2_gpu_set_init_window(mrb_state *mrb, mrb_value self) {
  mrb_int windowID;
  mrb_get_args(mrb, "i", &windowID);
  GPU_SetInitWindow(windowID);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_get_init_window(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(GPU_GetInitWindow());
}

static mrb_value
mrb_sdl2_gpu_set_pre_init_flags(mrb_state *mrb, mrb_value self) {
  mrb_int GPU_flags;
  mrb_get_args(mrb, "i", &GPU_flags);
  GPU_SetPreInitFlags(GPU_flags);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_get_pre_init_flags(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(GPU_GetPreInitFlags());
}

static mrb_value
mrb_sdl2_gpu_set_required_features(mrb_state *mrb, mrb_value self) {
  mrb_int features;
  mrb_get_args(mrb, "i", &features);
  GPU_SetRequiredFeatures(features);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_get_required_features(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(GPU_GetRequiredFeatures());
}

static mrb_value
mrb_sdl2_gpu_get_default_render_order(mrb_state *mrb, mrb_value self) {
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_get_renderer_order(mrb_state *mrb, mrb_value self) {
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_set_renderer_order(mrb_state *mrb, mrb_value self) {
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_init(mrb_state *mrb, mrb_value self) {
  mrb_int w, h, sdl_flags;
  GPU_Target *t;
  mrb_get_args(mrb, "iii", &w, &h, &sdl_flags);
  t = GPU_Init(w, h, sdl_flags);
  if (NULL == t)
    mrb_raise(mrb, E_NOTIMP_ERROR, "GEORGE");

  return mrb_sdl2_gpu_target(mrb, t);
}

static mrb_value
mrb_sdl2_gpu_init_renderer(mrb_state *mrb, mrb_value self) {
  mrb_int w, h, sdl_flags, renderer_enum;
  GPU_Target *t;
  mrb_get_args(mrb, "iiii", &renderer_enum, &w, &h, &sdl_flags);
  t = GPU_InitRenderer(renderer_enum, w, h, sdl_flags);
  return mrb_sdl2_gpu_target(mrb, t);
}

static mrb_value
mrb_sdl2_gpu_init_renderer_by_id(mrb_state *mrb, mrb_value self) {
  mrb_int w, h, sdl_flags;
  mrb_value renderer_request;
  GPU_Target *t;
  GPU_RendererID *r;
  mrb_get_args(mrb, "oiii", &renderer_request, &w, &h, &sdl_flags);
  r = mrb_sdl2_gpu_rendererid_get_ptr(mrb, renderer_request);
  t = GPU_InitRendererByID(*r, w, h, sdl_flags);
  return mrb_sdl2_gpu_target(mrb, t);
}

static mrb_value
mrb_sdl2_gpu_feature_enabled(mrb_state *mrb, mrb_value self) {
  mrb_int feature;
  int result;
  mrb_get_args(mrb, "i", &feature);
  result = GPU_IsFeatureEnabled(feature);
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_gpu_close_current_renderer(mrb_state *mrb, mrb_value self) {
  GPU_CloseCurrentRenderer();
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_quit(mrb_state *mrb, mrb_value self) {
  GPU_Quit();
  return mrb_nil_value();
}

/*********************************************
 *  Binding initialization functions ends here
 *********************************************/


/***********************************************************************
 *  Binding Debugging, Logging, and Error Handling functions starts here
 ***********************************************************************/

static mrb_value
mrb_sdl2_gpu_set_debug_level(mrb_state *mrb, mrb_value self) {
  mrb_int level;
  mrb_get_args(mrb, "i", &level);
  GPU_SetDebugLevel(level);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_get_debug_level(mrb_state *mrb, mrb_value self) {
  int result = GPU_GetDebugLevel();
  return mrb_fixnum_value(result);
}

/***********************************************************************
 *  Binding Debugging, Logging, and Error Handling functions ends here
 ***********************************************************************/


/***********************************************************************
 *  Binding Renderer Setup functions starts here
 ***********************************************************************/
static mrb_value
mrb_sdl2_gpu_mark_renderer_id(mrb_state *mrb, mrb_value self) {
  mrb_value name;
  mrb_int renderer, major_version, minor_version;
  GPU_RendererID rid;
  mrb_get_args(mrb, "Siii", &name, &renderer, &major_version, &minor_version);

  rid = GPU_MakeRendererID(RSTRING_PTR(name),
                           renderer, major_version, minor_version);
  return mrb_sdl2_gpu_rendererid(mrb, &rid);
}

static mrb_value
mrb_sdl2_gpu_get_renderer_id(mrb_state *mrb, mrb_value self) {
  mrb_value name;
  mrb_int renderer, major_version, minor_version;
  GPU_RendererID rid;
  mrb_get_args(mrb, "Siii", &name, &renderer, &major_version, &minor_version);

  rid = GPU_MakeRendererID(RSTRING_PTR(name),
                           renderer, major_version, minor_version);
  return mrb_sdl2_gpu_rendererid(mrb, &rid);
}

static mrb_value
mrb_sdl2_gpu_get_renderer_index(mrb_state *mrb, mrb_value self) {
  mrb_int index;
  GPU_RendererID rid;
  mrb_get_args(mrb, "i", &index);

  rid = GPU_GetRendererID(index);
  return mrb_sdl2_gpu_rendererid(mrb, &rid);
}

static mrb_value
mrb_sdl2_gpu_get_num_registered_renderers(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(GPU_GetNumRegisteredRenderers());
}
/***********************************************************************
 *  Binding Renderer Setup functions ends here
 ***********************************************************************/


/***********************************************************************
 * Binding Renderer Controls functions ends here
 ***********************************************************************/
static mrb_value
mrb_sdl2_gpu_reserver_next_enum(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(GPU_ReserveNextRendererEnum());
}

static mrb_value
mrb_sdl2_gpu_num_active_renderers(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(GPU_GetNumActiveRenderers());
}

static mrb_value
mrb_sdl2_gpu_get_renderer(mrb_state *mrb, mrb_value self) {
  mrb_value id;
  GPU_RendererID *rid;
  GPU_Renderer *t;
  mrb_get_args(mrb, "i", &id);
  rid = mrb_sdl2_gpu_rendererid_get_ptr(mrb, id);
  t = GPU_GetRenderer(*rid);
  if (NULL == t)
    return mrb_nil_value();
  return mrb_sdl2_gpu_renderer(mrb, t);
}

static mrb_value
mrb_sdl2_gpu_get_current_renderer(mrb_state *mrb, mrb_value self) {
  GPU_Renderer *t = GPU_GetCurrentRenderer();
  if (NULL == t)
    return mrb_nil_value();
  return mrb_sdl2_gpu_renderer(mrb, t);
}

static mrb_value
mrb_sdl2_gpu_set_current_renderer(mrb_state *mrb, mrb_value self) {
  mrb_value id;
  GPU_RendererID *rid;
  mrb_get_args(mrb, "o", &id);
  rid = mrb_sdl2_gpu_rendererid_get_ptr(mrb, id);
  GPU_SetCurrentRenderer(*rid);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_reset_renderer_state(mrb_state *mrb, mrb_value self) {
  GPU_ResetRendererState();
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_get_context_target(mrb_state *mrb, mrb_value self) {
  GPU_Target *t = GPU_GetContextTarget();
  if (NULL == t)
    return mrb_nil_value();
  return mrb_sdl2_gpu_target(mrb, t);
}

static mrb_value
mrb_sdl2_gpu_get_window_target(mrb_state *mrb, mrb_value self) {
  mrb_int windowID;
  GPU_Target *t;
  mrb_get_args(mrb, "i", &windowID);
  t = GPU_GetWindowTarget(windowID);
  if (NULL == t)
    return mrb_nil_value();
  return mrb_sdl2_gpu_target(mrb, t);
}

static mrb_value
mrb_sdl2_gpu_create_target_from_window(mrb_state *mrb, mrb_value self) {
  mrb_int windowID;
  GPU_Target *t;
  mrb_get_args(mrb, "i", &windowID);
  t = GPU_CreateTargetFromWindow(windowID);
  if (NULL == t)
    return mrb_nil_value();
  return mrb_sdl2_gpu_target(mrb, t);
}

static mrb_value
mrb_sdl2_gpu_target_make_current(mrb_state *mrb, mrb_value self) {
  GPU_Target *t;
  mrb_int windowID;
  mrb_get_args(mrb, "i", &windowID);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);
  GPU_MakeCurrent(t, windowID);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_set_window_resolution(mrb_state *mrb, mrb_value self) {
  mrb_int w, h;
  int result;
  mrb_get_args(mrb, "ii", &w, &h);
  result = GPU_SetWindowResolution(w, h);
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_gpu_set_fullscreen(mrb_state *mrb, mrb_value self) {
  mrb_int enable_fullscreen, use_desktop_resolution;
  int result;
  mrb_get_args(mrb, "ii", &enable_fullscreen, &use_desktop_resolution);
  result = GPU_SetFullscreen(enable_fullscreen, use_desktop_resolution);
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_gpu_get_fullscreen(mrb_state *mrb, mrb_value self) {
  int result = GPU_GetFullscreen();
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_gpu_set_shape_blending(mrb_state *mrb, mrb_value self) {
  mrb_int enable;
  mrb_get_args(mrb, "i", &enable);
  GPU_SetShapeBlending(enable);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_get_blend_mode_from_preset(mrb_state *mrb, mrb_value self) {
  mrb_int preset;
  GPU_BlendMode bm;
  mrb_get_args(mrb, "i", &preset);
  bm = GPU_GetBlendModeFromPreset(preset);
  return mrb_sdl2_gpu_blendmode(mrb, &bm);
}

static mrb_value
mrb_sdl2_gpu_set_shape_blend_mode_function(mrb_state *mrb, mrb_value self) {
  mrb_int source_color, dest_color, source_alpha, dest_alpha;
  mrb_get_args(mrb, "iiii", &source_color, &dest_color,
                            &source_alpha, &dest_alpha);
  GPU_SetShapeBlendFunction(source_color, dest_color, source_alpha, dest_alpha);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_set_shape_blend_equation(mrb_state *mrb, mrb_value self) {
  mrb_int color_equation, alpha_equation;
  mrb_get_args(mrb, "ii", &color_equation, &alpha_equation);
  GPU_SetShapeBlendEquation(color_equation, alpha_equation);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_set_shape_blend_mode(mrb_state *mrb, mrb_value self) {
  mrb_int mode;
  mrb_get_args(mrb, "i", &mode);
  GPU_SetShapeBlendMode(mode);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_set_line_thickness(mrb_state *mrb, mrb_value self) {
  mrb_int thickness;
  float f;
  mrb_get_args(mrb, "i", &thickness);
  f = GPU_SetLineThickness(thickness);
  return mrb_float_value(mrb, f);
}

static mrb_value
mrb_sdl2_gpu_get_line_thickness(mrb_state *mrb, mrb_value self) {
  return mrb_float_value(mrb, GPU_GetLineThickness());
}

static mrb_value
mrb_sdl2_gpu_create_alias_target(mrb_state *mrb, mrb_value self) {
  mrb_value target;
  GPU_Target *t;
  GPU_Target *returnt;
  mrb_get_args(mrb, "o", &target);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, target);
  returnt = GPU_CreateAliasTarget(t);

  return mrb_sdl2_gpu_target(mrb, returnt);
}

static mrb_value
mrb_sdl2_gpu_load_target(mrb_state *mrb, mrb_value self) {
  mrb_value image;
  GPU_Target *t;
  GPU_Image *i;
  mrb_get_args(mrb, "o", &image);
  i = mrb_sdl2_gpu_image_get_ptr(mrb, image);
  t = GPU_LoadTarget(i);

  return mrb_sdl2_gpu_target(mrb, t);
}

static mrb_value
mrb_sdl2_gpu_set_virtual_resolution(mrb_state *mrb, mrb_value self) {
  mrb_value target;
  mrb_int w, h;
  GPU_Target *t;
  mrb_get_args(mrb, "oii", &target, &w, &h);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, target);
  GPU_SetVirtualResolution(t, w, h);

  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_get_virtual_coords(mrb_state *mrb, mrb_value self) {
  mrb_value target;
  GPU_Target *t;
  float x, y;
  mrb_float dx, dy;
  mrb_get_args(mrb, "off", &target, &dx, &dy);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, target);
  GPU_GetVirtualCoords(t, &x, &y, dx, dy);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_unset_virtual_resolution(mrb_state *mrb, mrb_value self) {
  mrb_value target;
  GPU_Target *t;
  mrb_get_args(mrb, "oii", &target);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, target);
  GPU_UnsetVirtualResolution(t);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_make_rect(mrb_state *mrb, mrb_value self) {
  GPU_Rect r;
  mrb_float x, y, w, h;
  mrb_get_args(mrb, "ffff", &x, &y, &w, &h);
  r = GPU_MakeRect(x, y, w, h);
  return mrb_sdl2_gpu_rect(mrb, r);
}

static mrb_value
mrb_sdl2_gpu_make_color(mrb_state *mrb, mrb_value self) {
  mrb_value array;
  SDL_Color c;
  mrb_int r, g, b, a;
  mrb_get_args(mrb, "iiii", &r, &g, &b, &a);
  c = GPU_MakeColor(r, g, b, a);
  array = mrb_ary_new_capa(mrb, 4);
  mrb_ary_push(mrb, array, mrb_fixnum_value(c.r));
  mrb_ary_push(mrb, array, mrb_fixnum_value(c.g));
  mrb_ary_push(mrb, array, mrb_fixnum_value(c.b));
  mrb_ary_push(mrb, array, mrb_fixnum_value(c.a));
  return array;
}

static mrb_value
mrb_sdl2_gpu_target_set_viewport(mrb_state *mrb, mrb_value self) {
  GPU_Target *t;
  GPU_Rect *r;
  mrb_value rect;
  mrb_get_args(mrb, "o", &rect);
  r = mrb_sdl2_gpu_rect_get_ptr(mrb, rect);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);
  GPU_SetViewport(t, *r);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_default_camera(mrb_state *mrb, mrb_value self) {
  GPU_Camera c = GPU_GetDefaultCamera();
  return mrb_sdl2_gpu_camera(mrb, &c);
}

static mrb_value
mrb_sdl2_gpu_target_get_camera(mrb_state *mrb, mrb_value self) {
  GPU_Target *t = mrb_sdl2_gpu_target_get_ptr(mrb, self);
  GPU_Camera c = GPU_GetCamera(t);
  return mrb_sdl2_gpu_camera(mrb, &c);
}

static mrb_value
mrb_sdl2_gpu_target_set_camera(mrb_state *mrb, mrb_value self) {
  GPU_Target *t;
  GPU_Camera *c;
  GPU_Camera resultc;
  mrb_value camera;
  mrb_get_args(mrb, "o", &camera);
  c = mrb_sdl2_gpu_camera_get_ptr(mrb, camera);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);

  resultc = GPU_SetCamera(t, c);
  return mrb_sdl2_gpu_camera(mrb, &resultc);
}

static mrb_value
mrb_sdl2_gpu_target_get_pixel(mrb_state *mrb, mrb_value self) {
  GPU_Target *t;
  mrb_value array;
  SDL_Color c;
  mrb_int x, y;
  mrb_get_args(mrb, "ii", &x, &y);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);
  c = GPU_GetPixel(t, x, y);
  array = mrb_ary_new_capa(mrb, 4);
  mrb_ary_push(mrb, array, mrb_fixnum_value(c.r));
  mrb_ary_push(mrb, array, mrb_fixnum_value(c.g));
  mrb_ary_push(mrb, array, mrb_fixnum_value(c.b));
  mrb_ary_push(mrb, array, mrb_fixnum_value(c.a));

  return array;
}

static mrb_value
mrb_sdl2_gpu_target_set_clip_rect(mrb_state *mrb, mrb_value self) {
  GPU_Target *t;
  GPU_Rect *r = NULL;
  GPU_Rect rectResult;
  mrb_value rect;
  mrb_get_args(mrb, "o", &rect);

  if (mrb_nil_p(rect))
    mrb_raise(mrb, E_RUNTIME_ERROR,
              "GPU::Rect can't be nil. Use .unset_clip instead.");

  r = mrb_sdl2_gpu_rect_get_ptr(mrb, rect);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);
  rectResult = GPU_SetClipRect(t, *r);
  return mrb_sdl2_gpu_rect(mrb, rectResult);
}

static mrb_value
mrb_sdl2_gpu_target_set_clip(mrb_state *mrb, mrb_value self) {
  GPU_Target *t;
  GPU_Rect rectResult;
  mrb_int x, y, w, h;
  mrb_get_args(mrb, "iiii", &x, &y, &w, &h);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);
  rectResult = GPU_SetClip(t, x, y, w, h);
  return mrb_sdl2_gpu_rect(mrb, rectResult);
}

static mrb_value
mrb_sdl2_gpu_target_unset_clip(mrb_state *mrb, mrb_value self) {
  GPU_Target *t = mrb_sdl2_gpu_target_get_ptr(mrb, self);
  GPU_UnsetClip(t);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_target_set_rgb(mrb_state *mrb, mrb_value self) {
  GPU_Target *t;
  mrb_int r, g, b;
  mrb_get_args(mrb, "iii", &r, &g, &b);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);
  GPU_SetTargetRGB(t, r, g, b);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_target_set_rgba(mrb_state *mrb, mrb_value self) {
  GPU_Target *t;
  mrb_int r, g, b, a;
  mrb_get_args(mrb, "iiii", &r, &g, &b, &a);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);
  GPU_SetTargetRGBA(t, r, g, b, a);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_target_unset_color(mrb_state *mrb, mrb_value self) {
  GPU_Target *t = mrb_sdl2_gpu_target_get_ptr(mrb, self);
  GPU_UnsetTargetColor(t);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_target_free(mrb_state *mrb, mrb_value self) {
  mrb_sdl2_gpu_target_data_t *data =
    (mrb_sdl2_gpu_target_data_t*)
      mrb_data_get_ptr(mrb, self, &mrb_sdl2_gpu_target_data_type);
  if (NULL != data->target) {
    GPU_FreeTarget(data->target);
    data->target = NULL;
  }
  mrb_free(mrb, data);
  return self;
}

static mrb_value
mrb_sdl2_gpu_load_surface(mrb_state *mrb, mrb_value self) {
  mrb_value filename;
  SDL_Surface *s;
  mrb_get_args(mrb, "S", &filename);
  s = GPU_LoadSurface(RSTRING_PTR(filename));
  return mrb_sdl2_video_surface(mrb, s, false);
}

static mrb_value
mrb_sdl2_gpu_save_surface(mrb_state *mrb, mrb_value self) {
  mrb_value surface, filename;
  mrb_int format;
  SDL_Surface *s;
  mrb_get_args(mrb, "oSi", &surface, &filename, &format);
  s = mrb_sdl2_video_surface_get_ptr(mrb, surface);
  return mrb_fixnum_value(GPU_SaveSurface(s, RSTRING_PTR(filename), format));
}

static mrb_value
mrb_sdl2_gpu_image_initialize(mrb_state *mrb, mrb_value self) {
  GPU_Image *image = NULL;
  mrb_sdl2_gpu_image_data_t *data =
    (mrb_sdl2_gpu_image_data_t*)DATA_PTR(self);

  if (NULL == data) {
    data = (mrb_sdl2_gpu_image_data_t*)
        mrb_malloc(mrb, sizeof(mrb_sdl2_gpu_image_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->image = NULL;
  }
  if (1 == mrb->c->ci->argc) {
    mrb_value str;
    SDL_Surface *surface;
    mrb_get_args(mrb, "S", &str);
#ifdef SDL_GPU_NEW_IMAGE_FROM_STRING_BUG
    surface = IMG_Load(RSTRING_PTR(str));
    if (NULL == surface) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "could not load image");
      return mrb_nil_value();
    }
    image = GPU_CreateImage(surface->w, surface->h, GPU_FORMAT_RGBA);
    GPU_UpdateImage(image, NULL, surface, NULL);
    SDL_FreeSurface(surface);
#else
    image = GPU_LoadImage(RSTRING_PTR(str));
#endif
  } else if (2 == mrb->c->ci->argc) {
    mrb_int handle, take_ownership;
    mrb_get_args(mrb, "ii", &handle, &take_ownership);
  } else if (3 == mrb->c->ci->argc) {
    mrb_int w, h, format;
    mrb_get_args(mrb, "iii", &w, &h, &format);
    image = GPU_CreateImage(w, h, format);
  } else {
    mrb_free(mrb, data);
    mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of arguments.");
  }
  if (NULL == image) {
    mrb_free(mrb, data);
    mrb_raise(mrb, E_RUNTIME_ERROR,
              "Could not initialize Image with the given paramets");
  }
  data->image = image;

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_gpu_image_data_type;
  return self;
}

static mrb_value
mrb_sdl2_gpu_image_free(mrb_state *mrb, mrb_value self) {
  mrb_sdl2_gpu_image_data_t *data =
    (mrb_sdl2_gpu_image_data_t*)
      mrb_data_get_ptr(mrb, self, &mrb_sdl2_gpu_image_data_type);
  if (NULL != data->image) {
    GPU_FreeImage(data->image);
    data->image = NULL;
  }
  return self;
}

static mrb_value
mrb_sdl2_gpu_image_create_alias(mrb_state *mrb, mrb_value self) {
  mrb_value image;
  GPU_Image *i;
  GPU_Image *alias;
  mrb_get_args(mrb, "o", &image);
  i = mrb_sdl2_gpu_image_get_ptr(mrb, image);
  alias = GPU_CreateAliasImage(i);
  return mrb_sdl2_gpu_image(mrb, alias);
}

static mrb_value
mrb_sdl2_gpu_image_copy(mrb_state *mrb, mrb_value self) {
  mrb_value image;
  GPU_Image *i;
  GPU_Image *alias;
  mrb_get_args(mrb, "o", &image);
  i = mrb_sdl2_gpu_image_get_ptr(mrb, image);
  alias = GPU_CopyImage(i);
  return mrb_sdl2_gpu_image(mrb, alias);
}

static mrb_value
mrb_sdl2_gpu_image_update(mrb_state *mrb, mrb_value self) {
  mrb_value surface, surface_rect, image_rect;
  SDL_Surface *s;
  GPU_Rect *sr = NULL;
  GPU_Rect *ir = NULL;
  GPU_Image *i;
  int argc = mrb_get_args(mrb, "o|oo", &surface, &image_rect, &surface_rect);
  i = mrb_sdl2_gpu_image_get_ptr(mrb, self);
  s = mrb_sdl2_video_surface_get_ptr(mrb, surface);

  if (argc > 1)
    ir = mrb_sdl2_gpu_rect_get_ptr(mrb, image_rect);

  if (argc > 2)
    sr = mrb_sdl2_gpu_rect_get_ptr(mrb, surface_rect);

  GPU_UpdateImage(i, ir, s, sr);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_image_save(mrb_state *mrb, mrb_value self) {
  mrb_value filename;
  mrb_int format;
  GPU_Image *i;
  int result;
  mrb_get_args(mrb, "Si", &filename, &format);
  i = mrb_sdl2_gpu_image_get_ptr(mrb, self);
  result = GPU_SaveImage(i, RSTRING_PTR(filename), format);
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_gpu_image_generate_mipmaps(mrb_state *mrb, mrb_value self) {
  GPU_GenerateMipmaps(mrb_sdl2_gpu_image_get_ptr(mrb, self));
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_image_set_rgb(mrb_state *mrb, mrb_value self) {
  mrb_int r, g, b;
  GPU_Image *i;
  mrb_get_args(mrb, "iii", &r, &g, &b);
  i = mrb_sdl2_gpu_image_get_ptr(mrb, self);
  GPU_SetRGB(i, r, g, b);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_image_set_rgba(mrb_state *mrb, mrb_value self) {
  mrb_int r, g, b, a;
  GPU_Image *i;
  mrb_get_args(mrb, "iiii", &r, &g, &b, &a);
  i = mrb_sdl2_gpu_image_get_ptr(mrb, self);
  GPU_SetRGBA(i, r, g, b, a);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_image_unset_color(mrb_state *mrb, mrb_value self) {
  GPU_UnsetColor(mrb_sdl2_gpu_image_get_ptr(mrb, self));
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_image_get_blending(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(
      GPU_GetBlending(mrb_sdl2_gpu_image_get_ptr(mrb, self)));
}

static mrb_value
mrb_sdl2_gpu_image_set_blending(mrb_state *mrb, mrb_value self) {
  mrb_int blending;
  mrb_get_args(mrb, "i", &blending);
  GPU_SetBlending(mrb_sdl2_gpu_image_get_ptr(mrb, self), blending);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_image_set_blend_function(mrb_state *mrb, mrb_value self) {
  mrb_int source_color, dest_color, source_alpha, dest_alpha;
  mrb_get_args(mrb, "iiii", &source_color, &dest_color,
                            &source_alpha, &dest_alpha);
  GPU_SetBlendFunction(mrb_sdl2_gpu_image_get_ptr(mrb, self),
                         source_color, dest_color, source_alpha, dest_alpha);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_image_set_blend_equation(mrb_state *mrb, mrb_value self) {
  mrb_int color_equation, alpha_equation;
  mrb_get_args(mrb, "ii", &color_equation, &alpha_equation);
  GPU_SetBlendEquation(
      mrb_sdl2_gpu_image_get_ptr(mrb, self), color_equation, alpha_equation);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_image_set_blend_mode(mrb_state *mrb, mrb_value self) {
  mrb_int blend_mode;
  mrb_get_args(mrb, "i", &blend_mode);
  GPU_SetBlendMode(mrb_sdl2_gpu_image_get_ptr(mrb, self), blend_mode);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_image_set_filter(mrb_state *mrb, mrb_value self) {
  mrb_int filter;
  mrb_get_args(mrb, "i", &filter);
  GPU_SetImageFilter(mrb_sdl2_gpu_image_get_ptr(mrb, self), filter);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_image_get_snap_mode(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(
      GPU_GetSnapMode(mrb_sdl2_gpu_image_get_ptr(mrb, self)));
}

static mrb_value
mrb_sdl2_gpu_image_set_snap_mode(mrb_state *mrb, mrb_value self) {
  mrb_int snap_mode;
  mrb_get_args(mrb, "i", &snap_mode);
  GPU_SetSnapMode(mrb_sdl2_gpu_image_get_ptr(mrb, self), snap_mode);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_image_set_wrap_mode(mrb_state *mrb, mrb_value self) {
  mrb_int wrap_mode_x, wrap_mode_y;
  mrb_get_args(mrb, "ii", &wrap_mode_x, &wrap_mode_y);
  GPU_SetWrapMode(mrb_sdl2_gpu_image_get_ptr(mrb, self),
                  wrap_mode_x, wrap_mode_y);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_surface_to_image(mrb_state *mrb, mrb_value self) {
  GPU_Image *i = GPU_CopyImageFromSurface(
      mrb_sdl2_video_surface_get_ptr(mrb, self));
  if (NULL == i)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not convert surface to image");
  return  mrb_sdl2_gpu_image(mrb, i);
}

static mrb_value
mrb_sdl2_gpu_target_to_image(mrb_state *mrb, mrb_value self) {
  return
      mrb_sdl2_gpu_image(mrb,
                         GPU_CopyImageFromTarget(
                             mrb_sdl2_gpu_target_get_ptr(mrb, self)));
}

static mrb_value
mrb_sdl2_gpu_target_to_surface(mrb_state *mrb, mrb_value self) {
  return
      mrb_sdl2_video_surface(mrb,
                             GPU_CopySurfaceFromTarget(
                                 mrb_sdl2_gpu_target_get_ptr(mrb, self)),
                                 false);
}

static mrb_value
mrb_sdl2_gpu_image_to_surface(mrb_state *mrb, mrb_value self) {
  return
      mrb_sdl2_video_surface(mrb,
                             GPU_CopySurfaceFromImage(
                                 mrb_sdl2_gpu_image_get_ptr(mrb, self)), false);
}

static mrb_value
mrb_sdl2_gpu_target_clear(mrb_state *mrb, mrb_value self) {
  GPU_Clear(mrb_sdl2_gpu_target_get_ptr(mrb, self));
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_target_clear_rgb(mrb_state *mrb, mrb_value self) {
  mrb_int r, g, b;
  mrb_get_args(mrb, "iii", &r, &g, &b);
  GPU_ClearRGB(mrb_sdl2_gpu_target_get_ptr(mrb, self), r, g, b);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_target_clear_rgba(mrb_state *mrb, mrb_value self) {
  mrb_int r, g, b, a;
  mrb_get_args(mrb, "iii", &r, &g, &b, &a);
  GPU_ClearRGBA(mrb_sdl2_gpu_target_get_ptr(mrb, self), r, g, b, a);
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_target_flip(mrb_state *mrb, mrb_value self) {
  GPU_Flip(mrb_sdl2_gpu_target_get_ptr(mrb, self));
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_flush_blit_buffer(mrb_state *mrb, mrb_value self) {
  GPU_FlushBlitBuffer();
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_gpu_target_blit(mrb_state *mrb, mrb_value self) {
  mrb_float x, y;
  mrb_value src_rect, src_image;
  GPU_Target *t;
  GPU_Image *i;
  GPU_Rect *r;
  if (4 == mrb->c->ci->argc) {
    mrb_get_args(mrb, "ooff", &src_image, &src_rect, &x, &y);
    t = mrb_sdl2_gpu_target_get_ptr(mrb, self);
    i = mrb_sdl2_gpu_image_get_ptr(mrb, src_image);
    r = mrb_sdl2_gpu_rect_get_ptr(mrb, src_rect);
    GPU_Blit(i, r, t, x, y);
  } else if (5 == mrb->c->ci->argc) {
    mrb_float degrees;
    mrb_get_args(mrb, "oofff", &src_image, &src_rect, &x, &y, &degrees);
    t = mrb_sdl2_gpu_target_get_ptr(mrb, self);
    i = mrb_sdl2_gpu_image_get_ptr(mrb, src_image);
    r = mrb_sdl2_gpu_rect_get_ptr(mrb, src_rect);
    GPU_BlitRotate(i, r, t, x, y, degrees);
  } else if (6 == mrb->c->ci->argc) {
    mrb_float scale_x, scale_y;
    mrb_get_args(mrb, "ooffff", &src_image, &src_rect, &x, &y,
                                &scale_x, &scale_y);
    t = mrb_sdl2_gpu_target_get_ptr(mrb, self);
    i = mrb_sdl2_gpu_image_get_ptr(mrb, src_image);
    r = mrb_sdl2_gpu_rect_get_ptr(mrb, src_rect);
    GPU_BlitScale(i, r, t, x, y, scale_x, scale_y);
  } else if (7 == mrb->c->ci->argc) {
    mrb_float degrees, scale_x, scale_y;
    mrb_get_args(mrb, "ooffff", &src_image, &src_rect, &x, &y,
                                &scale_x, &scale_y, &degrees);
    t = mrb_sdl2_gpu_target_get_ptr(mrb, self);
    i = mrb_sdl2_gpu_image_get_ptr(mrb, src_image);
    r = mrb_sdl2_gpu_rect_get_ptr(mrb, src_rect);
    GPU_BlitTransform(i, r, t, x, y, degrees, scale_x, scale_y);
  } else if (9 == mrb->c->ci->argc) {
    mrb_float pivot_x, pivot_y, degrees, scaleX, scaleY;
    mrb_get_args(mrb, "oofffffff", &src_image, &src_rect, &x, &y,
                                   &scaleX, &scaleY, &degrees,
                                   &pivot_x, &pivot_y);
    t = mrb_sdl2_gpu_target_get_ptr(mrb, self);
    i = mrb_sdl2_gpu_image_get_ptr(mrb, src_image);
    r = mrb_sdl2_gpu_rect_get_ptr(mrb, src_rect);
    GPU_BlitTransformX(i, r, t, x, y, pivot_x, pivot_y,
                       degrees, scaleX, scaleY);
  } else {
    mrb_raise(mrb, E_RUNTIME_ERROR,
              "Incorrect number of arguments, expected 4, 5, 6 or 7 arguments");
  }

  return self;
}

static mrb_value
mrb_sdl2_gpu_rendererid_get_name(mrb_state *mrb, mrb_value self) {
  return mrb_str_new_cstr(mrb, mrb_sdl2_gpu_rendererid_get_ptr(mrb, self)
                          ->name);
}

static mrb_value
mrb_sdl2_gpu_rendererid_set_name(mrb_state *mrb, mrb_value self) {
  mrb_value str;
  mrb_get_args(mrb, "s", &str);
  return mrb_str_new_cstr(mrb, (mrb_sdl2_gpu_rendererid_get_ptr(mrb, self)
                                ->name = RSTRING_PTR(str)));
}

static mrb_value
mrb_sdl2_gpu_rendererid_get_renderer(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_rendererid_get_ptr(mrb, self)->renderer);
}

static mrb_value
mrb_sdl2_gpu_rendererid_set_renderer(mrb_state *mrb, mrb_value self) {
  mrb_int renderer;
  mrb_get_args(mrb, "i", &renderer);
  return mrb_fixnum_value((mrb_sdl2_gpu_rendererid_get_ptr(mrb, self)
                           ->renderer = renderer));
}

static mrb_value
mrb_sdl2_gpu_image_renderer(mrb_state *mrb, mrb_value self) {
  return mrb_sdl2_gpu_renderer(mrb, (mrb_sdl2_gpu_image_get_ptr(mrb, self)
                                     ->renderer));
}

static mrb_value
mrb_sdl2_gpu_image_target(mrb_state *mrb, mrb_value self) {
  return mrb_sdl2_gpu_target(mrb, (mrb_sdl2_gpu_image_get_ptr(mrb, self)
                                   ->target));
}

static mrb_value
mrb_sdl2_gpu_image_w(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_image_get_ptr(mrb, self)->w);
}

static mrb_value
mrb_sdl2_gpu_image_h(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_image_get_ptr(mrb, self)->h);
}

static mrb_value
mrb_sdl2_gpu_image_set_w(mrb_state *mrb, mrb_value self) {
  mrb_int w;
  mrb_get_args(mrb, "i", &w);
  return mrb_fixnum_value((mrb_sdl2_gpu_image_get_ptr(mrb, self)->w = w));
}

static mrb_value
mrb_sdl2_gpu_image_set_h(mrb_state *mrb, mrb_value self) {
  mrb_int h;
  mrb_get_args(mrb, "i", &h);
  return mrb_fixnum_value((mrb_sdl2_gpu_image_get_ptr(mrb, self)->h = h));
}

static mrb_value
mrb_sdl2_gpu_image_using_vr(mrb_state *mrb, mrb_value self) {
  return (0 != mrb_sdl2_gpu_image_get_ptr(mrb, self)
          ->using_virtual_resolution) ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_sdl2_gpu_image_set_using_vr(mrb_state *mrb, mrb_value self) {
  mrb_bool bool_var;
  mrb_get_args(mrb, "b", &bool_var);
  return (0 != (mrb_sdl2_gpu_image_get_ptr(mrb, self)
                ->using_virtual_resolution = bool_var)) ?
      mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_sdl2_gpu_image_base_w(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_image_get_ptr(mrb, self)->base_w);
}

static mrb_value
mrb_sdl2_gpu_image_base_h(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_image_get_ptr(mrb, self)->base_h);
}

static mrb_value
mrb_sdl2_gpu_image_set_base_w(mrb_state *mrb, mrb_value self) {
  mrb_int w;
  mrb_get_args(mrb, "i", &w);
  return mrb_fixnum_value((mrb_sdl2_gpu_image_get_ptr(mrb, self)->base_w = w));
}

static mrb_value
mrb_sdl2_gpu_image_set_base_h(mrb_state *mrb, mrb_value self) {
  mrb_int h;
  mrb_get_args(mrb, "i", &h);
  return mrb_fixnum_value((mrb_sdl2_gpu_image_get_ptr(mrb, self)->base_h = h));
}

static mrb_value
mrb_sdl2_gpu_image_format(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_image_get_ptr(mrb, self)->format);
}

static mrb_value
mrb_sdl2_gpu_image_set_format(mrb_state *mrb, mrb_value self) {
  mrb_int format;
  mrb_get_args(mrb, "i", &format);
  return mrb_fixnum_value((mrb_sdl2_gpu_image_get_ptr(mrb, self)
                           ->format = format));
}

static mrb_value
mrb_sdl2_gpu_image_num_layers(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_image_get_ptr(mrb, self)->num_layers);
}

static mrb_value
mrb_sdl2_gpu_image_set_num_layers(mrb_state *mrb, mrb_value self) {
  mrb_int layers;
  mrb_get_args(mrb, "i", &layers);
  return mrb_fixnum_value((mrb_sdl2_gpu_image_get_ptr(mrb, self)
                           ->num_layers = layers));
}

static mrb_value
mrb_sdl2_gpu_image_bytes_per_pixel(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_image_get_ptr(mrb, self)
                          ->bytes_per_pixel);
}

static mrb_value
mrb_sdl2_gpu_image_set_bytes_per_pixel(mrb_state *mrb, mrb_value self) {
  mrb_int bpp;
  mrb_get_args(mrb, "i", &bpp);
  return mrb_fixnum_value((mrb_sdl2_gpu_image_get_ptr(mrb, self)
                           ->bytes_per_pixel = bpp));
}

static mrb_value
mrb_sdl2_gpu_image_texture_w(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_image_get_ptr(mrb, self)->texture_w);
}

static mrb_value
mrb_sdl2_gpu_image_texture_h(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_image_get_ptr(mrb, self)->texture_h);
}

static mrb_value
mrb_sdl2_gpu_image_set_texture_w(mrb_state *mrb, mrb_value self) {
  mrb_int w;
  mrb_get_args(mrb, "i", &w);
  return mrb_fixnum_value((mrb_sdl2_gpu_image_get_ptr(mrb, self)
                           ->texture_w = w));
}

static mrb_value
mrb_sdl2_gpu_image_set_texture_h(mrb_state *mrb, mrb_value self) {
  mrb_int h;
  mrb_get_args(mrb, "i", &h);
  return mrb_fixnum_value((mrb_sdl2_gpu_image_get_ptr(mrb, self)
                           ->texture_h = h));
}

static mrb_value
mrb_sdl2_gpu_image_has_mipmaps(mrb_state *mrb, mrb_value self) {
  return (0 != mrb_sdl2_gpu_image_get_ptr(mrb, self)->has_mipmaps) ?
      mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_sdl2_gpu_image_set_has_mipmaps(mrb_state *mrb, mrb_value self) {
  mrb_bool bool_var;
  mrb_get_args(mrb, "b", &bool_var);
  return (0 != (mrb_sdl2_gpu_image_get_ptr(mrb, self)->has_mipmaps =
                bool_var)) ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_sdl2_gpu_image_use_blending(mrb_state *mrb, mrb_value self) {
  return (0 != mrb_sdl2_gpu_image_get_ptr(mrb, self)->
          use_blending) ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_sdl2_gpu_image_set_use_blending(mrb_state *mrb, mrb_value self) {
  mrb_bool bool_var;
  mrb_get_args(mrb, "b", &bool_var);
  return 0 != (mrb_sdl2_gpu_image_get_ptr(mrb, self)->
               use_blending = bool_var) ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_sdl2_gpu_image_set_renderer(mrb_state *mrb, mrb_value self) {
  mrb_value renderer;
  mrb_get_args(mrb, "o", &renderer);
  return
  mrb_sdl2_gpu_renderer(mrb,
                        (mrb_sdl2_gpu_image_get_ptr(mrb, self)
                         ->renderer =
                         mrb_sdl2_gpu_renderer_get_ptr(mrb, renderer)));
}

static mrb_value
mrb_sdl2_gpu_image_set_target(mrb_state *mrb, mrb_value self) {
  mrb_value target;
  mrb_get_args(mrb, "o", &target);
  return
  mrb_sdl2_gpu_target(mrb,
                      (mrb_sdl2_gpu_image_get_ptr(mrb, self)
                       ->target =
                       mrb_sdl2_gpu_target_get_ptr(mrb, target)));
}

static mrb_value
mrb_sdl2_gpu_image_is_alias(mrb_state *mrb, mrb_value self) {
  return
  (0 != mrb_sdl2_gpu_image_get_ptr(mrb, self)
       ->is_alias) ? mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_sdl2_gpu_image_set_is_alias(mrb_state *mrb, mrb_value self) {
  mrb_bool bool_var;
  mrb_get_args(mrb, "b", &bool_var);
  return
  (0 != (mrb_sdl2_gpu_image_get_ptr(mrb, self)->is_alias = bool_var)) ?
      mrb_true_value() : mrb_false_value();
}

static mrb_value
mrb_sdl2_gpu_image_blend_mode(mrb_state *mrb, mrb_value self) {
  return
  mrb_sdl2_gpu_blendmode(mrb,
                         &(mrb_sdl2_gpu_image_get_ptr(mrb, self)->blend_mode));
}

static mrb_value
mrb_sdl2_gpu_image_filter_mode(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_image_get_ptr(mrb, self)->filter_mode);
}

static mrb_value
mrb_sdl2_gpu_image_set_filter_mode(mrb_state *mrb, mrb_value self) {
  mrb_int filter_mode;
  mrb_get_args(mrb, "i", &filter_mode);
  return
      mrb_fixnum_value((mrb_sdl2_gpu_image_get_ptr(mrb, self)
                        ->filter_mode = filter_mode));
}

static mrb_value
mrb_sdl2_gpu_image_snap_mode(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_image_get_ptr(mrb, self)->snap_mode);
}

static mrb_value
mrb_sdl2_gpu_image_wrap_mode_x(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_image_get_ptr(mrb, self)->wrap_mode_x);
}

static mrb_value
mrb_sdl2_gpu_image_set_wrap_mode_x(mrb_state *mrb, mrb_value self) {
  mrb_int mode;
  mrb_get_args(mrb, "i", &mode);
  return mrb_fixnum_value((mrb_sdl2_gpu_image_get_ptr(mrb, self)
                           ->wrap_mode_x = mode));
}

static mrb_value
mrb_sdl2_gpu_image_wrap_mode_y(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_image_get_ptr(mrb, self)->wrap_mode_y);
}

static mrb_value
mrb_sdl2_gpu_image_set_wrap_mode_y(mrb_state *mrb, mrb_value self) {
  mrb_int mode;
  mrb_get_args(mrb, "i", &mode);
  return mrb_fixnum_value((mrb_sdl2_gpu_image_get_ptr(mrb, self)
                           ->wrap_mode_y = mode));
}

static mrb_value
mrb_sdl2_gpu_image_refcount(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_image_get_ptr(mrb, self)->refcount);
}

static mrb_value
mrb_sdl2_gpu_image_set_refcount(mrb_state *mrb, mrb_value self) {
  mrb_int refcount;
  mrb_get_args(mrb, "i", &refcount);
  return mrb_fixnum_value((mrb_sdl2_gpu_image_get_ptr(mrb, self)
                           ->refcount = refcount));
}

static mrb_value
mrb_sdl2_gpu_blendmode_source_color(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_blendmode_get_ptr(mrb, self)
                          ->source_color);
}

static mrb_value
mrb_sdl2_gpu_blendmode_set_source_color(mrb_state *mrb, mrb_value self) {
  mrb_int color;
  mrb_get_args(mrb, "i", &color);
  return mrb_fixnum_value((mrb_sdl2_gpu_blendmode_get_ptr(mrb, self)
                           ->source_color = color));
}

static mrb_value
mrb_sdl2_gpu_blendmode_source_alpha(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_blendmode_get_ptr(mrb, self)
                          ->source_alpha);
}

static mrb_value
mrb_sdl2_gpu_blendmode_set_source_alpha(mrb_state *mrb, mrb_value self) {
  mrb_int alpha;
  mrb_get_args(mrb, "i", &alpha);
  return mrb_fixnum_value((mrb_sdl2_gpu_blendmode_get_ptr(mrb, self)
                           ->source_alpha = alpha));
}

static mrb_value
mrb_sdl2_gpu_blendmode_dest_color(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_blendmode_get_ptr(mrb, self)
                          ->dest_color);
}

static mrb_value
mrb_sdl2_gpu_blendmode_set_dest_color(mrb_state *mrb, mrb_value self) {
  mrb_int color;
  mrb_get_args(mrb, "i", &color);
  return mrb_fixnum_value((mrb_sdl2_gpu_blendmode_get_ptr(mrb, self)
                           ->dest_color = color));
}

static mrb_value
mrb_sdl2_gpu_blendmode_dest_alpha(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_blendmode_get_ptr(mrb, self)
                          ->dest_alpha);
}

static mrb_value
mrb_sdl2_gpu_blendmode_set_dest_alpha(mrb_state *mrb, mrb_value self) {
  mrb_int alpha;
  mrb_get_args(mrb, "i", &alpha);
  return mrb_fixnum_value((mrb_sdl2_gpu_blendmode_get_ptr(mrb, self)
                           ->dest_alpha = alpha));
}

static mrb_value
mrb_sdl2_gpu_blendmode_color_equation(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_blendmode_get_ptr(mrb, self)
                          ->color_equation);
}

static mrb_value
mrb_sdl2_gpu_blendmode_set_color_equation(mrb_state *mrb, mrb_value self) {
  mrb_int color_equation;
  mrb_get_args(mrb, "i", &color_equation);
  return mrb_fixnum_value((mrb_sdl2_gpu_blendmode_get_ptr(mrb, self)
                           ->color_equation = color_equation));
}

static mrb_value
mrb_sdl2_gpu_blendmode_alpha_equation(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_blendmode_get_ptr(mrb, self)
                          ->alpha_equation);
}

static mrb_value
mrb_sdl2_gpu_blendmode_set_alpha_equation(mrb_state *mrb, mrb_value self) {
  mrb_int alpha_equation;
  mrb_get_args(mrb, "i", &alpha_equation);
  return mrb_fixnum_value((mrb_sdl2_gpu_blendmode_get_ptr(mrb, self)
                           ->alpha_equation = alpha_equation));
}

static mrb_value
mrb_sdl2_gpu_rect_init(mrb_state *mrb, mrb_value self) {
  mrb_float x, y, w, h;
  int const argc = mrb_get_args(mrb, "|ffff", &x, &y, &w, &h);
  mrb_sdl2_gpu_rect_data_t *data =
    (mrb_sdl2_gpu_rect_data_t*)DATA_PTR(self);
  if (data == NULL) {
    data = (mrb_sdl2_gpu_rect_data_t*)
        mrb_malloc(mrb, sizeof(mrb_sdl2_gpu_rect_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
  }

  switch (argc) {
  case 0:
    data->rect.x = 0;
    data->rect.y = 0;
    data->rect.w = 0;
    data->rect.h = 0;
    break;
  case 1:
    data->rect.x = x;
    data->rect.y = 0;
    data->rect.w = 0;
    data->rect.h = 0;
    break;
  case 2:
    data->rect.x = x;
    data->rect.y = y;
    data->rect.w = 0;
    data->rect.h = 0;
    break;
  case 3:
    data->rect.x = x;
    data->rect.y = y;
    data->rect.w = w;
    data->rect.h = 0;
    break;
  case 4:
    data->rect.x = x;
    data->rect.y = y;
    data->rect.w = w;
    data->rect.h = h;
    break;
  }

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_gpu_rect_data_type;

  return self;
}

static mrb_value
mrb_sdl2_gpu_rect_get_x(mrb_state *mrb, mrb_value self) {
  GPU_Rect *r = mrb_sdl2_gpu_rect_get_ptr(mrb, self);
  return mrb_fixnum_value(r->x);
}

static mrb_value
mrb_sdl2_gpu_rect_get_y(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_rect_get_ptr(mrb, self)->y);
}

static mrb_value
mrb_sdl2_gpu_rect_get_w(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_rect_get_ptr(mrb, self)->w);
}

static mrb_value
mrb_sdl2_gpu_rect_get_h(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(mrb_sdl2_gpu_rect_get_ptr(mrb, self)->h);
}

static mrb_value
mrb_sdl2_gpu_rect_set_x(mrb_state *mrb, mrb_value self) {
  mrb_int x;
  mrb_get_args(mrb, "i", &x);
  return mrb_fixnum_value((mrb_sdl2_gpu_rect_get_ptr(mrb, self)->x = x));
}

static mrb_value
mrb_sdl2_gpu_rect_set_y(mrb_state *mrb, mrb_value self) {
  mrb_float y;
  mrb_get_args(mrb, "f", &y);
  return mrb_fixnum_value((mrb_sdl2_gpu_rect_get_ptr(mrb, self)->y = y));
}

static mrb_value
mrb_sdl2_gpu_rect_set_w(mrb_state *mrb, mrb_value self) {
  mrb_float w;
  mrb_get_args(mrb, "f", &w);
  return mrb_fixnum_value((mrb_sdl2_gpu_rect_get_ptr(mrb, self)->w = w));
}

static mrb_value
mrb_sdl2_gpu_rect_set_h(mrb_state *mrb, mrb_value self) {
  mrb_float h;
  mrb_get_args(mrb, "f", &h);
  return mrb_fixnum_value((mrb_sdl2_gpu_rect_get_ptr(mrb, self)->h = h));
}

static mrb_value
mrb_sdl2_gpu_target_pixel(mrb_state *mrb, mrb_value self) {
  mrb_float x, y;
  mrb_int r, g, b, a;
  GPU_Target *t = NULL;
  mrb_get_args(mrb, "ffiiii", &x, &y, &r, &g, &b, &a);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);

  if (NULL == t)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Target's ptr");

  GPU_Pixel(t, x, y, (SDL_Color) {r, g, b, a});
  return self;
}

static mrb_value
mrb_sdl2_gpu_target_line(mrb_state *mrb, mrb_value self) {
  mrb_float x1, y1, x2, y2;
  mrb_int r, g, b, a;
  GPU_Target *t = NULL;
  mrb_get_args(mrb, "ffffiiii", &x1, &y1, &x2, &y2, &r, &g, &b, &a);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);

  if (NULL == t)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Target's ptr");

  GPU_Line(t, x1, y1, x2, y2, (SDL_Color) {r, g, b, a});
  return self;
}

static mrb_value
mrb_sdl2_gpu_target_arc(mrb_state *mrb, mrb_value self) {
  mrb_float x, y, radius, start_angle, end_angle;
  mrb_int r, g, b, a;
  GPU_Target *t = NULL;
  mrb_get_args(mrb, "fffffiiii", &x, &y, &radius, &start_angle,
                                &end_angle, &r, &g, &b, &a);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);

  if (NULL == t)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Target's ptr");

  GPU_Arc(t, x, y, radius, start_angle, end_angle, (SDL_Color) {r, g, b, a});
  return self;
}

static mrb_value
mrb_sdl2_gpu_target_arc_filled(mrb_state *mrb, mrb_value self) {
  mrb_float x, y, radius, start_angle, end_angle;
  mrb_int r, g, b, a;
  GPU_Target *t = NULL;
  mrb_get_args(mrb, "fffffiiii", &x, &y, &radius, &start_angle,
                                &end_angle, &r, &g, &b, &a);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);

  if (NULL == t)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Target's ptr");

  GPU_ArcFilled(t, x, y, radius, start_angle,
                end_angle, (SDL_Color) {r, g, b, a});
  return self;
}

static mrb_value
mrb_sdl2_gpu_target_circle(mrb_state *mrb, mrb_value self) {
  mrb_float x, y, radius;
  mrb_int r, g, b, a;
  GPU_Target *t = NULL;
  mrb_get_args(mrb, "fffiiii", &x, &y, &radius, &r, &g, &b, &a);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);

  if (NULL == t)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Target's ptr");

  GPU_Circle(t, x, y, radius, (SDL_Color) {r, g, b, a});
  return self;
}

static mrb_value
mrb_sdl2_gpu_target_circle_filled(mrb_state *mrb, mrb_value self) {
  mrb_float x, y, radius;
  mrb_int r, g, b, a;
  GPU_Target *t = NULL;
  mrb_get_args(mrb, "fffiiii", &x, &y, &radius, &r, &g, &b, &a);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);

  if (NULL == t)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Target's ptr");

  GPU_CircleFilled(t, x, y, radius, (SDL_Color) {r, g, b, a});
  return self;
}

static mrb_value
mrb_sdl2_gpu_target_ellipse(mrb_state *mrb, mrb_value self) {
  mrb_float x, y, rx, ry, degree;
  mrb_int r, g, b, a;
  GPU_Target *t = NULL;
  mrb_get_args(mrb, "fffffiiii", &x, &y, &rx, &ry, &degree, &r, &g, &b, &a);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);

  if (NULL == t)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Target's ptr");

  GPU_Ellipse(t, x, y, rx, ry, degree, (SDL_Color) {r, g, b, a});
  return self;
}

static mrb_value
mrb_sdl2_gpu_target_ellipse_filled(mrb_state *mrb, mrb_value self) {
  mrb_float x, y, rx, ry, degree;
  mrb_int r, g, b, a;
  GPU_Target *t = NULL;
  mrb_get_args(mrb, "fffffiiii", &x, &y, &rx, &ry, &degree, &r, &g, &b, &a);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);

  if (NULL == t)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Target's ptr");

  GPU_EllipseFilled(t, x, y, rx, ry, degree, (SDL_Color) {r, g, b, a});
  return self;
}

static mrb_value
mrb_sdl2_gpu_target_sector(mrb_state *mrb, mrb_value self) {
  mrb_float x, y, inner_radius, outer_radius, start_angle, end_angle;
  mrb_int r, g, b, a;
  GPU_Target *t = NULL;
  mrb_get_args(mrb, "ffffffiiii", &x, &y, &inner_radius, &outer_radius,
                                  &end_angle, &start_angle, &r, &g, &b, &a);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);

  if (NULL == t)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Target's ptr");

  GPU_Sector(t, x, y, inner_radius, outer_radius,
             start_angle, end_angle, (SDL_Color) {r, g, b, a});
  return self;
}

static mrb_value
mrb_sdl2_gpu_target_sector_filled(mrb_state *mrb, mrb_value self) {
  mrb_float x, y, inner_radius, outer_radius, start_angle, end_angle;
  mrb_int r, g, b, a;
  GPU_Target *t = NULL;
  mrb_get_args(mrb, "ffffffiiii", &x, &y, &inner_radius, &outer_radius,
                                  &end_angle, &start_angle, &r, &g, &b, &a);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);

  if (NULL == t)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Target's ptr");

  GPU_SectorFilled(t, x, y, inner_radius, outer_radius,
             start_angle, end_angle, (SDL_Color) {r, g, b, a});
  return self;
}

static mrb_value
mrb_sdl2_gpu_target_tri(mrb_state *mrb, mrb_value self) {
  mrb_float x1, y1, x2, y2, x3, y3;
  mrb_int r, g, b, a;
  GPU_Target *t = NULL;
  mrb_get_args(mrb, "ffffffiiii", &x1, &y1, &x2, &y2, &x3, &y3,
                                  &r, &g, &b, &a);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);

  if (NULL == t)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Target's ptr");

  GPU_Tri(t, x1, y1, x2, y2, x3, y3, (SDL_Color) {r, g, b, a});
  return self;
}

static mrb_value
mrb_sdl2_gpu_target_tri_filled(mrb_state *mrb, mrb_value self) {
  mrb_float x1, y1, x2, y2, x3, y3;
  mrb_int r, g, b, a;
  GPU_Target *t = NULL;
  mrb_get_args(mrb, "ffffffiiii", &x1, &y1, &x2, &y2, &x3, &y3,
                                  &r, &g, &b, &a);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);

  if (NULL == t)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Target's ptr");

  GPU_TriFilled(t, x1, y1, x2, y2, x3, y3, (SDL_Color) {r, g, b, a});
  return self;
}

static mrb_value
mrb_sdl2_gpu_target_rect(mrb_state *mrb, mrb_value self) {
  mrb_float x1, y1, x2, y2;
  mrb_int   r, g, b, a;
  GPU_Target *t = NULL;
  mrb_get_args(mrb, "ffffiiii", &x1, &y1, &x2, &y2, &r, &g, &b, &a);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);

  if (NULL == t)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Target's ptr");

  GPU_Rectangle(t, x1, y1, x2, y2, (SDL_Color) {r, g, b, a});
  return self;
}

static mrb_value
mrb_sdl2_gpu_target_rect2(mrb_state *mrb, mrb_value self) {
  mrb_value rect;
  mrb_int   r, g, b, a;
  GPU_Target *t = NULL;
  GPU_Rect *re;
  mrb_get_args(mrb, "oiiii", &rect, &r, &g, &b, &a);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);
  re = mrb_sdl2_gpu_rect_get_ptr(mrb, rect);
  if (NULL == t)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Target's ptr");
  if (NULL == re)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Rect's ptr");
  GPU_Rectangle2(t, *re, (SDL_Color) {r, g, b, a});
  return self;
}

static mrb_value
mrb_sdl2_gpu_target_rect_filled(mrb_state *mrb, mrb_value self) {
  mrb_float x1, y1, x2, y2;
  mrb_int   r, g, b, a;
  GPU_Target *t = NULL;
  mrb_get_args(mrb, "ffffiiii", &x1, &y1, &x2, &y2, &r, &g, &b, &a);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);

  if (NULL == t)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Target's ptr");

  GPU_RectangleFilled(t, x1, y1, x2, y2, (SDL_Color) {r, g, b, a});
  return self;
}

static mrb_value
mrb_sdl2_gpu_target_rect_filled2(mrb_state *mrb, mrb_value self) {
  mrb_value rect;
  mrb_int   r, g, b, a;
  GPU_Target *t = NULL;
  GPU_Rect *re;
  mrb_get_args(mrb, "oiiii", &rect, &r, &g, &b, &a);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);
  re = mrb_sdl2_gpu_rect_get_ptr(mrb, rect);
  
  if (NULL == t)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Target's ptr");

  if (NULL == re)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Rect's ptr");

  GPU_RectangleFilled2(t, *re, (SDL_Color) {r, g, b, a});
  return self;
}

static mrb_value
mrb_sdl2_gpu_target_rect_round(mrb_state *mrb, mrb_value self) {
  mrb_value *args;
  mrb_int argc;
  GPU_Target *t = NULL;
  mrb_get_args(mrb, "*", &args, &argc);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);

  if (NULL == t)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Target's ptr");
  if (8 == argc) {
    mrb_float x1     = mrb_float(args[0]),
              y1     = mrb_float(args[1]),
              x2     = mrb_float(args[2]),
              y2     = mrb_float(args[3]),
              radius = mrb_float(args[4]);
    mrb_int r = mrb_fixnum(args[5]),
            g = mrb_fixnum(args[6]),
            b = mrb_fixnum(args[7]),
            a = mrb_fixnum(args[8]);
    GPU_RectangleRound(t, x1, y1, x2, y2,
                       radius, (SDL_Color) {r, g, b, a});
  } else if (5 == argc) {
    mrb_int r = mrb_fixnum(args[2]),
            g = mrb_fixnum(args[3]),
            b = mrb_fixnum(args[4]),
            a = mrb_fixnum(args[5]);
    GPU_Rect *re      = mrb_sdl2_gpu_rect_get_ptr(mrb, args[0]);
    mrb_float radius = mrb_float(args[1]);
    GPU_RectangleRound2(t, *re, radius, (SDL_Color) {r, g, b, a});
  } else {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "unexpected arguments");
  }
  return self;
}


static mrb_value
mrb_sdl2_gpu_target_rect_round_filled(mrb_state *mrb, mrb_value self) {
  mrb_value *args;
  mrb_int argc;
  GPU_Target *t = NULL;
  mrb_get_args(mrb, "*", &args, &argc);
  t = mrb_sdl2_gpu_target_get_ptr(mrb, self);

  if (NULL == t)
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get the Target's ptr");
  if (8 == argc) {
    mrb_float x1     = mrb_float(args[0]),
              y1     = mrb_float(args[1]),
              x2     = mrb_float(args[2]),
              y2     = mrb_float(args[3]),
              radius = mrb_float(args[4]);
    mrb_int r = mrb_fixnum(args[5]),
            g = mrb_fixnum(args[6]),
            b = mrb_fixnum(args[7]),
            a = mrb_fixnum(args[8]);
    GPU_RectangleRoundFilled(t, x1, y1, x2, y2,
                       radius, (SDL_Color) {r, g, b, a});
  } else if (5 == argc) {
    mrb_int r = mrb_fixnum(args[2]),
            g = mrb_fixnum(args[3]),
            b = mrb_fixnum(args[4]),
            a = mrb_fixnum(args[5]);
    GPU_Rect *re      = mrb_sdl2_gpu_rect_get_ptr(mrb, args[0]);
    mrb_float radius = mrb_float(args[1]);
    GPU_RectangleRoundFilled2(t, *re, radius, (SDL_Color) {r, g, b, a});
  } else {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "unexpected arguments");
  }
  return self;
}

static mrb_value
mrb_sdl2_gpu_target_gradient_fill_rect(mrb_state *mrb, mrb_value self) {
  GPU_Target *target;
  mrb_int r1, r2, g1, g2, b1, b2, a1, a2;
  mrb_value rect;
  GPU_Rect *re = NULL;
  mrb_bool vertical;
  float i;
  int difr, difg, difb, difa;
  mrb_get_args(mrb, "iiiiiiiiob", &r1, &g1, &b1, &a1,
                                  &r2, &g2, &b2, &a2,
                                  &rect, &vertical);
  target = mrb_sdl2_gpu_target_get_ptr(mrb, self);
  re = mrb_sdl2_gpu_rect_get_ptr(mrb, rect);

  if (target == NULL || re == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "Could not get target or rectangle");
  }

  difr = r2 - r1;
  difg = g2 - g1;
  difb = b2 - b1;
  difa = a2 - a1;
  if (vertical) {
    int r, g, b, a;
    for (i = 0; i < re->h; i+=1.0) {
      GPU_Rect rec;
      float mod = i / re->h;
      rec.x = re->x;
      rec.y = re->y + i;
      rec.w = re->w;
      rec.h = 1;
      r = r1 + difr * mod;
      g = g1 + difg * mod;
      b = b1 + difb * mod;
      a = a1 + difa * mod;
      GPU_RectangleFilled2(target, rec, (SDL_Color) {r, g, b, a});
    }
  } else {
    int r, g, b, a;
    for (i = 0; i < re->w; i+=1.0) {
      GPU_Rect rec;
      float mod = i / re->w;
      rec.x = re->x + i;
      rec.y = re->y;
      rec.w = 1;
      rec.h = re->h;
      r = r1 + difr * mod;
      g = g1 + difg * mod;
      b = b1 + difb * mod;
      a = a1 + difa * mod;
      GPU_RectangleFilled2(target, rec, (SDL_Color) {r, g, b, a});
    }
  }

  return self;
}
static mrb_value
mrb_sdl2_gpu_target_get_rgba(mrb_state *mrb, mrb_value self) {
  mrb_value ary;
  GPU_Target *t = mrb_sdl2_gpu_target_get_ptr(mrb, self);
  ary = mrb_ary_new_capa(mrb, 4);
  mrb_ary_push(mrb, ary, mrb_fixnum_value(t->color.r));
  mrb_ary_push(mrb, ary, mrb_fixnum_value(t->color.g));
  mrb_ary_push(mrb, ary, mrb_fixnum_value(t->color.b));
  mrb_ary_push(mrb, ary, mrb_fixnum_value(t->color.a));
  return ary;
}

static mrb_value
mrb_sdl2_gpu_image_get_rgba(mrb_state *mrb, mrb_value self) {
  mrb_value ary;
  GPU_Image *i = mrb_sdl2_gpu_image_get_ptr(mrb, self);
  ary = mrb_ary_new_capa(mrb, 4);
  mrb_ary_push(mrb, ary, mrb_fixnum_value(i->color.r));
  mrb_ary_push(mrb, ary, mrb_fixnum_value(i->color.g));
  mrb_ary_push(mrb, ary, mrb_fixnum_value(i->color.b));
  mrb_ary_push(mrb, ary, mrb_fixnum_value(i->color.a));
  return ary;
}

static mrb_value
mrb_sdl2_gpu_program_initialize(mrb_state *mrb, mrb_value self) {
  Uint32 programid = -1;
  mrb_value object;
  mrb_value *objects;
  mrb_int rest_args;
  int const argc = mrb_get_args(mrb, "|o*", &object, &objects, &rest_args);
  mrb_sdl2_gpu_program_data_t *data =
    (mrb_sdl2_gpu_program_data_t*)DATA_PTR(self);

  if (NULL == data) {
    data = (mrb_sdl2_gpu_program_data_t*)
        mrb_malloc(mrb, sizeof(mrb_sdl2_gpu_program_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->programid = 0;
  }
  if (0 == argc) {
    programid = GPU_CreateShaderProgram();
  } else if (2 == argc) {
    Uint32 s1 = mrb_sdl2_gpu_shader_get_uint32(mrb, object),
           s2 = mrb_sdl2_gpu_shader_get_uint32(mrb, objects[0]);
    programid = GPU_LinkShaders(s1, s2);
  } else {
    int i;
    Uint32 *p = (Uint32 *) SDL_malloc(sizeof(Uint32) * (rest_args + 1));
    p[0] = mrb_sdl2_gpu_shader_get_uint32(mrb, object);
    for (i = 1; i < (rest_args + 1); i++) {
      p[1] = mrb_sdl2_gpu_shader_get_uint32(mrb, objects[i-1]);
    }
    programid = GPU_LinkManyShaders(p, rest_args+1);
  }
  if (!programid) {
    mrb_raise(mrb, E_RUNTIME_ERROR,
              "Could not initialize Shader Program");
  }
  data->programid = programid;

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_gpu_program_data_type;
  return self;
}

static mrb_value
mrb_sdl2_gpu_program_free(mrb_state *mrb, mrb_value self) {
  mrb_sdl2_gpu_program_data_t *data =
    (mrb_sdl2_gpu_program_data_t*)
      mrb_data_get_ptr(mrb, self, &mrb_sdl2_gpu_program_data_type);
  if (NULL != data) {
    if (0 != data->programid) {
      GPU_FreeShaderProgram(data->programid);
      data->programid = 0;
    }
    mrb_free(mrb, data);
  }
  return self;
}

static mrb_value
mrb_sdl2_gpu_shader_initialize(mrb_state *mrb, mrb_value self) {
  Uint32 shaderid = -1;
  mrb_value shader_source;
  mrb_int shader_type;
  mrb_sdl2_gpu_shader_data_t *data =
    (mrb_sdl2_gpu_shader_data_t*)DATA_PTR(self);
  mrb_get_args(mrb, "iS", &shader_type, &shader_source);

  if (NULL == data) {
    data = (mrb_sdl2_gpu_shader_data_t*)
        mrb_malloc(mrb, sizeof(mrb_sdl2_gpu_shader_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->shaderid = 0;
  }

  shaderid = GPU_CompileShader(shader_type, RSTRING_PTR(shader_source));
  if (!shaderid) {
    mrb_raise(mrb, E_RUNTIME_ERROR,
              "Could not initialize Shader");
  }
  data->shaderid = shaderid;

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_gpu_shader_data_type;
  return self;
}

static mrb_value
mrb_sdl2_gpu_shader_free(mrb_state *mrb, mrb_value self) {
  mrb_sdl2_gpu_shader_data_t *data =
    (mrb_sdl2_gpu_shader_data_t*)
      mrb_data_get_ptr(mrb, self, &mrb_sdl2_gpu_shader_data_type);
  if (NULL != data) {
    if (0 != data->shaderid) {
      GPU_FreeShader(data->shaderid);
      data->shaderid = 0;
    }
    mrb_free(mrb, data);
  }
  return self;
}

static mrb_value
mrb_sdl2_gpu_program_attach(mrb_state *mrb, mrb_value self) {
  mrb_value shader_object;
  mrb_get_args(mrb, "o", &shader_object);
  GPU_AttachShader(mrb_sdl2_gpu_program_get_uint32(mrb, self),
                   mrb_sdl2_gpu_shader_get_uint32(mrb, shader_object));
  return self;
}

static mrb_value
mrb_sdl2_gpu_program_detach(mrb_state *mrb, mrb_value self) {
  mrb_value shader_object;
  mrb_get_args(mrb, "o", &shader_object);
  GPU_DetachShader(mrb_sdl2_gpu_program_get_uint32(mrb, self),
                   mrb_sdl2_gpu_shader_get_uint32(mrb, shader_object));
  return self;
}

static mrb_value
mrb_sdl2_gpu_program_link(mrb_state *mrb, mrb_value self) {
  GPU_LinkShaderProgram(mrb_sdl2_gpu_program_get_uint32(mrb, self));
  return self;
}

static mrb_value
mrb_sdl2_gpu_get_current_shader_program(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(GPU_GetCurrentShaderProgram());
}

static mrb_value
mrb_sdl2_gpu_program_is_default(mrb_state *mrb, mrb_value self) {
  Uint32 program_uint = mrb_sdl2_gpu_program_get_uint32(mrb, self);
  return 0 == GPU_IsDefaultShaderProgram(program_uint) ?
      mrb_false_value() : mrb_false_value();
}

static mrb_value
mrb_sdl2_gpu_program_activate(mrb_state *mrb, mrb_value self) {
  mrb_value block;
  GPU_ShaderBlock *shaderblock = NULL;
  int argc = mrb_get_args(mrb, "|o", &block);
  if (1 == argc)
    shaderblock = mrb_sdl2_gpu_shaderblock_get_ptr(mrb, block);
  GPU_ActivateShaderProgram(mrb_sdl2_gpu_program_get_uint32(mrb, self),
                            shaderblock);
  return self;
}

static mrb_value
mrb_sdl2_gpu_program_deactivate(mrb_state *mrb, mrb_value self) {
  GPU_DeactivateShaderProgram();
  return self;
}

static mrb_value
mrb_sdl2_gpu_shader_get_msg(mrb_state *mrb, mrb_value self) {
  const char * msg = GPU_GetShaderMessage();
  if (NULL == msg)
    msg = "";
  return mrb_str_new_cstr(mrb, msg);
}

static mrb_value
mrb_sdl2_gpu_program_get_arg_location(mrb_state *mrb, mrb_value self) {
  mrb_value attribute_name;
  mrb_get_args(mrb, "S", &attribute_name);
  return mrb_fixnum_value(
      GPU_GetAttributeLocation(mrb_sdl2_gpu_program_get_uint32(mrb, self),
                               RSTRING_PTR(attribute_name)));
}

static mrb_value
mrb_sdl2_gpu_attributeformat_initialize(mrb_state *mrb, mrb_value self) {
  mrb_int num_elements_per_vertex, stride_bytes, offset_bytes, type;
  mrb_bool normalized;
  mrb_get_args(mrb, "iibii", &num_elements_per_vertex, &type, &normalized,
                             &stride_bytes, &offset_bytes);
  return mrb_sdl2_gpu_attributeformat(mrb,
                                      GPU_MakeAttributeFormat(
                                          num_elements_per_vertex,
                                          type,
                                          normalized,
                                          stride_bytes,
                                          offset_bytes));
}

static mrb_value
mrb_sdl2_gpu_attributeformat_free(mrb_state *mrb, mrb_value self) {
  mrb_sdl2_gpu_attributeformat_data_t *data =
    (mrb_sdl2_gpu_attributeformat_data_t*)
      mrb_data_get_ptr(mrb, self, &mrb_sdl2_gpu_attributeformat_data_type);
  if (NULL != data) {
    mrb_free(mrb, data);
  }
  return self;
}

static mrb_value
mrb_sdl2_gpu_attribute_initialize(mrb_state *mrb, mrb_value self) {
  mrb_int location;
  mrb_value values, format;
  mrb_get_args(mrb, "iAo", &location, &values, &format);
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
}

static mrb_value
mrb_sdl2_gpu_attribute_free(mrb_state *mrb, mrb_value self) {
  mrb_sdl2_gpu_attribute_data_t *data =
    (mrb_sdl2_gpu_attribute_data_t*)
      mrb_data_get_ptr(mrb, self, &mrb_sdl2_gpu_attribute_data_type);
  if (NULL != data) {
    if (NULL != data->attribute) {
      mrb_free(mrb, data->attribute);
    }
    mrb_free(mrb, data);
  }
  return self;
}

static mrb_value
mrb_sdl2_gpu_program_get_uni_location(mrb_state *mrb, mrb_value self) {
  mrb_value uniform_name;
  int result;
  mrb_get_args(mrb, "S", &uniform_name);
  result = GPU_GetUniformLocation(mrb_sdl2_gpu_program_get_uint32(mrb, self),
                                  RSTRING_PTR(uniform_name));
  if (-1 == result) {
    const char *c = GPU_GetShaderMessage();
    if (NULL == c)
      c = "";
    mrb_raisef(mrb, E_RUNTIME_ERROR,  c);
  }
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_gpu_program_load_shader_block(mrb_state *mrb, mrb_value self) {
  mrb_value position_name, texcoord_name, color_name, modelViewMatrix_name;
  mrb_get_args(mrb, "SSSS", &position_name, &texcoord_name,
                            &color_name, &modelViewMatrix_name);
  return mrb_sdl2_gpu_shaderblock(mrb,
                                  GPU_LoadShaderBlock(
                                      mrb_sdl2_gpu_program_get_uint32(mrb,
                                                                      self),
                                      RSTRING_PTR(position_name),
                                      RSTRING_PTR(texcoord_name),
                                      RSTRING_PTR(color_name),
                                      RSTRING_PTR(modelViewMatrix_name)));
}

static mrb_value
mrb_sdl2_gpu_shaderblock_free(mrb_state *mrb, mrb_value self) {
  mrb_sdl2_gpu_shaderblock_data_t *data =
    (mrb_sdl2_gpu_shaderblock_data_t*)
      mrb_data_get_ptr(mrb, self, &mrb_sdl2_gpu_shaderblock_data_type);
  if (NULL != data) {
    mrb_free(mrb, data);
  }
  return self;
}

static mrb_value
mrb_sdl2_gpu_shaderblock_set(mrb_state *mrb, mrb_value self) {
  GPU_SetShaderBlock(*mrb_sdl2_gpu_shaderblock_get_ptr(mrb, self));
  return self;
}

static mrb_value
mrb_sdl2_gpu_image_set(mrb_state *mrb, mrb_value self) {
  mrb_int location, image_unit;
  mrb_get_args(mrb, "ii", &location, &image_unit);
  GPU_SetShaderImage(mrb_sdl2_gpu_image_get_ptr(mrb, self),
                     location, image_unit);
  return self;
}

static mrb_value
mrb_sdl2_gpu_program_get_uniformuiv(mrb_state *mrb, mrb_value self) {
  mrb_int location, arguments_num;
  unsigned int *values;
  int i;
  mrb_value ary;
  mrb_get_args(mrb, "ii", &location, &arguments_num);
  values = (unsigned int *) SDL_malloc(sizeof(unsigned int *) * arguments_num);
  GPU_GetUniformuiv(mrb_sdl2_gpu_program_get_uint32(mrb, self),
                    location,
                    values);
  ary = mrb_ary_new_capa(mrb, arguments_num);
  for (i = 0; i < arguments_num; i++) {
    mrb_ary_push(mrb, ary, mrb_fixnum_value(values[i]));
  }
  return ary;
}

static mrb_value
mrb_sdl2_gpu_set_uniformui(mrb_state *mrb, mrb_value self) {
  mrb_int location, value;
  mrb_get_args(mrb, "ii", &location, &value);
  GPU_SetUniformui(location, value);
  return self;
}

static mrb_value
mrb_sdl2_gpu_set_uniformi(mrb_state *mrb, mrb_value self) {
  mrb_int location, value;
  mrb_get_args(mrb, "ii", &location, &value);
  GPU_SetUniformi(location, value);
  return self;
}

static mrb_value
mrb_sdl2_gpu_set_uniformuiv(mrb_state *mrb, mrb_value self) {
  mrb_int location, num_elements_per_value, num_values;
  mrb_value values;
  unsigned int * values_c;
  int i;
  mrb_get_args(mrb, "iiiA", &location, &num_elements_per_value,
                                       &num_values, &values);
  values_c = (unsigned int *) SDL_malloc(sizeof(unsigned int) *
                                         num_values           *
                                         num_elements_per_value);
  for (i = 0; i < num_values; i++) {
    values_c[i] = mrb_fixnum(mrb_ary_ref(mrb, values, i));
  }
  return self;
}

static mrb_value
mrb_sdl2_gpu_program_get_uniformfv(mrb_state *mrb, mrb_value self) {
  mrb_int location, number_of_values;
  mrb_value ary;
  int i;
  float *values;
  mrb_get_args(mrb, "ii", &location, &number_of_values);
  values = (float *) SDL_malloc(sizeof(float) * number_of_values);
  ary = mrb_ary_new_capa(mrb, number_of_values);
  GPU_GetUniformfv(mrb_sdl2_gpu_program_get_uint32(mrb, self),
                   location, values);
  for (i = 0; i < number_of_values; i++) {
    mrb_ary_push(mrb, ary, mrb_float_value(mrb, values[i]));
  }
  return ary;
}

static mrb_value
mrb_sdl2_gpu_set_uniformf(mrb_state *mrb, mrb_value self) {
  mrb_int location;
  mrb_float value;
  mrb_get_args(mrb, "if", &location, &value);
  GPU_SetUniformf(location, value);
  return self;
}

static mrb_value
mrb_sdl2_gpu_set_uniformfv(mrb_state *mrb, mrb_value self) {
  mrb_int location, num_elements_per_value, num_values;
  mrb_value values;
  float * values_c;
  int i;
  mrb_get_args(mrb, "iiiA", &location, &num_elements_per_value,
                            &num_values, &values);
  values_c = (float *) SDL_malloc(sizeof(float) *
                                  num_values           *
                                  num_elements_per_value);
  for (i = 0; i < num_values; i++) {
    values_c[i] = mrb_float(mrb_ary_ref(mrb, values, i));
  }
  return self;
}

static mrb_value
mrb_sdl2_gpu_program_get_umfv(mrb_state *mrb, mrb_value self) {
  mrb_int location, number_of_values;
  mrb_value ary;
  int i;
  float *values;
  mrb_get_args(mrb, "ii", &location, &number_of_values);
  values = (float *) SDL_malloc(sizeof(float) * number_of_values);
  ary = mrb_ary_new_capa(mrb, number_of_values);
  GPU_GetUniformMatrixfv(mrb_sdl2_gpu_program_get_uint32(mrb, self),
                         location, values);
  for (i = 0; i < number_of_values; i++) {
    mrb_ary_push(mrb, ary, mrb_float_value(mrb, values[i]));
  }
  return ary;
}

static mrb_value
mrb_sdl2_gpu_set_umfv(mrb_state *mrb, mrb_value self) {
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented");
}

static mrb_value
mrb_sdl2_gpu_set_attributef(mrb_state *mrb, mrb_value self) {
  mrb_int location;
  mrb_float value;
  mrb_get_args(mrb, "if", &location, &value);
  GPU_SetAttributef(location, value);
  return self;
}

static mrb_value
mrb_sdl2_gpu_set_attributei(mrb_state *mrb, mrb_value self) {
  mrb_int location;
  mrb_int value;
  mrb_get_args(mrb, "if", &location, &value);
  GPU_SetAttributei(location, value);
  return self;
}

static mrb_value
mrb_sdl2_gpu_set_attributeui(mrb_state *mrb, mrb_value self) {
  mrb_int location;
  mrb_int value;
  mrb_get_args(mrb, "if", &location, &value);
  GPU_SetAttributeui(location, value);
  return self;
}

static mrb_value
mrb_sdl2_gpu_set_attributefv(mrb_state *mrb, mrb_value self) {
  mrb_int location;
  mrb_value values;
  float *values_c;
  int size, i;
  mrb_get_args(mrb, "iA", &location, &values);
  size = mrb_ary_len(mrb, values);
  values_c = (float *) SDL_malloc(sizeof(float) * size);
  for (i = 0; i < size; i++) {
    values_c[i] = mrb_float(mrb_ary_ref(mrb, values, i));
  }
  GPU_SetAttributefv(location, size, values_c);
  return self;
}

static mrb_value
mrb_sdl2_gpu_set_attributeiv(mrb_state *mrb, mrb_value self) {
  mrb_int location;
  mrb_value values;
  int *values_c;
  int size, i;
  mrb_get_args(mrb, "iA", &location, &values);
  size = mrb_ary_len(mrb, values);
  values_c = (int *) SDL_malloc(sizeof(int) * size);
  for (i = 0; i < size; i++) {
    values_c[i] = mrb_float(mrb_ary_ref(mrb, values, i));
  }
  GPU_SetAttributeiv(location, size, values_c);
  return self;
}

static mrb_value
mrb_sdl2_gpu_set_attributeuiv(mrb_state *mrb, mrb_value self) {
  mrb_int location;
  mrb_value values;
  unsigned int *values_c;
  int size, i;
  mrb_get_args(mrb, "iA", &location, &values);
  size = mrb_ary_len(mrb, values);
  values_c = (unsigned int *) SDL_malloc(sizeof(unsigned int) * size);
  for (i = 0; i < size; i++) {
    values_c[i] = mrb_float(mrb_ary_ref(mrb, values, i));
  }
  GPU_SetAttributeuiv(location, size, values_c);
  return self;
}

float *get_floats(mrb_state *mrb, mrb_value self) {
  float * result = NULL;
  mrb_value f;
  int size, i, j, k;

  if (MRB_TT_ARRAY != self.tt)
    return NULL;

  size = RARRAY_LEN(self);  // mrb_ary_len(mrb, self);
  if (mrb_ary_ref(mrb, self, 0).tt == MRB_TT_ARRAY)
    size *= RARRAY_LEN(mrb_ary_ref(mrb, self, 0));

  result = (float *) SDL_malloc(sizeof(float) * size);
  i = 0;
  f = self;
  if (mrb_array_p(mrb_ary_ref(mrb, self, 0))) {
    for (j = 0; j < RARRAY_LEN(f); j++) {
      int k_max = RARRAY_LEN(mrb_ary_ref(mrb, self, 0));
      mrb_value j_value = mrb_ary_ref(mrb, self, j);
      for (k = 0; k < k_max; k++)
        result[i++] = mrb_float(mrb_ary_ref(mrb, j_value, k));
    }
  } else {
    for (j = 0; j < RARRAY_LEN(f); j++) {
      result[i++] = mrb_float(mrb_ary_ref(mrb, self, j));
    }
  }
  return result;
}

unsigned short *get_uint(mrb_state *mrb, mrb_value self) {
  unsigned short * result = NULL;
  mrb_value f;
  int size = 1, i, j, k;

  if (MRB_TT_ARRAY != self.tt)
    return NULL;

  f = self;
  while (mrb_array_p(f)) {
    size *= mrb_ary_len(mrb, f);
    f = mrb_ary_ref(mrb, self, 0);
  }

  result = (unsigned short *) SDL_malloc(sizeof(unsigned short) * size);
  i = 0;
  if (mrb_array_p(mrb_ary_ref(mrb, self, 0))) {
    for (j = 0; j < mrb_ary_len(mrb, self); j++) {
      int k_max = mrb_ary_len(mrb, mrb_ary_ref(mrb, self, 0));
      mrb_value j_value = mrb_ary_ref(mrb, self, j);
      for (k = 0; k < k_max; k++)
        result[i++] = mrb_int(mrb, mrb_ary_ref(mrb, j_value, k));
    }
  } else {
    for (j = 0; j < mrb_ary_len(mrb, self); j++) {
      result[i++] = mrb_int(mrb, mrb_ary_ref(mrb, self, j));
    }
  }
  return result;
}

static mrb_value
mrb_sdl2_gpu_target_blit_batch(mrb_state *mrb, mrb_value self) {
  mrb_value image, indices, values;
  mrb_int batch_flags;
  GPU_Image *image_c = NULL;
  float *values_c;
  unsigned short *indices_c = NULL;
  int i = 0, j;
  mrb_get_args(mrb, "oAA!i", &image, &values, &indices, &batch_flags);

  image_c = mrb_sdl2_gpu_image_get_ptr(mrb, image);

  if (batch_flags & GPU_BATCH_XYZ) i+=3;
  else if (batch_flags & GPU_BATCH_XY) i+=2;
  if (batch_flags & GPU_BATCH_ST) i+=2;
  if (batch_flags & GPU_BATCH_RGBA) i+=4;
  else if (batch_flags & GPU_BATCH_RGB) i+=3;

  values_c = get_floats(mrb, values);
  indices_c = get_uint(mrb, indices);

  j = mrb_ary_len(mrb, values) * mrb_ary_len(mrb, mrb_ary_ref(mrb, values, 0));

  GPU_TriangleBatch(image_c, mrb_sdl2_gpu_target_get_ptr(mrb, self),
                    mrb_ary_len(mrb, values), values_c,
                    mrb_array_p(indices) ? mrb_ary_len(mrb, indices) : 0,
                    indices_c, batch_flags);

  SDL_free(values_c);
  SDL_free(indices_c);

  return self;
}

void mrb_mruby_sdl2_gpu_gem_init(mrb_state *mrb) {
  struct RClass *class_Surface;
  struct RClass *mod_Video;
  int arena_size;

  mod_GPU               = mrb_define_module     (mrb,            "GPU");
  mod_Video             = mrb_module_get_under  (mrb, mod_SDL2,  "Video");
  class_Surface         = mrb_class_get_under   (mrb, mod_Video, "Surface");
  class_Target          = mrb_define_class_under(mrb, mod_GPU,   "Target",          mrb->object_class);
  class_RendererID      = mrb_define_class_under(mrb, mod_GPU,   "RendererID",      mrb->object_class);
  class_Renderer        = mrb_define_class_under(mrb, mod_GPU,   "Renderer",        mrb->object_class);
  class_Context         = mrb_define_class_under(mrb, mod_GPU,   "Context",         mrb->object_class);
  class_Camera          = mrb_define_class_under(mrb, mod_GPU,   "Camera",          mrb->object_class);
  class_BlendMode       = mrb_define_class_under(mrb, mod_GPU,   "BlendMode",       mrb->object_class);
  class_Rect            = mrb_define_class_under(mrb, mod_GPU,   "Rect",            mrb->object_class);
  class_Image           = mrb_define_class_under(mrb, mod_GPU,   "Image",           mrb->object_class);
  class_MatrixStack     = mrb_define_class_under(mrb, mod_GPU,   "MatrixStack",     mrb->object_class);
  class_Shader          = mrb_define_class_under(mrb, mod_GPU,   "Shader",          mrb->object_class);
  class_Program         = mrb_define_class_under(mrb, mod_GPU,   "Program",         mrb->object_class);
  class_ShaderBlock     = mrb_define_class_under(mrb, mod_GPU,   "ShaderBlock",     mrb->object_class);
  class_Attribute       = mrb_define_class_under(mrb, mod_GPU,   "Attribute",       mrb->object_class);
  class_AttributeFormat = mrb_define_class_under(mrb, mod_GPU,   "AttributeFormat", mrb->object_class);

  MRB_SET_INSTANCE_TT(class_Rect,            MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_Surface,         MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_RendererID,      MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_Renderer,        MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_Context,         MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_Camera,          MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_BlendMode,       MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_Image,           MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_MatrixStack,     MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_Shader,          MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_Program,         MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_ShaderBlock,     MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_Attribute,       MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_AttributeFormat, MRB_TT_DATA);

  /**************************************************************************
   * Initialization 
   * info: http://dinomage.com/reference/SDL_gpu/group__Initialization.html
   **************************************************************************/
  mrb_define_module_function(mrb, mod_GPU, "get_linked_ver",           mrb_sdl2_gpu_get_linked_ver,           MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "set_init_window",          mrb_sdl2_gpu_set_init_window,          MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GPU, "get_init_window",          mrb_sdl2_gpu_get_init_window,          MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "set_pre_init_flags",       mrb_sdl2_gpu_set_pre_init_flags,       MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GPU, "get_pre_init_flags",       mrb_sdl2_gpu_get_pre_init_flags,       MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "set_required_features",    mrb_sdl2_gpu_set_required_features,    MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GPU, "get_required_features",    mrb_sdl2_gpu_get_required_features,    MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "get_default_render_order", mrb_sdl2_gpu_get_default_render_order, MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, mod_GPU, "get_renderer_order",       mrb_sdl2_gpu_get_renderer_order,       MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "set_renderer_order",       mrb_sdl2_gpu_set_renderer_order,       MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "init",                     mrb_sdl2_gpu_init,                     MRB_ARGS_REQ(3));
  mrb_define_module_function(mrb, mod_GPU, "init_renderer",            mrb_sdl2_gpu_init_renderer,            MRB_ARGS_REQ(4));
  mrb_define_module_function(mrb, mod_GPU, "init_renderer_by_id",      mrb_sdl2_gpu_init_renderer_by_id,      MRB_ARGS_REQ(4));
  mrb_define_module_function(mrb, mod_GPU, "feature_enabled?",         mrb_sdl2_gpu_feature_enabled,          MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GPU, "close_current_renderer",   mrb_sdl2_gpu_close_current_renderer,   MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "quit",                     mrb_sdl2_gpu_quit,                     MRB_ARGS_NONE());

  /*****************************************************************
   * Debugging, Logging, and Error Handling
   * info: http://dinomage.com/reference/SDL_gpu/group__Logging.html
   ******************************************************************/
  mrb_define_module_function(mrb, mod_GPU, "debug_level=", mrb_sdl2_gpu_set_debug_level, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GPU, "debug_level",  mrb_sdl2_gpu_get_debug_level, MRB_ARGS_NONE());
  // TBD add functions that use va_lists

  /***********************************************************************
   * Renderer Setup
   * info: http://dinomage.com/reference/SDL_gpu/group__RendererSetup.html
   ***********************************************************************/
  mrb_define_module_function(mrb, mod_GPU, "mark_renderer_id",         mrb_sdl2_gpu_mark_renderer_id,             MRB_ARGS_REQ(4));
  mrb_define_module_function(mrb, mod_GPU, "renderer_id",              mrb_sdl2_gpu_get_renderer_id,              MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GPU, "renderer_by_index",        mrb_sdl2_gpu_get_renderer_index,           MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GPU, "num_registered_renderers", mrb_sdl2_gpu_get_num_registered_renderers, MRB_ARGS_NONE());

  /***************************************************************************
   * Renderer Control
   * info: http://dinomage.com/reference/SDL_gpu/group__RendererControls.html
   ***************************************************************************/
  mrb_define_module_function(mrb, mod_GPU, "reserver_next_enum",   mrb_sdl2_gpu_reserver_next_enum,   MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GPU, "num_active_renderers", mrb_sdl2_gpu_num_active_renderers, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "get_renderer",         mrb_sdl2_gpu_get_renderer,         MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GPU, "get_current_renderer", mrb_sdl2_gpu_get_current_renderer, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "set_current_renderer", mrb_sdl2_gpu_set_current_renderer, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GPU, "reset_renderer_state", mrb_sdl2_gpu_reset_renderer_state, MRB_ARGS_NONE());

  /***************************************************************************
   * Context Control
   * info: http://dinomage.com/reference/SDL_gpu/group__ContextControls.html
   ***************************************************************************/
  mrb_define_module_function(mrb, mod_GPU, "context_target",                mrb_sdl2_gpu_get_context_target,            MRB_ARGS_NONE());  
  mrb_define_module_function(mrb, mod_GPU, "window_target",                 mrb_sdl2_gpu_get_window_target,             MRB_ARGS_REQ(1));  
  mrb_define_module_function(mrb, mod_GPU, "create_target_from_window",     mrb_sdl2_gpu_create_target_from_window,     MRB_ARGS_REQ(1));  
  mrb_define_module_function(mrb, mod_GPU, "set_window_resolution",         mrb_sdl2_gpu_set_window_resolution,         MRB_ARGS_REQ(2));  
  mrb_define_module_function(mrb, mod_GPU, "set_fullscreen",                mrb_sdl2_gpu_set_fullscreen,                MRB_ARGS_REQ(2));  
  mrb_define_module_function(mrb, mod_GPU, "get_fullscreen",                mrb_sdl2_gpu_get_fullscreen,                MRB_ARGS_NONE()); 
  mrb_define_module_function(mrb, mod_GPU, "set_shape_blending",            mrb_sdl2_gpu_set_shape_blending,            MRB_ARGS_REQ(1)); 
  mrb_define_module_function(mrb, mod_GPU, "get_blend_mode_from_preset",    mrb_sdl2_gpu_get_blend_mode_from_preset,    MRB_ARGS_REQ(1)); 
  mrb_define_module_function(mrb, mod_GPU, "set_shape_blend_mode_function", mrb_sdl2_gpu_set_shape_blend_mode_function, MRB_ARGS_REQ(4)); 
  mrb_define_module_function(mrb, mod_GPU, "set_shape_blend_equation",      mrb_sdl2_gpu_set_shape_blend_equation,      MRB_ARGS_REQ(2)); 
  mrb_define_module_function(mrb, mod_GPU, "set_shape_blend_mode",          mrb_sdl2_gpu_set_shape_blend_mode,          MRB_ARGS_REQ(1)); 
  mrb_define_module_function(mrb, mod_GPU, "set_line_thickness",            mrb_sdl2_gpu_set_line_thickness,            MRB_ARGS_REQ(1)); 
  mrb_define_module_function(mrb, mod_GPU, "get_line_thickness",            mrb_sdl2_gpu_get_line_thickness,            MRB_ARGS_REQ(1)); 

  /***************************************************************************
   * Target Control
   * info: http://dinomage.com/reference/SDL_gpu/group__TargetControls.html
   ***************************************************************************/
  mrb_define_module_function(mrb, mod_GPU, "create_alias_target",      mrb_sdl2_gpu_create_alias_target,      MRB_ARGS_REQ(1)); 
  mrb_define_module_function(mrb, mod_GPU, "load_target",              mrb_sdl2_gpu_load_target,              MRB_ARGS_REQ(1)); 
  mrb_define_module_function(mrb, mod_GPU, "set_virtual_resolution",   mrb_sdl2_gpu_set_virtual_resolution,   MRB_ARGS_REQ(3));
  mrb_define_module_function(mrb, mod_GPU, "get_virtual_coords",       mrb_sdl2_gpu_get_virtual_coords,       MRB_ARGS_REQ(3)); 
  mrb_define_module_function(mrb, mod_GPU, "unset_virtual_resolution", mrb_sdl2_gpu_unset_virtual_resolution, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GPU, "make_rect",                mrb_sdl2_gpu_make_rect,                MRB_ARGS_REQ(4));
  mrb_define_module_function(mrb, mod_GPU, "make_color",               mrb_sdl2_gpu_make_color,               MRB_ARGS_REQ(4));
  mrb_define_module_function(mrb, mod_GPU, "default_camera",           mrb_sdl2_gpu_default_camera,           MRB_ARGS_REQ(4));

  /***************************************************************************
   * Surface Control
   * info: http://dinomage.com/reference/SDL_gpu/group__SurfaceControls.html
   ***************************************************************************/
  mrb_define_module_function(mrb, mod_GPU, "load_surface", mrb_sdl2_gpu_load_surface, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GPU, "save_surface", mrb_sdl2_gpu_save_surface, MRB_ARGS_REQ(2));

  /***************************************************************************
   * Target Class Functions
   ***************************************************************************/
  mrb_define_method(mrb, class_Target, "make_current",  mrb_sdl2_gpu_target_make_current,  MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Target, "free",          mrb_sdl2_gpu_target_free,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Target, "destroy",       mrb_sdl2_gpu_target_free,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Target, "set_viewport",  mrb_sdl2_gpu_target_set_viewport,  MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Target, "camera",        mrb_sdl2_gpu_target_get_camera,    MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Target, "camera=",       mrb_sdl2_gpu_target_set_camera,    MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Target, "get_pixel",     mrb_sdl2_gpu_target_get_pixel,     MRB_ARGS_REQ(2));
  mrb_define_method(mrb, class_Target, "set_clip_rect", mrb_sdl2_gpu_target_set_clip_rect, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Target, "set_clip",      mrb_sdl2_gpu_target_set_clip,      MRB_ARGS_REQ(4));
  mrb_define_method(mrb, class_Target, "unset_clip",    mrb_sdl2_gpu_target_unset_clip,    MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Target, "set_rgb",       mrb_sdl2_gpu_target_set_rgb,       MRB_ARGS_REQ(3));
  mrb_define_method(mrb, class_Target, "set_rgba",      mrb_sdl2_gpu_target_set_rgba,      MRB_ARGS_REQ(4));
  mrb_define_method(mrb, class_Target, "get_rgba",      mrb_sdl2_gpu_target_get_rgba,      MRB_ARGS_REQ(4));
  mrb_define_method(mrb, class_Target, "unset_color",   mrb_sdl2_gpu_target_unset_color,   MRB_ARGS_NONE());

  /***************************************************************************
   * Image Class Functions
   ***************************************************************************/
  mrb_define_method(mrb, class_Image, "initialize",         mrb_sdl2_gpu_image_initialize,         MRB_ARGS_REQ(1) | MRB_ARGS_OPT(2));
  mrb_define_method(mrb, class_Image, "free",               mrb_sdl2_gpu_image_free,               MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "destroy",            mrb_sdl2_gpu_image_free,               MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "create_alias",       mrb_sdl2_gpu_image_create_alias,       MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "copy",               mrb_sdl2_gpu_image_copy,               MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "update",             mrb_sdl2_gpu_image_update,             MRB_ARGS_REQ(1) | MRB_ARGS_OPT(2));
  mrb_define_method(mrb, class_Image, "save",               mrb_sdl2_gpu_image_save,               MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "generate_mipmaps",   mrb_sdl2_gpu_image_generate_mipmaps,   MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "set_rgb",            mrb_sdl2_gpu_image_set_rgb,            MRB_ARGS_REQ(3));
  mrb_define_method(mrb, class_Image, "set_rgba",           mrb_sdl2_gpu_image_set_rgba,           MRB_ARGS_REQ(4));
  mrb_define_method(mrb, class_Image, "get_rgba",           mrb_sdl2_gpu_image_get_rgba,           MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "unset_color",        mrb_sdl2_gpu_image_unset_color,        MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "blending",           mrb_sdl2_gpu_image_get_blending,       MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "blending=",          mrb_sdl2_gpu_image_set_blending,       MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "set_blend_function", mrb_sdl2_gpu_image_set_blend_function, MRB_ARGS_REQ(4));
  mrb_define_method(mrb, class_Image, "set_blend_equation", mrb_sdl2_gpu_image_set_blend_equation, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, class_Image, "blend_mode",         mrb_sdl2_gpu_image_set_blend_mode,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "filter",             mrb_sdl2_gpu_image_set_filter,         MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "snap_mode",          mrb_sdl2_gpu_image_get_snap_mode,      MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "snap_mode=",         mrb_sdl2_gpu_image_set_snap_mode,      MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "set_wrap_mode",      mrb_sdl2_gpu_image_set_wrap_mode,      MRB_ARGS_REQ(2));

  /***************************************************************************
   * Surface Control
   * info: http://dinomage.com/reference/SDL_gpu/group__SurfaceControls.html
   ***************************************************************************/
  mrb_define_method(mrb, class_Surface, "to_image",   mrb_sdl2_gpu_surface_to_image,  MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Target,  "to_image",   mrb_sdl2_gpu_target_to_image,   MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Target,  "to_surface", mrb_sdl2_gpu_target_to_surface, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image,   "to_surface", mrb_sdl2_gpu_image_to_surface,  MRB_ARGS_NONE());

  /***************************************************************************
   * Matrix Control
   * info: http://dinomage.com/reference/SDL_gpu/group__Matrix.html
   ***************************************************************************/
  // TBD THE WHOLE MATRIX CONTROL!!

  /***************************************************************************
   * Rendering
   * info: http://dinomage.com/reference/SDL_gpu/group__Rendering.html
   ***************************************************************************/
  mrb_define_module_function(mrb, mod_GPU, "flush_blit_buffer", mrb_sdl2_gpu_flush_blit_buffer, MRB_ARGS_NONE()); 

  mrb_define_method(mrb, class_Target, "clear",              mrb_sdl2_gpu_target_clear,              MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Target, "clear_rgb",          mrb_sdl2_gpu_target_clear_rgb,          MRB_ARGS_REQ(3));
  mrb_define_method(mrb, class_Target, "clear_rgba",         mrb_sdl2_gpu_target_clear_rgba,         MRB_ARGS_REQ(4));
  mrb_define_method(mrb, class_Target, "flip",               mrb_sdl2_gpu_target_flip,               MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Target, "blit",               mrb_sdl2_gpu_target_blit,               MRB_ARGS_REQ(4) | MRB_ARGS_OPT(3));
  mrb_define_method(mrb, class_Target, "blit_batch",         mrb_sdl2_gpu_target_blit_batch,         MRB_ARGS_REQ(4));
  mrb_define_method(mrb, class_Target, "pixel",              mrb_sdl2_gpu_target_pixel,              MRB_ARGS_REQ(6));
  mrb_define_method(mrb, class_Target, "line",               mrb_sdl2_gpu_target_line,               MRB_ARGS_REQ(8));
  mrb_define_method(mrb, class_Target, "arc",                mrb_sdl2_gpu_target_arc,                MRB_ARGS_REQ(9));
  mrb_define_method(mrb, class_Target, "arc_filled",         mrb_sdl2_gpu_target_arc_filled,         MRB_ARGS_REQ(9));
  mrb_define_method(mrb, class_Target, "circle",             mrb_sdl2_gpu_target_circle,             MRB_ARGS_REQ(7));
  mrb_define_method(mrb, class_Target, "circle_filled",      mrb_sdl2_gpu_target_circle_filled,      MRB_ARGS_REQ(7));
  mrb_define_method(mrb, class_Target, "ellipse",            mrb_sdl2_gpu_target_ellipse,            MRB_ARGS_REQ(7));
  mrb_define_method(mrb, class_Target, "ellipse_filled",     mrb_sdl2_gpu_target_ellipse_filled,     MRB_ARGS_REQ(7));
  mrb_define_method(mrb, class_Target, "sector",             mrb_sdl2_gpu_target_sector,             MRB_ARGS_REQ(10));
  mrb_define_method(mrb, class_Target, "sector_filled",      mrb_sdl2_gpu_target_sector_filled,      MRB_ARGS_REQ(10));
  mrb_define_method(mrb, class_Target, "tri",                mrb_sdl2_gpu_target_tri,                MRB_ARGS_REQ(10));
  mrb_define_method(mrb, class_Target, "tri_filled",         mrb_sdl2_gpu_target_tri_filled,         MRB_ARGS_REQ(10));
  mrb_define_method(mrb, class_Target, "rect2",              mrb_sdl2_gpu_target_rect2,              MRB_ARGS_REQ(5));
  mrb_define_method(mrb, class_Target, "rect_filled2",       mrb_sdl2_gpu_target_rect_filled2,       MRB_ARGS_REQ(5));
  mrb_define_method(mrb, class_Target, "rect",               mrb_sdl2_gpu_target_rect,               MRB_ARGS_REQ(8));
  mrb_define_method(mrb, class_Target, "rect_filled",        mrb_sdl2_gpu_target_rect_filled,        MRB_ARGS_REQ(8));
  mrb_define_method(mrb, class_Target, "rect_round",         mrb_sdl2_gpu_target_rect_round,         MRB_ARGS_REQ(6) | MRB_ARGS_OPT(3));
  mrb_define_method(mrb, class_Target, "rect_round_filled",  mrb_sdl2_gpu_target_rect_round_filled,  MRB_ARGS_REQ(6) | MRB_ARGS_OPT(3));
  mrb_define_method(mrb, class_Target, "gradient_fill_rect", mrb_sdl2_gpu_target_gradient_fill_rect, MRB_ARGS_REQ(10));
  // TBD - GPU_Polygon - array of floats involved

  /**************************************************************************
   * ShaderInterface
   *
   *************************************************************************/
  mrb_define_module_function(mrb, mod_GPU, "get_current_shader_program", mrb_sdl2_gpu_get_current_shader_program, MRB_ARGS_NONE());

  mrb_define_module_function(mrb, class_Program, "deactivate", mrb_sdl2_gpu_program_deactivate, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Program, "initialize",             mrb_sdl2_gpu_program_initialize,        MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Program, "free",                   mrb_sdl2_gpu_program_free,              MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Program, "attach_shader",          mrb_sdl2_gpu_program_attach,            MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Program, "detach_shader",          mrb_sdl2_gpu_program_detach,            MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Program, "link",                   mrb_sdl2_gpu_program_link,              MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Program, "is_default?",            mrb_sdl2_gpu_program_is_default,        MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Program, "activate",               mrb_sdl2_gpu_program_activate,          MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Program, "get_attribute_location", mrb_sdl2_gpu_program_get_arg_location,  MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Program, "get_uniform_location",   mrb_sdl2_gpu_program_get_uni_location,  MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Program, "load_shader_block",      mrb_sdl2_gpu_program_load_shader_block, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Program, "get_uniformuiv",         mrb_sdl2_gpu_program_get_uniformuiv,    MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Program, "get_uniformfv",          mrb_sdl2_gpu_program_get_uniformfv,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Program, "get_uniform_matrix_fv",  mrb_sdl2_gpu_program_get_umfv,          MRB_ARGS_NONE());

  mrb_define_module_function(mrb, mod_GPU, "set_uniformi",          mrb_sdl2_gpu_set_uniformi,     MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "set_uniformui",         mrb_sdl2_gpu_set_uniformui,    MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "set_uniformuiv",        mrb_sdl2_gpu_set_uniformuiv,   MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "set_uniformf",          mrb_sdl2_gpu_set_uniformf,     MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "set_uniformfv",         mrb_sdl2_gpu_set_uniformfv,    MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "set_uniform_matrix_fv", mrb_sdl2_gpu_set_umfv,         MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "set_attributef",        mrb_sdl2_gpu_set_attributef,   MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "set_attributei",        mrb_sdl2_gpu_set_attributei,   MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "set_attributeui",       mrb_sdl2_gpu_set_attributeui,  MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "set_attributefv",       mrb_sdl2_gpu_set_attributefv,  MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "set_attributefv",       mrb_sdl2_gpu_set_attributeiv,  MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GPU, "set_attributefv",       mrb_sdl2_gpu_set_attributeuiv, MRB_ARGS_NONE());

  mrb_define_method(mrb, class_Shader, "initialize", mrb_sdl2_gpu_shader_initialize, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Shader, "free",       mrb_sdl2_gpu_shader_free,       MRB_ARGS_NONE());
  mrb_define_module_function(mrb, class_Shader, "get_message", mrb_sdl2_gpu_shader_get_msg, MRB_ARGS_NONE());

  mrb_define_method(mrb, class_ShaderBlock, "free",       mrb_sdl2_gpu_shaderblock_free,       MRB_ARGS_NONE());
  mrb_define_method(mrb, class_ShaderBlock, "set",       mrb_sdl2_gpu_shaderblock_set,       MRB_ARGS_NONE());

  mrb_define_method(mrb, class_AttributeFormat, "initialize", mrb_sdl2_gpu_attributeformat_initialize, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_AttributeFormat, "free",       mrb_sdl2_gpu_attributeformat_free,       MRB_ARGS_NONE());

  mrb_define_method(mrb, class_Attribute, "initialize", mrb_sdl2_gpu_attribute_initialize, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Attribute, "free",       mrb_sdl2_gpu_attribute_free,       MRB_ARGS_NONE());
  
  mrb_define_method(mrb, class_Image, "set", mrb_sdl2_gpu_image_set,       MRB_ARGS_NONE());


  /***************************************************************************
   * Additionals access to
   * * GPU_Rect's properties
   * * GPU_RendererID's properties
   * * GPU_BlendMode's properties
   * * GPU_Image's properties
   ***************************************************************************/
  /*********************** GPU_Rect ********************************************************/
  mrb_define_method(mrb, class_Rect, "initialize", mrb_sdl2_gpu_rect_init,  MRB_ARGS_OPT(4));
  mrb_define_method(mrb, class_Rect, "x",          mrb_sdl2_gpu_rect_get_x, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Rect, "x=",         mrb_sdl2_gpu_rect_set_x, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Rect, "y",          mrb_sdl2_gpu_rect_get_y, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Rect, "y=",         mrb_sdl2_gpu_rect_set_y, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Rect, "w",          mrb_sdl2_gpu_rect_get_w, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Rect, "w=",         mrb_sdl2_gpu_rect_set_w, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Rect, "h",          mrb_sdl2_gpu_rect_get_h, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Rect, "h=",         mrb_sdl2_gpu_rect_set_h, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Rect, "width",      mrb_sdl2_gpu_rect_get_w, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Rect, "width=",     mrb_sdl2_gpu_rect_set_w, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Rect, "height",     mrb_sdl2_gpu_rect_get_h, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Rect, "height=",    mrb_sdl2_gpu_rect_set_h, MRB_ARGS_REQ(1));

  /*********************** GPU_RendererID ********************************************************************/
  mrb_define_method(mrb, class_RendererID, "name",      mrb_sdl2_gpu_rendererid_get_name,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_RendererID, "name=",     mrb_sdl2_gpu_rendererid_set_name,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_RendererID, "renderer",  mrb_sdl2_gpu_rendererid_get_renderer, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_RendererID, "renderer=", mrb_sdl2_gpu_rendererid_set_renderer, MRB_ARGS_NONE());

  /*********************** GPU_BlendMode ******************************************************************************/
  mrb_define_method(mrb, class_BlendMode, "source_color",    mrb_sdl2_gpu_blendmode_source_color,       MRB_ARGS_NONE());
  mrb_define_method(mrb, class_BlendMode, "source_color=",   mrb_sdl2_gpu_blendmode_set_source_color,   MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_BlendMode, "source_alpha",    mrb_sdl2_gpu_blendmode_source_alpha,       MRB_ARGS_NONE());
  mrb_define_method(mrb, class_BlendMode, "source_alpha=",   mrb_sdl2_gpu_blendmode_set_source_alpha,   MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_BlendMode, "dest_color",      mrb_sdl2_gpu_blendmode_dest_color,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_BlendMode, "dest_color=",     mrb_sdl2_gpu_blendmode_set_dest_color,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_BlendMode, "dest_alpha",      mrb_sdl2_gpu_blendmode_dest_alpha,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_BlendMode, "dest_alpha=",     mrb_sdl2_gpu_blendmode_set_dest_alpha,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_BlendMode, "color_equation",  mrb_sdl2_gpu_blendmode_color_equation,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_BlendMode, "color_equation=", mrb_sdl2_gpu_blendmode_set_color_equation, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_BlendMode, "alpha_equation",  mrb_sdl2_gpu_blendmode_alpha_equation,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_BlendMode, "alpha_equation=", mrb_sdl2_gpu_blendmode_set_alpha_equation, MRB_ARGS_REQ(1));

  /*********************** GPU_Image *****************************************************************************/
  mrb_define_method(mrb, class_Image, "renderer",         mrb_sdl2_gpu_image_renderer,            MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "target",           mrb_sdl2_gpu_image_target,              MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "w",                mrb_sdl2_gpu_image_w,                   MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "h",                mrb_sdl2_gpu_image_h,                   MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "using_vr?",        mrb_sdl2_gpu_image_using_vr,            MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "using_vr",         mrb_sdl2_gpu_image_using_vr,            MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "format",           mrb_sdl2_gpu_image_format,              MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "num_layers",       mrb_sdl2_gpu_image_num_layers,          MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "num_layers=",      mrb_sdl2_gpu_image_set_num_layers,      MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "bytes_per_pixel",  mrb_sdl2_gpu_image_bytes_per_pixel,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "base_w",           mrb_sdl2_gpu_image_base_w,              MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "base_h",           mrb_sdl2_gpu_image_base_h,              MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "texture_w",        mrb_sdl2_gpu_image_texture_w,           MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "texture_h",        mrb_sdl2_gpu_image_texture_h,           MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "has_mipmaps?",     mrb_sdl2_gpu_image_has_mipmaps,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "mipmaps",          mrb_sdl2_gpu_image_has_mipmaps,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "mipmaps=",         mrb_sdl2_gpu_image_set_has_mipmaps,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "refcount",         mrb_sdl2_gpu_image_refcount,            MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "refcount=",        mrb_sdl2_gpu_image_set_refcount,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "wrap_mode_x",      mrb_sdl2_gpu_image_wrap_mode_x,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "wrap_mode_x=",     mrb_sdl2_gpu_image_set_wrap_mode_x,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "wrap_mode_y",      mrb_sdl2_gpu_image_wrap_mode_y,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "wrap_mode_y=",     mrb_sdl2_gpu_image_set_wrap_mode_y,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "snap_mode",        mrb_sdl2_gpu_image_snap_mode,           MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "snap_mode=",       mrb_sdl2_gpu_image_set_snap_mode,       MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "filter_mode",      mrb_sdl2_gpu_image_filter_mode,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "filter_mode=",     mrb_sdl2_gpu_image_set_filter_mode,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "blend_mode",       mrb_sdl2_gpu_image_blend_mode,          MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "blend_mode=",      mrb_sdl2_gpu_image_set_blend_mode,      MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "is_alias?",        mrb_sdl2_gpu_image_is_alias,            MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "alias",            mrb_sdl2_gpu_image_is_alias,            MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "alias=",           mrb_sdl2_gpu_image_set_is_alias,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "renderer=",        mrb_sdl2_gpu_image_set_renderer,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "target=",          mrb_sdl2_gpu_image_set_target,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "using_vr=",        mrb_sdl2_gpu_image_set_using_vr,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "w=",               mrb_sdl2_gpu_image_set_w,               MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "h=",               mrb_sdl2_gpu_image_set_h,               MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "base_w=",          mrb_sdl2_gpu_image_set_base_w,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "base_h=",          mrb_sdl2_gpu_image_set_base_h,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "format=",          mrb_sdl2_gpu_image_set_format,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "num_layers=",      mrb_sdl2_gpu_image_set_format,          MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "bytes_per_pixel=", mrb_sdl2_gpu_image_set_bytes_per_pixel, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "texture_w=",       mrb_sdl2_gpu_image_set_texture_w,       MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "texture_h=",       mrb_sdl2_gpu_image_set_texture_h,       MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "mipmaps=",         mrb_sdl2_gpu_image_set_using_vr,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Image, "use_blending?",    mrb_sdl2_gpu_image_use_blending,        MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "blending",         mrb_sdl2_gpu_image_use_blending,        MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Image, "blending=",        mrb_sdl2_gpu_image_set_use_blending,    MRB_ARGS_REQ(1));


  /***************************************************************************
   * Enums & Defines
   ***************************************************************************/
  arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_GPU, "GPU_RENDERER_UNKNOWN",       mrb_fixnum_value(GPU_RENDERER_UNKNOWN));
  mrb_define_const(mrb, mod_GPU, "GPU_RENDERER_OPENGL_1_BASE", mrb_fixnum_value(GPU_RENDERER_OPENGL_1_BASE));
  mrb_define_const(mrb, mod_GPU, "GPU_RENDERER_OPENGL_1",      mrb_fixnum_value(GPU_RENDERER_OPENGL_1));
  mrb_define_const(mrb, mod_GPU, "GPU_RENDERER_OPENGL_2",      mrb_fixnum_value(GPU_RENDERER_OPENGL_2));
  mrb_define_const(mrb, mod_GPU, "GPU_RENDERER_OPENGL_3",      mrb_fixnum_value(GPU_RENDERER_OPENGL_3));
  mrb_define_const(mrb, mod_GPU, "GPU_RENDERER_OPENGL_4",      mrb_fixnum_value(GPU_RENDERER_OPENGL_4));
  mrb_define_const(mrb, mod_GPU, "GPU_RENDERER_GLES_1",        mrb_fixnum_value(GPU_RENDERER_GLES_1));
  mrb_define_const(mrb, mod_GPU, "GPU_RENDERER_GLES_2",        mrb_fixnum_value(GPU_RENDERER_GLES_2));
  mrb_define_const(mrb, mod_GPU, "GPU_RENDERER_GLES_3",        mrb_fixnum_value(GPU_RENDERER_GLES_3));
  mrb_define_const(mrb, mod_GPU, "GPU_RENDERER_D3D9",          mrb_fixnum_value(GPU_RENDERER_D3D9));
  mrb_define_const(mrb, mod_GPU, "GPU_RENDERER_D3D10",         mrb_fixnum_value(GPU_RENDERER_D3D10));
  mrb_define_const(mrb, mod_GPU, "GPU_RENDERER_D3D11",         mrb_fixnum_value(GPU_RENDERER_D3D11));
  mrb_gc_arena_restore(mrb, arena_size);

  arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_GPU, "GPU_FUNC_ZERO",                mrb_fixnum_value(GPU_FUNC_ZERO));
  mrb_define_const(mrb, mod_GPU, "GPU_FUNC_ONE",                 mrb_fixnum_value(GPU_FUNC_ONE));
  mrb_define_const(mrb, mod_GPU, "GPU_FUNC_SRC_COLOR",           mrb_fixnum_value(GPU_FUNC_SRC_COLOR));
  mrb_define_const(mrb, mod_GPU, "GPU_FUNC_DST_COLOR",           mrb_fixnum_value(GPU_FUNC_DST_COLOR));
  mrb_define_const(mrb, mod_GPU, "GPU_FUNC_ONE_MINUS_SRC",       mrb_fixnum_value(GPU_FUNC_ONE_MINUS_SRC));
  mrb_define_const(mrb, mod_GPU, "GPU_FUNC_ONE_MINUS_DST",       mrb_fixnum_value(GPU_FUNC_ONE_MINUS_DST));
  mrb_define_const(mrb, mod_GPU, "GPU_FUNC_SRC_ALPHA",           mrb_fixnum_value(GPU_FUNC_SRC_ALPHA));
  mrb_define_const(mrb, mod_GPU, "GPU_FUNC_DST_ALPHA",           mrb_fixnum_value(GPU_FUNC_DST_ALPHA));
  mrb_define_const(mrb, mod_GPU, "GPU_FUNC_ONE_MINUS_SRC_ALPHA", mrb_fixnum_value(GPU_FUNC_ONE_MINUS_SRC_ALPHA));
  mrb_define_const(mrb, mod_GPU, "GPU_FUNC_ONE_MINUS_DST_ALPHA", mrb_fixnum_value(GPU_FUNC_ONE_MINUS_DST_ALPHA));
  mrb_gc_arena_restore(mrb, arena_size);

  arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_GPU, "GPU_EQ_ADD",              mrb_fixnum_value(GPU_EQ_ADD));
  mrb_define_const(mrb, mod_GPU, "GPU_EQ_SUBTRACT",         mrb_fixnum_value(GPU_EQ_SUBTRACT));
  mrb_define_const(mrb, mod_GPU, "GPU_EQ_REVERSE_SUBTRACT", mrb_fixnum_value(GPU_EQ_REVERSE_SUBTRACT));
  mrb_gc_arena_restore(mrb, arena_size);

  arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_GPU, "GPU_BLEND_NORMAL",              mrb_fixnum_value(GPU_BLEND_NORMAL));
  mrb_define_const(mrb, mod_GPU, "GPU_BLEND_PREMULTIPLIED_ALPHA", mrb_fixnum_value(GPU_BLEND_PREMULTIPLIED_ALPHA));
  mrb_define_const(mrb, mod_GPU, "GPU_BLEND_MULTIPLY",            mrb_fixnum_value(GPU_BLEND_MULTIPLY));
  mrb_define_const(mrb, mod_GPU, "GPU_BLEND_ADD",                 mrb_fixnum_value(GPU_BLEND_ADD));
  mrb_define_const(mrb, mod_GPU, "GPU_BLEND_SUBTRACT",            mrb_fixnum_value(GPU_BLEND_SUBTRACT));
  mrb_define_const(mrb, mod_GPU, "GPU_BLEND_MOD_ALPHA",           mrb_fixnum_value(GPU_BLEND_MOD_ALPHA));
  mrb_define_const(mrb, mod_GPU, "GPU_BLEND_SET_ALPHA",           mrb_fixnum_value(GPU_BLEND_SET_ALPHA));
  mrb_define_const(mrb, mod_GPU, "GPU_BLEND_SET",                 mrb_fixnum_value(GPU_BLEND_SET));
  mrb_define_const(mrb, mod_GPU, "GPU_BLEND_NORMAL_KEEP_ALPHA",   mrb_fixnum_value(GPU_BLEND_NORMAL_KEEP_ALPHA));
  mrb_define_const(mrb, mod_GPU, "GPU_BLEND_NORMAL_ADD_ALPHA",    mrb_fixnum_value(GPU_BLEND_NORMAL_ADD_ALPHA));
  mrb_gc_arena_restore(mrb, arena_size);

  arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_GPU, "GPU_FILTER_NEAREST",       mrb_fixnum_value(GPU_FILTER_NEAREST));
  mrb_define_const(mrb, mod_GPU, "GPU_FILTER_LINEAR",        mrb_fixnum_value(GPU_FILTER_LINEAR));
  mrb_define_const(mrb, mod_GPU, "GPU_FILTER_LINEAR_MIPMAP", mrb_fixnum_value(GPU_FILTER_LINEAR_MIPMAP));
  mrb_gc_arena_restore(mrb, arena_size);

  arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_GPU, "GPU_BATCH_XY",          mrb_fixnum_value(GPU_BATCH_XY));
  mrb_define_const(mrb, mod_GPU, "GPU_BATCH_XYZ",         mrb_fixnum_value(GPU_BATCH_XYZ));
  mrb_define_const(mrb, mod_GPU, "GPU_BATCH_ST",          mrb_fixnum_value(GPU_BATCH_ST));
  mrb_define_const(mrb, mod_GPU, "GPU_BATCH_RGB",         mrb_fixnum_value(GPU_BATCH_RGB));
  mrb_define_const(mrb, mod_GPU, "GPU_BATCH_RGBA",        mrb_fixnum_value(GPU_BATCH_RGBA));
  mrb_define_const(mrb, mod_GPU, "GPU_BATCH_XY_ST",       mrb_fixnum_value(GPU_BATCH_XY_ST));
  mrb_define_const(mrb, mod_GPU, "GPU_BATCH_XYZ_ST",      mrb_fixnum_value(GPU_BATCH_XYZ_ST));
  mrb_define_const(mrb, mod_GPU, "GPU_BATCH_XY_RGB",      mrb_fixnum_value(GPU_BATCH_XY_RGB));
  mrb_define_const(mrb, mod_GPU, "GPU_BATCH_XYZ_RGB",     mrb_fixnum_value(GPU_BATCH_XYZ_RGB));
  mrb_define_const(mrb, mod_GPU, "GPU_BATCH_XY_RGBA",     mrb_fixnum_value(GPU_BATCH_XY_RGBA));
  mrb_define_const(mrb, mod_GPU, "GPU_BATCH_XYZ_RGBA",    mrb_fixnum_value(GPU_BATCH_XYZ_RGBA));
  mrb_define_const(mrb, mod_GPU, "GPU_BATCH_XY_ST_RGBA",  mrb_fixnum_value(GPU_BATCH_XY_ST_RGBA));
  mrb_define_const(mrb, mod_GPU, "GPU_BATCH_XYZ_ST_RGBA", mrb_fixnum_value(GPU_BATCH_XYZ_ST_RGBA));
  mrb_gc_arena_restore(mrb, arena_size);

  arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_GPU, "GPU_SNAP_NONE",                    mrb_fixnum_value(GPU_SNAP_NONE));
  mrb_define_const(mrb, mod_GPU, "GPU_SNAP_POSITION",                mrb_fixnum_value(GPU_SNAP_POSITION));
  mrb_define_const(mrb, mod_GPU, "GPU_SNAP_DIMENSIONS",              mrb_fixnum_value(GPU_SNAP_DIMENSIONS));
  mrb_define_const(mrb, mod_GPU, "GPU_SNAP_POSITION_AND_DIMENSIONS", mrb_fixnum_value(GPU_SNAP_POSITION_AND_DIMENSIONS));
  mrb_gc_arena_restore(mrb, arena_size);

  arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_GPU, "GPU_VERTEX_SHADER",   mrb_fixnum_value(GPU_VERTEX_SHADER));
  mrb_define_const(mrb, mod_GPU, "GPU_FRAGMENT_SHADER", mrb_fixnum_value(GPU_FRAGMENT_SHADER));
  mrb_define_const(mrb, mod_GPU, "GPU_PIXEL_SHADER",    mrb_fixnum_value(GPU_PIXEL_SHADER));
  mrb_define_const(mrb, mod_GPU, "GPU_GEOMETRY_SHADER", mrb_fixnum_value(GPU_GEOMETRY_SHADER));
  mrb_gc_arena_restore(mrb, arena_size);

  arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_GPU, "GPU_WRAP_NONE",     mrb_fixnum_value(GPU_WRAP_NONE));
  mrb_define_const(mrb, mod_GPU, "GPU_WRAP_REPEAT",   mrb_fixnum_value(GPU_WRAP_REPEAT));
  mrb_define_const(mrb, mod_GPU, "GPU_WRAP_MIRRORED", mrb_fixnum_value(GPU_WRAP_MIRRORED));
  mrb_gc_arena_restore(mrb, arena_size);

  arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_GPU, "GPU_FORMAT_LUMINANCE",       mrb_fixnum_value(GPU_FORMAT_LUMINANCE));
  mrb_define_const(mrb, mod_GPU, "GPU_FORMAT_LUMINANCE_ALPHA", mrb_fixnum_value(GPU_FORMAT_LUMINANCE_ALPHA));
  mrb_define_const(mrb, mod_GPU, "GPU_FORMAT_RGB",             mrb_fixnum_value(GPU_FORMAT_RGB));
  mrb_define_const(mrb, mod_GPU, "GPU_FORMAT_RGBA",            mrb_fixnum_value(GPU_FORMAT_RGBA));
  mrb_define_const(mrb, mod_GPU, "GPU_FORMAT_ALPHA",           mrb_fixnum_value(GPU_FORMAT_ALPHA));
  mrb_define_const(mrb, mod_GPU, "GPU_FORMAT_RG",              mrb_fixnum_value(GPU_FORMAT_RG));
  mrb_define_const(mrb, mod_GPU, "GPU_FORMAT_YCbCr422",        mrb_fixnum_value(GPU_FORMAT_YCbCr422));
  mrb_define_const(mrb, mod_GPU, "GPU_FORMAT_YCbCr420P",       mrb_fixnum_value(GPU_FORMAT_YCbCr420P));
  mrb_gc_arena_restore(mrb, arena_size);

  arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_GPU, "GPU_FILE_AUTO", mrb_fixnum_value(GPU_FILE_AUTO));
  mrb_define_const(mrb, mod_GPU, "GPU_FILE_PNG",  mrb_fixnum_value(GPU_FILE_PNG));
  mrb_define_const(mrb, mod_GPU, "GPU_FILE_BMP",  mrb_fixnum_value(GPU_FILE_BMP));
  mrb_define_const(mrb, mod_GPU, "GPU_FILE_TGA",  mrb_fixnum_value(GPU_FILE_TGA));
  mrb_gc_arena_restore(mrb, arena_size);

  arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_GPU, "GPU_FILE_AUTO",                            mrb_fixnum_value(GPU_FILE_AUTO));
  mrb_define_const(mrb, mod_GPU, "GPU_INIT_DISABLE_VSYNC",                   mrb_fixnum_value(GPU_INIT_DISABLE_VSYNC));
  mrb_define_const(mrb, mod_GPU, "GPU_INIT_DISABLE_DOUBLE_BUFFER",           mrb_fixnum_value(GPU_INIT_DISABLE_DOUBLE_BUFFER));
  mrb_define_const(mrb, mod_GPU, "GPU_INIT_DISABLE_AUTO_VIRTUAL_RESOLUTION", mrb_fixnum_value(GPU_INIT_DISABLE_AUTO_VIRTUAL_RESOLUTION));
  mrb_define_const(mrb, mod_GPU, "GPU_INIT_REQUEST_COMPATIBILITY_PROFILE",   mrb_fixnum_value(GPU_INIT_REQUEST_COMPATIBILITY_PROFILE));
  mrb_define_const(mrb, mod_GPU, "GPU_DEFAULT_INIT_FLAGS",                   mrb_fixnum_value(GPU_DEFAULT_INIT_FLAGS));
  mrb_gc_arena_restore(mrb, arena_size);

  arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_GPU, "GPU_DEBUG_LEVEL_0",   mrb_fixnum_value(GPU_DEBUG_LEVEL_0));
  mrb_define_const(mrb, mod_GPU, "GPU_DEBUG_LEVEL_1",   mrb_fixnum_value(GPU_DEBUG_LEVEL_1));
  mrb_define_const(mrb, mod_GPU, "GPU_DEBUG_LEVEL_2",   mrb_fixnum_value(GPU_DEBUG_LEVEL_2));
  mrb_define_const(mrb, mod_GPU, "GPU_DEBUG_LEVEL_3",   mrb_fixnum_value(GPU_DEBUG_LEVEL_3));
  mrb_define_const(mrb, mod_GPU, "GPU_DEBUG_LEVEL_MAX", mrb_fixnum_value(GPU_DEBUG_LEVEL_MAX));
  mrb_gc_arena_restore(mrb, arena_size);
}

void mrb_mruby_sdl2_gpu_gem_final(mrb_state *mrb) {
}
