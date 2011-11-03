
#include  <volume_io/internal_volume_io.h>
#include  <graphics.h>
#include  <random_order.h>

#define  MAX_LINE_WIDTH  1000.0f

private  void     draw_marker_as_cube( Gwindow, Colour );

private  void  set_colour(
    Gwindow   window,
    Colour    colour )
{
     if( window->colour_map_state ||
         window->current_bitplanes == OVERLAY_PLANES )
         GS_set_colour_index( colour );
     /*else if( stereo_flag )
     {
         int  intensity;
         intensity = 0.324 * get_Colour_r(c) +
                     0.499 * get_Colour_g(c) +
                     0.177 * get_Colour_b(c) + 0.5;
         cpack( make_Colour( intensity, intensity, intensity ) );
     } */
     else
         GS_set_colour( colour );
}

public  void  initialize_display_interrupts(
    Gwindow        window )
{
    window->interrupt_allowed = FALSE;
    window->continuation_flag = FALSE;
    window->interrupt_occurred = FALSE;
    window->interrupt_interval = 100;
    window->interrupt_time_interval = 0.3;
}

public  void  G_set_drawing_interrupt_state(
    Gwindow          window,
    BOOLEAN         state )
{
    window->interrupt_allowed = state;
}

public  BOOLEAN  G_get_drawing_interrupt_state(
    Gwindow          window )
{
    return( window->interrupt_allowed );
}

public  void  G_set_drawing_interrupt_check_n_objects(
    Gwindow          window,
    int             interval )
{
    window->interrupt_interval = interval;
}

public  void  G_set_drawing_interrupt_time(
    Gwindow          window,
    Real             interval )
{
    window->interrupt_time_interval = interval;
}

public  void  G_start_interrupt_test(
    Gwindow          window )
{
    Real   current_time;

    current_time = current_realtime_seconds();
    window->interrupt_time = current_time + window->interrupt_time_interval;
}

public  BOOLEAN  G_get_interrupt_occurred(
    Gwindow          window )
{
    return( window->interrupt_occurred );
}

public  void  G_clear_drawing_interrupt_flag(
    Gwindow          window )
{
    window->interrupt_occurred = FALSE;
}

public  void  set_continuation_flag(
    Gwindow         window,
    BOOLEAN         state )
{
    window->continuation_flag = state;
}

#ifndef   DISALLOW_DRAWING_INTERRUPT

/*  macro for drawing objects with interrupts  */

