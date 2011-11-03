 
#include  <volume_io/internal_volume_io.h>
#include  <graphics.h>

#define  CLOSEST_FRONT_PLANE    1.0e-5

private  void  set_view_type(
    Gwindow         window,
    View_types      view_type );

/* ----------------------------- MNI Header -----------------------------------
@NAME       : initialize_window_view
@INPUT      : window
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Initializes the window view, called on creation of the window.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  initialize_window_view(
    Gwindow   window )
{
    Transform      identity;
    static  Point  default_origin = { 0.0f, 0.0f, 1.0f };
    static  Vector default_line_of_sight = { 0.0f, 0.0f, -1.0f };
    static  Vector default_up_direction = { 0.0f, 1.0f, 0.0f };
    Real           default_front_clip_distance = 0.01;
    Real           default_back_clip_distance = 2.0;
    BOOLEAN        default_perspective_flag = TRUE;
    Real           default_perspective_distance = 1.0;
    BOOLEAN        default_stereo_flag = FALSE;
    Real           default_eye_separation = 0.1;
    Real           default_window_width = 1.0;
    Real           default_window_height = 1.0;

    make_identity_transform( &identity );

    window->modeling_transform = identity;

    GS_set_matrix_mode( PROJECTION_MATRIX );
    GS_load_transform( &identity );
    GS_ortho( 0.0, 1.0, 0.0, 1.0, 0.0, 1.0 );
    GS_get_transform( &window->projection_matrices[SCREEN_VIEW] );

    GS_set_matrix_mode( VIEWING_MATRIX );

    window->viewing_matrices[SCREEN_VIEW] = identity;
    window->viewing_matrices[PIXEL_VIEW] = identity;

    set_view_type( window, SCREEN_VIEW );

    GS_initialize_window_view( window->GS_window );

    G_set_3D_view( window, &default_origin, &default_line_of_sight,
                   &default_up_direction,
                   default_front_clip_distance, default_back_clip_distance,
                   default_perspective_flag, default_perspective_distance,
                   default_stereo_flag, default_eye_separation,
                   default_window_width, default_window_height );

    window_was_resized( window );
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : set_view_type
@INPUT      : window
              view_type
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Informs the graphics hardware of the current viewing matrix.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

private  void  set_view_type(
    Gwindow         window,
    View_types      view_type )
{
    set_current_window( window );

    window->current_view_type = view_type;

    GS_set_matrix_mode( PROJECTION_MATRIX );

    GS_load_transform( &window->projection_matrices[view_type] );

    GS_set_matrix_mode( VIEWING_MATRIX );

    GS_load_transform( &window->viewing_matrices[view_type] );
}

public  void  set_view_for_eye(
    Gwindow         window,
    int             which_eye )
{
    if( view_is_stereo(window) )
    {
        GS_set_matrix_mode( PROJECTION_MATRIX );
        if( which_eye < 0 )
        {
            GS_load_transform( &window->projection_matrices
                                    [window->current_view_type] );
        }
        else if( which_eye == 0 )
        {
            GS_load_transform( &window->left_projection_matrices
                                    [window->current_view_type] );
            GS_set_colour( make_Colour( 100, 255, 255 ) );
        }
        else
        {
            GS_load_transform( &window->right_projection_matrices
                                    [window->current_view_type] );
            GS_set_colour( make_Colour( 255, 100, 100 ) );
            if( window->zbuffer_state )
                GS_clear_depth_buffer();
        }

        GS_set_matrix_mode( VIEWING_MATRIX );
    }
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : G_set_view_type
@INPUT      : window
              view_type
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Sets the current view type to one of PIXEL_VIEW, THREE_D_VIEW,
              etc.  Any objects drawn subsequently will be transformed by this
              view.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  G_set_view_type(
    Gwindow         window,
    View_types      view_type )
{
    if( window->current_view_type != view_type )
        set_view_type( window, view_type );
}

private  void  define_3D_projection(
    Gwindow        window,
    Real           front_clip_distance,
    Real           back_clip_distance,
    BOOLEAN        perspective_flag,
    Real           perspective_distance,
    BOOLEAN        stereo_flag,
    Real           eye_separation,
    Real           window_width,
    Real           window_height )
{
    Real        real_aspect, virtual_aspect, scaling;
    Transform   translate1, translate2, right_transform;
    Transform   left_transform;

    /* first modify window width and height to match aspect */

    real_aspect = (Real) window->y_size / (Real) window->x_size;
    virtual_aspect = (Real) window_height / (Real) window_width;

    if( virtual_aspect > real_aspect )
        window_width = window_height / real_aspect;
    else if( virtual_aspect < real_aspect )
        window_height = window_width * real_aspect;

    /* set up projection matrix */

    if( perspective_flag || stereo_flag )
    {
        if( front_clip_distance <= 0.0 )
            front_clip_distance = CLOSEST_FRONT_PLANE * back_clip_distance;

        if( front_clip_distance <= 0.0 )
            front_clip_distance = CLOSEST_FRONT_PLANE;

        if( back_clip_distance <= front_clip_distance )
            back_clip_distance = front_clip_distance + CLOSEST_FRONT_PLANE;

        GS_set_matrix_mode( PROJECTION_MATRIX );

        scaling = front_clip_distance / perspective_distance;

        GS_frustum( -window_width / 2.0 * scaling,
                     window_width / 2.0 * scaling,
                    -window_height / 2.0 * scaling,
                     window_height / 2.0 * scaling,
                    front_clip_distance, back_clip_distance );

        GS_get_transform( &window->projection_matrices[WORLD_VIEW] );
        GS_get_transform( &window->projection_matrices[MODEL_VIEW] );

        if( stereo_flag )
        {
            make_translation_transform( -eye_separation / 2.0, 0.0, 0.0,
                                        &translate1 );
            make_translation_transform( eye_separation / 2.0, 0.0, 0.0,
                                        &translate2 );

            concat_transforms( &right_transform, &translate1,
                               &window->projection_matrices[WORLD_VIEW] );
            concat_transforms( &right_transform, &right_transform,
                               &translate2 );

            window->right_projection_matrices[WORLD_VIEW] = right_transform;
            window->right_projection_matrices[MODEL_VIEW] = right_transform;

            /*--- now do left transform */

            concat_transforms( &left_transform, &translate2,
                               &window->projection_matrices[WORLD_VIEW] );
            concat_transforms( &left_transform, &left_transform,
                               &translate1 );

            window->left_projection_matrices[WORLD_VIEW] = left_transform;
            window->left_projection_matrices[MODEL_VIEW] = left_transform;
        }
    }
    else
    {
        if( back_clip_distance <= front_clip_distance )
            back_clip_distance = front_clip_distance + CLOSEST_FRONT_PLANE;

        GS_set_matrix_mode( PROJECTION_MATRIX );

        GS_ortho( -window_width / 2.0, window_width / 2.0,
                  -window_height / 2.0, window_height / 2.0,
                  front_clip_distance, back_clip_distance );

        GS_set_matrix_mode( PROJECTION_MATRIX );
        GS_get_transform( &window->projection_matrices[WORLD_VIEW] );
        GS_get_transform( &window->projection_matrices[MODEL_VIEW] );
    }

    GS_set_matrix_mode( VIEWING_MATRIX );

    set_view_type( window, window->current_view_type );  /* restore view type */
}

