 
#include  <volume_io/internal_volume_io.h>
#include  <graphics.h>

public  void   initialize_graphics_lights(
    Gwindow  window )
{
    set_current_window( window );

    GS_initialize_lights( window->GS_window );
}

public  void  G_set_ambient_light(
    Gwindow        window,
    Colour         colour )
{
    set_current_window( window );

    GS_set_ambient_light( window->GS_window, colour );
}

public  void  G_define_light(
    Gwindow         window,
    int             light_index,
    Light_types     type,
    Colour          colour,
    Vector          *direction,
    Point           *position,
    Real            spot_exponent,
    Real            spot_angle )
{
    set_current_window( window );

    GS_define_light( window->GS_window, light_index, type, colour,
                     direction, position, spot_exponent, spot_angle );
}

public  void  G_set_light_state(
    Gwindow         window,
    int             light_index,
    BOOLEAN         state )
{
    set_current_window( window );

    if( state )
        G_set_view_type( window, WORLD_VIEW );

    GS_set_light_state( window->GS_window, light_index, state );
}