#define  BEGIN_DRAW_OBJECTS( window, check_every, n_objects, wireframe_flag ) \
{ \
    Random_mask_type            OBJECT_INDEX; \
    int                         _i, _iter, _n_iters, _n_done, _n_before_check; \
    int                         _check_every, _n_objects_to_do; \
    BOOLEAN                     _random_order, _interrupt_allowed; \
    Random_mask_type            _random_order_mask; \
    Real                        interrupt_at, current_time; \
    int                         _n_objects, _first_object, _second_object; \
    BOOLEAN                     _wireframe_flag; \
    static   BOOLEAN            const_true = TRUE; \
 \
    _n_objects = (n_objects); \
    _wireframe_flag = (wireframe_flag); \
 \
    (window)->interrupt_occurred = FALSE; \
\
    interrupt_at = (window)->interrupt_time; \
    _interrupt_allowed = (G_get_drawing_interrupt_state( window ) && \
                         (_n_objects) > (window)->interrupt_interval ); \
 \
    if( _interrupt_allowed ) \
    { \
        _random_order = (window->zbuffer_state || \
                         const_true == _wireframe_flag); \
        if( _random_order ) \
            _random_order_mask = get_random_order_mask( _n_objects ); \
 \
        _check_every = (check_every); \
        _n_before_check = _check_every; \
    } \
    else \
        _random_order = FALSE; \
   \
    if( (window)->continuation_flag ) \
    { \
        _first_object = (window)->next_item; \
        _n_done = (window)->n_items_done;   \
        (window)->continuation_flag = FALSE;            \
    } \
    else \
    { \
        _first_object = 0; \
        _n_done = 0; \
    } \
 \
    if( view_is_stereo(window) )                                            \
    {                                                                         \
        _n_iters = 2;                                                         \
        set_view_for_eye( window, 0 );                                        \
    }                                                                         \
    else                                                                      \
        _n_iters = 1;                                                         \
                                                                              \
    _n_objects_to_do = _n_objects;                                            \
    if( _n_objects > 1 )                                                      \
        _second_object = get_random_int( _n_objects-1 ) + 1;                  \
    else                                                                      \
        _second_object = 0;                                                   \
                                                                              \
    for_less( _iter, 0, _n_iters )                                            \
    {                                                                         \
        OBJECT_INDEX = (Random_mask_type) _first_object;                      \
                                                                              \
        if( _iter == 1 )                                                      \
        {                                                                     \
            set_view_for_eye( window, 1 );                                    \
            _interrupt_allowed = FALSE;                                       \
        }                                                                     \
                                                                              \
        for_less( _i, _n_done, _n_objects_to_do ) \
        {


#define  END_DRAW_OBJECTS  \
            if( _random_order ) \
            { \
                if( _i == 0 ) \
                { \
                    OBJECT_INDEX = (Random_mask_type) _second_object; \
                } \
                else \
                { \
                    do \
                    { \
                        INCREMENT_RANDOM_ORDER( OBJECT_INDEX, \
                                                _random_order_mask ); \
                    } while( OBJECT_INDEX >= (Random_mask_type) _n_objects ); \
                } \
            } \
            else \
            { \
                ++OBJECT_INDEX; \
            } \
 \
            if( _interrupt_allowed ) \
            { \
                --_n_before_check; \
                if( _n_before_check == 0 ) \
                { \
                    current_time = current_realtime_seconds(); \
                    if( current_time >= interrupt_at ) \
                    { \
                        _n_objects_to_do = _i+1; \
                        (window)->n_items_done = _n_objects_to_do; \
                        (window)->next_item = (int) OBJECT_INDEX; \
                        (window)->interrupt_occurred = TRUE; \
                        break; \
                    } \
                    _n_before_check = _check_every; \
                } \
            } \
        } \
    } \
\
    if( view_is_stereo(window) ) \
    { \
        set_view_for_eye( window, -1 );                                        \
    } \
}

#else

#define  BEGIN_DRAW_OBJECTS( window, check_every, n_objects, wireframe_flag )\
{                                                                             \
    int                      _i, _n_iters, OBJECT_INDEX;                      \
                                                                              \
    if( view_is_stereo(window) )                                            \
    {                                                                         \
        _n_iters = 2;                                                         \
        set_view_for_eye( window, 0 );                                        \
    }                                                                         \
    else                                                                      \
        _n_iters = 1;                                                         \
                                                                              \
    for_less( _i, 0, _n_iters )                                               \
    {                                                                         \
        if( _i == 1 )                                                         \
        {                                                                     \
            set_view_for_eye( window, 1 );                                    \
        }                                                                     \
                                                                              \
        for_less( OBJECT_INDEX, 0, n_objects )                                \
        {

#define  END_DRAW_OBJECTS                                                     \
        }                                                                     \
    }                                                                         \
    if( view_is_stereo(window) )                                              \
    {                                                                         \
        set_view_for_eye( window, -1 );                                       \
    }                                                                         \
}

#endif

private  void  set_surface_property(
    Gwindow        window,
    Colour         col,
    Surfprop       *surfprop )
{
    GS_set_surface_property( window->GS_window, col, surfprop );
}

public  void  initialize_surface_property(
    Gwindow        window )
{
    Colour                 col;
    static  Surfprop       surfprop = { 1.0f, 1.0f, 1.0f, 40.0f, 1.0f };

    col = make_Colour( 255, 255, 255 );

    GS_initialize_surface_property( window->GS_window );
    set_surface_property( window, col, &surfprop );
}