public  void  G_set_3D_view(
    Gwindow        window,
    Point          *origin,
    Vector         *line_of_sight,
    Vector         *up_direction,
    Real           front_clip_distance,
    Real           back_clip_distance,
    BOOLEAN        perspective_flag,
    Real           perspective_distance,
    BOOLEAN        stereo_flag,
    Real           eye_separation,
    Real           window_width,
    Real           window_height )
{
    Vector      x_axis, y_axis, z_axis;
    Transform   view_matrix;

    set_current_window( window );

    define_3D_projection( window, front_clip_distance,
                          back_clip_distance,
                          perspective_flag, perspective_distance,
                          stereo_flag, eye_separation,
                          window_width, window_height );

    window->front_clip_distance = front_clip_distance;
    window->back_clip_distance = back_clip_distance;
    window->perspective_flag = perspective_flag;
    window->perspective_distance = perspective_distance;
    window->stereo_flag = stereo_flag;
    window->eye_separation = eye_separation;
    window->window_width = window_width;
    window->window_height = window_height;

    /* compute the viewing matrix */

    SCALE_VECTOR( z_axis, *line_of_sight, -1.0 );
    CROSS_VECTORS( x_axis, *up_direction, z_axis );
    CROSS_VECTORS( y_axis, z_axis, x_axis );

    NORMALIZE_VECTOR( x_axis, x_axis );
    NORMALIZE_VECTOR( y_axis, y_axis );
    NORMALIZE_VECTOR( z_axis, z_axis );

    make_change_from_bases_transform( origin, &x_axis, &y_axis, &z_axis,
                                      &view_matrix );

    window->viewing_matrices[WORLD_VIEW] = view_matrix;

    update_transforms( window );
}

