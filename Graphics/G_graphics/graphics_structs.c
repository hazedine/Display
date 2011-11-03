#include  <volume_io/internal_volume_io.h>
#include  <graphics.h>

private  void  delete_viewport( viewport_struct  * );

public  void  initialize_graphics_struct(
    graphics_struct   *graphics )
{
    Bitplane_types   bitplane;

    graphics->n_viewports = 0;

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
    {
        graphics->clear_bitplane_flags[bitplane][0] = FALSE;
        graphics->clear_bitplane_flags[bitplane][1] = FALSE;
    }
}

public  void  delete_graphics_struct(
    graphics_struct   *graphics )
{
    int        i;

    for_less( i, 0, graphics->n_viewports )
    {
        delete_viewport( &graphics->viewports[i] );
    }

    if( graphics->n_viewports > 0 )
        FREE( graphics->viewports );
}

private  void  initialize_viewport(
    viewport_struct  *viewport )
{
    Bitplane_types   bitplane;

    viewport->background_colour = make_Colour( 0, 0, 0 );
    viewport->background_colour_index = 0;

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
    {
        viewport->bitplanes[bitplane].n_objects = 0;
        viewport->bitplanes[bitplane].update_flag[0] = TRUE;
        viewport->bitplanes[bitplane].update_flag[1] = TRUE;
    }
}

private  void  delete_viewport(
    viewport_struct  *viewport )
{
    int              i;
    Bitplane_types   bitplane;

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
    {
        for_less( i, 0, viewport->bitplanes[bitplane].n_objects )
            delete_object( viewport->bitplanes[bitplane].objects[i] );

        if( viewport->bitplanes[bitplane].n_objects > 0 )
            FREE( viewport->bitplanes[bitplane].objects );
    }
}

private  void  check_viewport_exists(
    graphics_struct   *graphics,
    int               viewport_index )
{
    int      i;

    if( viewport_index >= graphics->n_viewports )
    {
        SET_ARRAY_SIZE( graphics->viewports, graphics->n_viewports,
                        viewport_index + 1, DEFAULT_CHUNK_SIZE );

        for_less( i, graphics->n_viewports, viewport_index + 1 )
        {
            initialize_viewport( &graphics->viewports[i] );
        }
        graphics->n_viewports = viewport_index + 1;
    }
}

public  void  set_graphics_viewport(
    graphics_struct   *graphics,
    int               viewport_index,
    int               x_min,
    int               x_max,
    int               y_min,
    int               y_max )
{
    Bitplane_types   bitplane;

    check_viewport_exists( graphics, viewport_index );

    graphics->viewports[viewport_index].x_min = x_min;
    graphics->viewports[viewport_index].x_max = x_max;
    graphics->viewports[viewport_index].y_min = y_min;
    graphics->viewports[viewport_index].y_max = y_max;

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
        set_viewport_update_flag( graphics, viewport_index, bitplane );
}

public  void  get_graphics_viewport(
    graphics_struct   *graphics,
    int               viewport_index,
    int               *x_min,
    int               *x_max,
    int               *y_min,
    int               *y_max )
{
    *x_min = graphics->viewports[viewport_index].x_min;
    *x_max = graphics->viewports[viewport_index].x_max;
    *y_min = graphics->viewports[viewport_index].y_min;
    *y_max = graphics->viewports[viewport_index].y_max;
}

public  void  set_graphics_viewport_background(
    graphics_struct   *graphics,
    int               viewport_index,
    Colour            background_colour,
    int               background_colour_index )
{
    Bitplane_types   bitplane;

    check_viewport_exists( graphics, viewport_index );

    graphics->viewports[viewport_index].background_colour = background_colour;
    graphics->viewports[viewport_index].background_colour_index =
                                                       background_colour_index;

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
        set_viewport_update_flag( graphics, viewport_index, bitplane );
}

public  void  add_object_to_viewport(
    graphics_struct   *graphics,
    int               viewport_index,
    Bitplane_types    bitplane,
    object_struct     *object )
{
    check_viewport_exists( graphics, viewport_index );

    ADD_ELEMENT_TO_ARRAY( 
        graphics->viewports[viewport_index].bitplanes[bitplane].objects,
        graphics->viewports[viewport_index].bitplanes[bitplane].n_objects,
        object, DEFAULT_CHUNK_SIZE );

    set_viewport_update_flag( graphics, viewport_index, bitplane );
}

