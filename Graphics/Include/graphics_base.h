#ifndef  DEF_GRAPHICS_BASE
#define  DEF_GRAPHICS_BASE

#include  <bicpl.h>

#define  RETURN_KEY    ((char) 13)
#define  DELETE_KEY    ((char) 127)
#define  BACKSPACE_KEY ((char) 8)

#define  LEFT_ARROW_KEY   245
#define  RIGHT_ARROW_KEY  246
#define  DOWN_ARROW_KEY   247
#define  UP_ARROW_KEY     248
#define  LEFT_SHIFT_KEY   249
#define  RIGHT_SHIFT_KEY  250
#define  LEFT_CTRL_KEY    251
#define  RIGHT_CTRL_KEY   252
#define  LEFT_ALT_KEY     253
#define  RIGHT_ALT_KEY    254

#define  SHIFT_KEY_BIT    (1 << 0)
#define  CTRL_KEY_BIT     (1 << 1)
#define  ALT_KEY_BIT      (1 << 2)

typedef  enum  {
                 NORMAL_PLANES,
                 OVERLAY_PLANES,
                 N_BITPLANE_TYPES } Bitplane_types;

typedef  enum  { PIXEL_VIEW, SCREEN_VIEW, WORLD_VIEW, MODEL_VIEW,
                 N_VIEW_TYPES }     View_types;

typedef  enum  { FLAT_SHADING, GOURAUD_SHADING }  Shading_types;

#define  N_LIGHTS  8

typedef  enum { 
                DIRECTIONAL_LIGHT,
                POINT_LIGHT,
                SPOT_LIGHT
              } Light_types;

typedef  enum { VIEWING_MATRIX, PROJECTION_MATRIX } Matrix_modes;

typedef  enum { LESS_OR_EQUAL } Depth_functions;



#define  NO_BELL       0
#define  SHORT_BELL    1
#define  LONG_BELL     2

#endif