public  void  G_set_modeling_transform(
    Gwindow         window,
    Transform       *transform )
{
    window->modeling_transform = *transform;
    update_transforms( window );
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : G_transform_point
@INPUT      : window
              point
              view_type
@OUTPUT     : x_pixel
              y_pixel
@RETURNS    : 
@DESCRIPTION: Finds the pixel that the given point maps to in the window.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  G_transform_point(
    Gwindow         window,
    Point           *point,
    View_types      view_type,
    int             *x_pixel,
    int             *y_pixel )
{
    Real   x, y, z;

    transform_point( &window->viewing_matrices[view_type],
                     (Real) Point_x(*point),
                     (Real) Point_y(*point),
                     (Real) Point_z(*point),
                     &x, &y, &z );
    transform_point( &window->projection_matrices[view_type], x, y, z,
                     &x, &y, &z );

    *x_pixel = (int) (((Real) x + 1.0) / 2.0 * (Real) window->x_size);
    *y_pixel = (int) (((Real) y + 1.0) / 2.0 * (Real) window->y_size);
}

public  void  update_transforms(
    Gwindow         window )
{
    set_current_window( window );

    concat_transforms( &window->viewing_matrices[MODEL_VIEW],
                       &window->modeling_transform,
                       &window->viewing_matrices[WORLD_VIEW] );

    set_view_type( window, window->current_view_type );  /* restore view type */
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : define_pixel_view
@INPUT      : window
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Defines and records the matrix required to perform a pixel
              view matrix.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

private  void  define_pixel_view(
    Gwindow   window )
{
    set_current_window( window );

    GS_set_matrix_mode( PROJECTION_MATRIX );

    GS_ortho_2d( 0, window->x_viewport_max - window->x_viewport_min,
                 0, window->y_viewport_max - window->y_viewport_min );

    GS_get_transform( &window->projection_matrices[PIXEL_VIEW] );

    GS_set_matrix_mode( VIEWING_MATRIX );

    set_view_type( window, window->current_view_type );  /* restore view type */
}

private  void  push_local_transform( Transform   *transform )
{
    GS_push_transform();
    GS_mult_transform( transform );
}

private  void  pop_local_transform( void )
{
    GS_pop_transform();
}

public  void  G_push_transform(
    Gwindow      window,
    Transform    *transform )
{
    set_current_window( window );

    push_local_transform( transform );
}

public  void  G_pop_transform(
    Gwindow    window )
{
    set_current_window( window );

    pop_local_transform();
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : window_was_resized
@INPUT      : window
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: This is called when a resize window event is noted, to record
              the new size and position, and to reset the viewport.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  window_was_resized(
    Gwindow    window )
{
    set_current_window( window );

    G_set_viewport( window, 0, window->x_size-1, 0, window->y_size-1 );

    define_3D_projection( window,
                          window->front_clip_distance,
                          window->back_clip_distance,
                          window->perspective_flag,
                          window->perspective_distance,
                          window->stereo_flag,
                          window->eye_separation,
                          window->window_width,
                          window->window_height );

    update_transforms( window );
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : G_set_viewport
@INPUT      : window
              x_min    - pixel coordinates
              x_max
              y_min
              y_max
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Sets the viewport into which subsequent drawing will take place.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

public  void  G_set_viewport(
    Gwindow        window,
    int            x_min,
    int            x_max,
    int            y_min,
    int            y_max )
{
    set_current_window( window );

    window->x_viewport_min = x_min;
    window->y_viewport_min = y_min;
    window->x_viewport_max = x_max;
    window->y_viewport_max = y_max;

    GS_set_viewport( x_min, x_max, y_min, y_max );

    define_pixel_view( window );
}
