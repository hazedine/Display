 
#include  <volume_io/internal_volume_io.h>
#include  <graphics.h>

public  void  G_set_shaded_state(
    Gwindow      window,
    BOOLEAN      state )
{
    if( state != window->shaded_mode_state )
        window->shaded_mode_state = state;
}

public  void  G_set_shading_type(
    Gwindow        window,
    Shading_types  type )
{
    Bitplane_types  bitplane;

    if( type != window->shading_type )
    {
        set_current_window( window );

        bitplane = G_get_bitplanes( window );

        G_set_bitplanes( window, NORMAL_PLANES );

        GS_set_shade_model( type );

        restore_bitplanes( window, bitplane );
            
        window->shading_type = type;
    }
}

public  BOOLEAN  G_get_lighting_state(
    Gwindow      window )
{
    return( window->lighting_state );
}

public  void  G_set_lighting_state(
    Gwindow      window,
    BOOLEAN      state )
{
    if( state != window->lighting_state )
    {
        set_current_window( window );

        GS_set_lighting_state( window->GS_window, state );
            
        window->lighting_state = state;
    }
}

public  void  G_backface_culling_state(
    Gwindow     window,
    BOOLEAN     state )
{
    if( state != window->backface_culling_state )
    {
        set_current_window( window );

        GS_backface_culling_state( state );

        window->backface_culling_state = state;
    }
}

public  void  G_set_render_lines_as_curves_state(
    Gwindow     window,
    BOOLEAN     state )
{
    window->render_lines_as_curves_state = state;
}

public  void  G_set_n_curve_segments(
    Gwindow  window,
    int      n_segments )
{
    if( n_segments != window->n_curve_segments && n_segments > 0 )
    {
        set_current_window( window );

        GS_set_n_curve_segments( n_segments );

        window->n_curve_segments = n_segments;
    }
}

public  void  G_set_markers_labels_visibility(
    Gwindow    window,
    BOOLEAN    state )
{
    window->marker_labels_visibility = state;
}
