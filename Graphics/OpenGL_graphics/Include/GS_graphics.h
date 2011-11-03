#ifndef  DEF_GRAPHICS_SYSTEM_SPECIFIC
#define  DEF_GRAPHICS_SYSTEM_SPECIFIC

#include <bicpl.h>
#include <WS_graphics.h>

#ifdef HAVE_APPLE_OPENGL_FRAMEWORK
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

typedef  struct
{
    int             light_index;
    Light_types     type;
    Colour          colour;
    Vector          direction;
    Point           position;
    Real            spot_exponent;
    Real            spot_angle;
} light_info_struct;

typedef  struct
{
    WS_window_struct   WS_window;

    BOOLEAN            ambient_set;
    Real               ambient[3];

    int                n_lights_defined;
    light_info_struct  *lights;

    int                n_light_states;
    BOOLEAN            *light_indices;
    BOOLEAN            *light_states;
}
GS_window_struct;

typedef  GS_window_struct   *GSwindow;

#ifndef  public
#define       public
#define       public_was_defined_here
#endif

#include  <opengl_graphics_prototypes.h>

#ifdef  public_was_defined_here
#undef       public
#undef       public_was_defined_here
#endif

#endif