public  BOOLEAN  view_is_stereo(
    Gwindow        window )
{
    return( window->stereo_flag &&
            !window->colour_map_state &&
            window->current_bitplanes == NORMAL_PLANES &&
            (window->current_view_type == WORLD_VIEW ||
             window->current_view_type == MODEL_VIEW) );
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : about_to_draw_graphics
@INPUT      : window
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Called just before drawing into a window, to set the current
              window, and clear it, if automatic clearing is enabled.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

private  void  about_to_draw_graphics( Gwindow        window )
{
    set_current_window( window );
    check_window_cleared( window );
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : draw_polygons_one_colour
@INPUT      : window
              polygons
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Draws a set of polygons which has a single colour for all
              polygons.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

private  void  draw_polygons_one_colour(
    Gwindow         window,
    polygons_struct *polygons )
{
#define     DEF_ONE_COLOUR

    if( window->shaded_mode_state )
    {
        if( window->lighting_state && polygons->normals != (Vector *) 0 )
        {
#define     DEF_NORMALS
#include    "draw_polygons.include.c"
#undef      DEF_NORMALS
        }
        else
        {
#include    "draw_polygons.include.c"
        }
    }
    else
    {
#define  DEF_WIREFRAME
        if( window->lighting_state && polygons->normals != (Vector *) 0 )
        {
#define     DEF_NORMALS
#include    "draw_polygons.include.c"
#undef      DEF_NORMALS
        }
        else
        {
#include    "draw_polygons.include.c"
        }
#undef   DEF_WIREFRAME
    }

#undef  DEF_ONE_COLOUR
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : draw_polygons_per_item_colours
@INPUT      : window
              polygons
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Draws a set of polygons which has a separate colour for each
              polygon.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

private  void  draw_polygons_per_item_colours(
    Gwindow         window,
    polygons_struct *polygons )
{
#define DEF_PER_ITEM_COLOURS

    if( window->shaded_mode_state )
    {
        if( window->lighting_state && polygons->normals != (Vector *) 0 )
        {
#define     DEF_NORMALS
#include    "draw_polygons.include.c"
#undef      DEF_NORMALS
        }
        else
        {
#include    "draw_polygons.include.c"
        }
    }
    else
    {
#define  DEF_WIREFRAME
        if( window->lighting_state && polygons->normals != (Vector *) 0 )
        {
#define     DEF_NORMALS
#include    "draw_polygons.include.c"
#undef      DEF_NORMALS
        }
        else
        {
#include    "draw_polygons.include.c"
        }
#undef   DEF_WIREFRAME
    }

#undef DEF_PER_ITEM_COLOURS
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : draw_polygons_per_vertex_colours
@INPUT      : window
              polygons
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Draws a set of polygons which have a separate colour for each
              vertex.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

private  void  draw_polygons_per_vertex_colours(
    Gwindow         window,
    polygons_struct *polygons )
{
#define DEF_PER_VERTEX_COLOURS

    if( window->shaded_mode_state )
    {
        if( window->lighting_state && polygons->normals != (Vector *) 0 )
        {
#define     DEF_NORMALS
#include    "draw_polygons.include.c"
#undef      DEF_NORMALS
        }
        else
        {
#include    "draw_polygons.include.c"
        }
    }
    else
    {
#define  DEF_WIREFRAME
        if( window->lighting_state && polygons->normals != (Vector *) 0 )
        {
#define     DEF_NORMALS
#include    "draw_polygons.include.c"
#undef      DEF_NORMALS
        }
        else
        {
#include    "draw_polygons.include.c"
        }
#undef   DEF_WIREFRAME
    }

#undef      DEF_PER_VERTEX_COLOURS
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : draw_polygons
@INPUT      : windows
              polygons
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Draws a set of polygons in the window.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

private  void  draw_polygons(
    Gwindow         window,
    polygons_struct *polygons )
{
    if( !window->shaded_mode_state &&
        polygons->line_thickness > 1.0f &&
        polygons->line_thickness < MAX_LINE_WIDTH )
    {
        GS_set_line_width( (Real) polygons->line_thickness );
    }

    switch( polygons->colour_flag )
    {
    case  ONE_COLOUR:
        draw_polygons_one_colour( window, polygons );
        break;

    case  PER_ITEM_COLOURS:
        draw_polygons_per_item_colours( window, polygons );
        break;

    case  PER_VERTEX_COLOURS:
        draw_polygons_per_vertex_colours( window, polygons );
        break;
    }

    if( !window->shaded_mode_state &&
        polygons->line_thickness > 1.0f &&
        polygons->line_thickness < MAX_LINE_WIDTH )
        GS_set_line_width( 1.0 );
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : G_draw_polygons
@INPUT      : window
              polygons
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Routine to draw a set of polygons in the window.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  G_draw_polygons(
    Gwindow         window,
    polygons_struct *polygons )
{
    about_to_draw_graphics( window );

    draw_polygons( window, polygons );
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : draw_quadmesh_one_colour
@INPUT      : window
              quadmesh
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Draws a quadmesh of a single colour into the window.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

private  void  draw_quadmesh_one_colour(
    Gwindow         window,
    quadmesh_struct *quadmesh )
{
#define DEF_ONE_COLOUR

    if( window->shaded_mode_state )
    {
        if( window->lighting_state && quadmesh->normals != (Vector *) 0 )
        {
#define     DEF_NORMALS
#include    "draw_quadmesh.include.c"
#undef      DEF_NORMALS
        }
        else
        {
#include    "draw_quadmesh.include.c"
        }
    }
    else
    {
#define  DEF_WIREFRAME
        if( window->lighting_state && quadmesh->normals != (Vector *) 0 )
        {
#define     DEF_NORMALS
#include    "draw_quadmesh.include.c"
#undef      DEF_NORMALS
        }
        else
        {
#include    "draw_quadmesh.include.c"
        }
#undef   DEF_WIREFRAME
    }

#undef  DEF_ONE_COLOUR
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : draw_quadmesh_per_item_colours
@INPUT      : window
              quadmesh
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Draws a quadmesh with a separate colour per quadrilateral.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

private  void  draw_quadmesh_per_item_colours(
    Gwindow         window,
    quadmesh_struct *quadmesh )
{
#define    DEF_PER_ITEM_COLOURS

    if( window->shaded_mode_state )
    {
        if( window->lighting_state && quadmesh->normals != (Vector *) 0 )
        {
#define     DEF_NORMALS
#include    "draw_quadmesh.include.c"
#undef      DEF_NORMALS
        }
        else
        {
#include    "draw_quadmesh.include.c"
        }
    }
    else
    {
#define  DEF_WIREFRAME
        if( window->lighting_state && quadmesh->normals != (Vector *) 0 )
        {
#define     DEF_NORMALS
#include    "draw_quadmesh.include.c"
#undef      DEF_NORMALS
        }
        else
        {
#include    "draw_quadmesh.include.c"
        }
#undef   DEF_WIREFRAME
    }

#undef      DEF_PER_ITEM_COLOURS
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : draw_quadmesh_per_vertex_colours
@INPUT      : window
              quadmesh
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Draws a quadmesh with a separate colour per vertex.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

private  void  draw_quadmesh_per_vertex_colours(
    Gwindow         window,
    quadmesh_struct *quadmesh )
{
#define DEF_PER_VERTEX_COLOURS

    if( window->shaded_mode_state )
    {
        if( window->lighting_state && quadmesh->normals != (Vector *) 0 )
        {
#define     DEF_NORMALS
#include    "draw_quadmesh.include.c"
#undef      DEF_NORMALS
        }
        else
        {
#include    "draw_quadmesh.include.c"
        }
    }
    else
    {
#define  DEF_WIREFRAME
        if( window->lighting_state && quadmesh->normals != (Vector *) 0 )
        {
#define     DEF_NORMALS
#include    "draw_quadmesh.include.c"
#undef      DEF_NORMALS
        }
        else
        {
#include    "draw_quadmesh.include.c"
        }
#undef   DEF_WIREFRAME
    }

#undef      DEF_PER_VERTEX_COLOURS
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : G_draw_quadmesh
@INPUT      : window
              quadmesh
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Draws a quadmesh in the window.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  G_draw_quadmesh(
    Gwindow         window,
    quadmesh_struct *quadmesh )
{
    about_to_draw_graphics( window );

    switch( quadmesh->colour_flag )
    {
    case  ONE_COLOUR:
        draw_quadmesh_one_colour( window, quadmesh );
        break;

    case  PER_ITEM_COLOURS:
        draw_quadmesh_per_item_colours( window, quadmesh );
        break;

    case  PER_VERTEX_COLOURS:
        draw_quadmesh_per_vertex_colours( window, quadmesh );
        break;
    }
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : G_draw_lines
@INPUT      : window
              lines
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Draws the lines in the given window.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  G_draw_lines(
    Gwindow         window,
    lines_struct    *lines )
{
    int      n_lines, i, pt_index, start_index, end_index;
    float    geom[4][3];
    BOOLEAN  wrap_around;
    BOOLEAN  lines_as_curves, save_lights;

    about_to_draw_graphics( window );

    save_lights = G_get_lighting_state( window );
    G_set_lighting_state( window, OFF );

    if( lines->line_thickness > 1.0f && lines->line_thickness < MAX_LINE_WIDTH )
        GS_set_line_width( (Real) lines->line_thickness );

    n_lines = lines->n_items;

    lines_as_curves = window->render_lines_as_curves_state;

    if( lines->colour_flag == ONE_COLOUR )
        set_colour( window, lines->colours[0] );

    BEGIN_DRAW_OBJECTS( window, window->interrupt_interval, n_lines, TRUE )

        if( lines->colour_flag == PER_ITEM_COLOURS )
            set_colour( window, lines->colours[OBJECT_INDEX] );

        start_index = START_INDEX( lines->end_indices, OBJECT_INDEX );
        end_index = lines->end_indices[OBJECT_INDEX];

        if( start_index == end_index-1 )
        {
            GS_begin_point();
                pt_index = lines->indices[start_index];
                if( lines->colour_flag == PER_VERTEX_COLOURS )
                    set_colour( window, lines->colours[pt_index] );
                GS_set_point( &lines->points[pt_index] );
            GS_end_point();
        }
        else if( lines_as_curves )
        {
            wrap_around = ( lines->indices[start_index] ==
                            lines->indices[end_index-1] );

            for_less( i, start_index, end_index-1 )
            {
                if( i == start_index )
                {
                    if( wrap_around && end_index - 2 >= start_index )
                        pt_index = end_index-2;
                    else
                        pt_index = start_index;
                }
                else
                    pt_index = i - 1;

                pt_index = lines->indices[pt_index];
                geom[0][0] = Point_x(lines->points[pt_index]);
                geom[0][1] = Point_y(lines->points[pt_index]);
                geom[0][2] = Point_z(lines->points[pt_index]);

                if( lines->colour_flag == PER_VERTEX_COLOURS )
                    set_colour( window, lines->colours[pt_index] );

                pt_index = lines->indices[i];
                geom[1][0] = Point_x(lines->points[pt_index]);
                geom[1][1] = Point_y(lines->points[pt_index]);
                geom[1][2] = Point_z(lines->points[pt_index]);

                pt_index = lines->indices[i+1];
                geom[2][0] = Point_x(lines->points[pt_index]);
                geom[2][1] = Point_y(lines->points[pt_index]);
                geom[2][2] = Point_z(lines->points[pt_index]);

                if( i == end_index-2 )
                {
                    if( wrap_around && start_index + 1 < end_index )
                        pt_index = start_index + 1;
                    else
                        pt_index = end_index-1;
                }
                else
                    pt_index = i + 2;

                pt_index = lines->indices[pt_index];
                geom[3][0] = Point_x(lines->points[pt_index]);
                geom[3][1] = Point_y(lines->points[pt_index]);
                geom[3][2] = Point_z(lines->points[pt_index]);

                GS_curve( geom );
            }
        }
        else
        {
            GS_begin_line();

                for_less( i, start_index, end_index )
                {
                    pt_index = lines->indices[i];
                    if( lines->colour_flag == PER_VERTEX_COLOURS )
                        set_colour( window, lines->colours[pt_index] );
                    GS_set_point( &lines->points[pt_index] );
                }

            GS_end_line();
        }

    END_DRAW_OBJECTS

    if( lines->line_thickness > 1.0f && lines->line_thickness < MAX_LINE_WIDTH )
        GS_set_line_width( 1.0 );

    G_set_lighting_state( window, save_lights );
}

/* ------------------------------ text and fonts ------------------- */

public  Real  G_get_text_height(
    Font_types       type,
    Real             size )
{
    return( GS_get_character_height( type, size ) );
}

public  Real  G_get_text_length(
    STRING           str,
    Font_types       type,
    Real             size )
{
    return( GS_get_text_length( str, type, size ) );
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : G_draw_text
@INPUT      : window
              text
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Draws the text in the given window.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  G_draw_text(
    Gwindow         window,
    text_struct     *text )
{
    about_to_draw_graphics( window );

    set_colour( window, text->colour );

    BEGIN_DRAW_OBJECTS( window, window->interrupt_interval, 1, TRUE )

        GS_set_raster_position( (Real) Point_x(text->origin),
                                (Real) Point_y(text->origin),
                                (Real) Point_z(text->origin) );

        GS_draw_text( text->font, text->size, text->string );

    END_DRAW_OBJECTS
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : G_draw_marker
@INPUT      : window
              marker
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Draws a 3D marker in the window
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  G_draw_marker(
    Gwindow         window,
    marker_struct   *marker )
{
    BOOLEAN     save_lights;
    Transform   transform;

    about_to_draw_graphics( window );

    save_lights = G_get_lighting_state( window );
    G_set_lighting_state( window, OFF );

    set_colour( window, marker->colour );

    BEGIN_DRAW_OBJECTS( window, window->interrupt_interval, 1, TRUE )

        if( marker->size <= 0.0 )
        {
            GS_begin_point();
                GS_set_point( &marker->position );
            GS_end_point();
        }
        else
        {
            GS_push_transform();

            make_translation_transform( (Real) Point_x(marker->position),
                                        (Real) Point_y(marker->position),
                                        (Real) Point_z(marker->position),
                                        &transform );
            GS_mult_transform( &transform );

            make_scale_transform( marker->size, marker->size, marker->size,
                                  &transform );
            GS_mult_transform( &transform );

            switch( marker->type )
            {
            case  BOX_MARKER:
                draw_marker_as_cube( window, marker->colour );
                break;

            case  SPHERE_MARKER:
                draw_marker_as_cube( window, marker->colour );
                break;
            }

            GS_pop_transform();
        }

        if( window->marker_labels_visibility )
        {
            GS_set_raster_position( (Real) Point_x(marker->position) +
                                       1.5 * (Real) marker->size / 2.0,
                                    (Real) Point_y(marker->position),
                                    (Real) Point_z(marker->position) );

            GS_draw_text( FIXED_FONT, 10.0, marker->label );
        }

    END_DRAW_OBJECTS

    G_set_lighting_state( window, save_lights );
}

private  void  draw_marker_as_cube( Gwindow         window, Colour colour )
{
    static  Colour           colours[1];
    static  Point            points[24] = {
                                   { -0.5f, -0.5f, -0.5f },
                                   { -0.5f, -0.5f,  0.5f },
                                   { -0.5f,  0.5f,  0.5f },
                                   { -0.5f,  0.5f, -0.5f },

                                   {  0.5f, -0.5f, -0.5f },
                                   {  0.5f,  0.5f, -0.5f },
                                   {  0.5f,  0.5f,  0.5f },
                                   {  0.5f, -0.5f,  0.5f },

                                   { -0.5f, -0.5f, -0.5f },
                                   {  0.5f, -0.5f, -0.5f },
                                   {  0.5f, -0.5f,  0.5f },
                                   { -0.5f, -0.5f,  0.5f },

                                   { -0.5f,  0.5f, -0.5f },
                                   { -0.5f,  0.5f,  0.5f },
                                   {  0.5f,  0.5f,  0.5f },
                                   {  0.5f,  0.5f, -0.5f },

                                   { -0.5f, -0.5f, -0.5f },
                                   { -0.5f,  0.5f, -0.5f },
                                   {  0.5f,  0.5f, -0.5f },
                                   {  0.5f, -0.5f, -0.5f },

                                   { -0.5f, -0.5f,  0.5f },
                                   {  0.5f, -0.5f,  0.5f },
                                   {  0.5f,  0.5f,  0.5f },
                                   { -0.5f,  0.5f,  0.5f } };


    static  Vector           normals[24] = {

                                   { -1.0f,  0.0f,  0.0f },
                                   { -1.0f,  0.0f,  0.0f },
                                   { -1.0f,  0.0f,  0.0f },
                                   { -1.0f,  0.0f,  0.0f },

                                   {  1.0f,  0.0f,  0.0f },
                                   {  1.0f,  0.0f,  0.0f },
                                   {  1.0f,  0.0f,  0.0f },
                                   {  1.0f,  0.0f,  0.0f },

                                   {  0.0f, -1.0f,  0.0f },
                                   {  0.0f, -1.0f,  0.0f },
                                   {  0.0f, -1.0f,  0.0f },
                                   {  0.0f, -1.0f,  0.0f },

                                   {  0.0f,  1.0f,  0.0f },
                                   {  0.0f,  1.0f,  0.0f },
                                   {  0.0f,  1.0f,  0.0f },
                                   {  0.0f,  1.0f,  0.0f },

                                   {  0.0f,  0.0f, -1.0f },
                                   {  0.0f,  0.0f, -1.0f },
                                   {  0.0f,  0.0f, -1.0f },
                                   {  0.0f,  0.0f, -1.0f },

                                   {  0.0f,  0.0f,  1.0f },
                                   {  0.0f,  0.0f,  1.0f },
                                   {  0.0f,  0.0f,  1.0f },
                                   {  0.0f,  0.0f,  1.0f }
                                           };
    static  int              end_indices[6] = { 4, 8, 12, 16, 20, 24 };
    static  int              indices[24] = {  0,  1,  2,  3,  4,  5,  6,  7,
                                              8,  9, 10, 11, 12, 13, 14, 15,
                                             16, 17, 18, 19, 20, 21, 22, 23 };

    static  polygons_struct  polygons = {
                                            ONE_COLOUR,
                                            colours,
                                            { 0.4f, 0.6f, 0.6f, 30.0f, 1.0f },
                                            1.0f,
                                            24,
                                            points,
                                            normals,
                                            6,
                                            end_indices,
                                            indices,
                                            (Smallest_int *) NULL,
                                            (int *) NULL,
                                            (bintree_struct_ptr) NULL
                                        };

    polygons.colours[0] = colour;
    draw_polygons( window, &polygons );
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : G_draw_pixels
@INPUT      : window
              pixels
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Draws the pixels in the window.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  G_draw_pixels(
    Gwindow         window,
    pixels_struct   *pixels )
{
    if( pixels->x_size <= 0 || pixels->y_size <= 0 )
        return;

    about_to_draw_graphics( window );

    if( pixels->x_zoom > 0.0 && pixels->y_zoom > 0.0 )
        GS_set_pixel_zoom( pixels->x_zoom, pixels->y_zoom );
    else
        GS_set_pixel_zoom( 1.0, 1.0 );

    if( window->colour_map_state )
    {
        if( pixels->pixel_type == RGB_PIXEL )
        {
            print_error(
               "G_draw_pixels(): cannot draw rgb pixels in colour map mode.\n");
            return;
        }

        GS_draw_colour_map_pixels( window->x_viewport_min,
                                   window->y_viewport_min, pixels );
    }
    else
    {
        if( pixels->pixel_type != RGB_PIXEL )
        {
            print_error(
               "G_draw_pixels(): cannot draw colour map pixels in rgb mode.\n");
            return;
        }

        GS_draw_rgb_pixels( window->x_viewport_min,
                            window->y_viewport_min, pixels );
    }
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : G_read_pixels
@INPUT      : window
              x_min
              x_max
              y_min
              y_max
@OUTPUT     : pixels
@RETURNS    : 
@DESCRIPTION: Reads the pixels from the frame buffer.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  G_read_pixels(
    Gwindow         window,
    int             x_min,
    int             x_max,
    int             y_min,
    int             y_max,
    Colour          pixels[] )
{
    set_current_window( window );

    GS_read_pixels( G_get_colour_map_state(window),
                    x_min, x_max, y_min, y_max, pixels );
}