public  void  remove_object_from_viewport(
    graphics_struct   *graphics,
    int               viewport_index,
    Bitplane_types    bitplane,
    object_struct     *object )
{
    int      i;

    for_less( i, 0, graphics->viewports[viewport_index].
                                     bitplanes[bitplane].n_objects )
    {
        if( graphics->viewports[viewport_index].bitplanes[bitplane].objects[i]
                                                                  == object )
            break;
    }

    if( i < graphics->viewports[viewport_index].bitplanes[bitplane].n_objects )
    {
        DELETE_ELEMENT_FROM_ARRAY(
         graphics->viewports[viewport_index].bitplanes[bitplane].objects,
         graphics->viewports[viewport_index].bitplanes[bitplane].n_objects,
         i, DEFAULT_CHUNK_SIZE );

        set_viewport_update_flag( graphics, viewport_index, bitplane );
    }
    else
    {
        handle_internal_error( "remove_object_from_viewport" );
    }
}

public  void  set_viewport_update_flag( 
    graphics_struct   *graphics,
    int               viewport_index,
    Bitplane_types    bitplane )
{
    check_viewport_exists( graphics, viewport_index );

    graphics->viewports[viewport_index].bitplanes[bitplane].update_flag[0]
                                                           = TRUE;
    graphics->viewports[viewport_index].bitplanes[bitplane].update_flag[1]
                                                           = TRUE;
}

public  BOOLEAN  get_viewport_update_flag( 
    graphics_struct   *graphics,
    int               viewport_index,
    Bitplane_types    bitplane,
    int               buffer )
{
    check_viewport_exists( graphics, viewport_index );

    return( graphics->viewports[viewport_index].
                    bitplanes[bitplane].update_flag[buffer] );
}

public  void  set_bitplanes_clear_flag( 
    graphics_struct   *graphics,
    Bitplane_types    bitplane )
{
    graphics->clear_bitplane_flags[bitplane][0] = TRUE;
    graphics->clear_bitplane_flags[bitplane][1] = TRUE;
}

public  void  set_viewport_bitplane_objects_visibility(
    graphics_struct   *graphics,
    int               viewport_index,
    Bitplane_types    bitplane,
    BOOLEAN           visibility )
{
    int                        i;
    bitplane_graphics_struct   *viewport_bitplane;

    viewport_bitplane = &graphics->viewports[viewport_index].
                                      bitplanes[bitplane];

    for_less( i, 0, viewport_bitplane->n_objects )
        set_object_visibility( viewport_bitplane->objects[i], visibility );

    if( viewport_bitplane->n_objects > 0 )
        set_viewport_update_flag( graphics, viewport_index, bitplane );
}

public  void  set_viewport_objects_visibility(
    graphics_struct   *graphics,
    int               viewport_index,
    BOOLEAN           visibility )
{
    Bitplane_types  bitplane;

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
    {
        set_viewport_bitplane_objects_visibility( graphics, viewport_index,
                                                  bitplane, visibility );
    }
}

private  BOOLEAN  check_redraw_viewport(
    Gwindow           window,
    viewport_struct   *viewport,
    int               current_buffer )
{
    BOOLEAN          drawn;
    int              i;
    Colour           bg_colour_or_index;
    Bitplane_types   bitplane;

    drawn = viewport->bitplanes[NORMAL_PLANES].update_flag[current_buffer];;

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
    {
        if( (bitplane != OVERLAY_PLANES ||
             G_window_has_overlay_planes(window)) &&
            viewport->bitplanes[bitplane].update_flag[current_buffer] )
        {
            G_set_bitplanes( window, bitplane );
            G_set_view_type( window, PIXEL_VIEW );

            G_set_viewport( window, viewport->x_min, viewport->x_max,
                            viewport->y_min, viewport->y_max );

            if( G_get_colour_map_state(window) )
                bg_colour_or_index = (Colour) viewport->background_colour_index;
            else
                bg_colour_or_index = viewport->background_colour;

            G_clear_viewport( window, bg_colour_or_index );

            for_less( i, 0, viewport->bitplanes[bitplane].n_objects )
                draw_object( window,
                             viewport->bitplanes[bitplane].objects[i] );

            viewport->bitplanes[bitplane].update_flag[current_buffer] =
                                           FALSE;
        }
    }

    return( drawn );
}

public  BOOLEAN  redraw_out_of_date_viewports(
    graphics_struct   *graphics,
    Gwindow           window,
    int               current_buffer )
{
    BOOLEAN          something_drawn;
    int              viewport_index;
    Bitplane_types   bitplane;

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
    {
        if( (bitplane != OVERLAY_PLANES ||
             G_window_has_overlay_planes(window)) &&
            graphics->clear_bitplane_flags[bitplane][current_buffer] )
        {
            G_set_bitplanes( window, bitplane );
            G_clear_window( window );
            graphics->clear_bitplane_flags[bitplane][current_buffer] = FALSE;
        }
    }

    something_drawn = FALSE;

    for_less( viewport_index, 0, graphics->n_viewports )
    {
        if( check_redraw_viewport( window, &graphics->viewports[viewport_index],
                                   current_buffer ) )
        {
            something_drawn = TRUE;
        }
    }

    return( something_drawn );
}
