#ifndef  DEF_GRAPHICS_STRUCT
#define  DEF_GRAPHICS_STRUCT

#include  <volume_io/basic.h>
#include  <graphics.h>

typedef  struct
{
    int             n_objects;
    object_struct   **objects;
    BOOLEAN         update_flag[2];    /* DOUBLE BUFFER */
} bitplane_graphics_struct;

typedef struct
{
    Colour                      background_colour;
    int                         background_colour_index;
    int                         x_min;
    int                         x_max;
    int                         y_min;
    int                         y_max;
    bitplane_graphics_struct    bitplanes[N_BITPLANE_TYPES];
} viewport_struct;

typedef  struct
{
    BOOLEAN           clear_bitplane_flags[N_BITPLANE_TYPES][2]; /* doub buff */
    int               n_viewports;
    viewport_struct   *viewports;
} graphics_struct;

#endif
