#include  <volume_io/internal_volume_io.h>
#include  <graphics.h>

static  BOOLEAN    left_button_state = FALSE;
static  BOOLEAN    middle_button_state = FALSE;
static  BOOLEAN    right_button_state = FALSE;
static  int        keyboard_modifiers = 0;
static  int        current_mouse_x = 0;
static  int        current_mouse_y = 0;
static  Gwindow    current_window = NULL;
static  Real       default_min_update_time = 0.01;  /* max 100 frames per sec */
static  int        n_windows_to_update_on_idle = 0;

private  void  check_update_windows(
    void  *void_ptr );

public  BOOLEAN  G_get_left_mouse_button( void )
{
    return( left_button_state );
}

public  BOOLEAN  G_get_middle_mouse_button( void )
{
    return( middle_button_state );
}

public  BOOLEAN  G_get_right_mouse_button( void )
{
    return( right_button_state );
}

public  BOOLEAN  G_get_shift_key_state( void )
{
    return( (keyboard_modifiers & SHIFT_KEY_BIT) != 0 );
}

public  BOOLEAN  G_get_ctrl_key_state( void )
{
    return( (keyboard_modifiers & CTRL_KEY_BIT) != 0 );
}

public  BOOLEAN  G_get_alt_key_state( void )
{
    return( (keyboard_modifiers & ALT_KEY_BIT) != 0 );
}

private  Gwindow  lookup_window_for_window_id(
    Window_id  window_id )
{
    int       i, n_windows;
    Gwindow   window;

    window = get_current_window();

    if( window != NULL &&
        GS_get_window_id( window->GS_window ) == window_id )
    {
        current_window = window;
        return( window );
    }

    n_windows = get_n_graphics_windows();

    for_less( i, 0, n_windows )
    {
        window = get_nth_graphics_window( i );

        if( GS_get_window_id( window->GS_window ) == window_id )
            break;
    }

    if( i >= n_windows )
        current_window = NULL;
    else
        current_window = window;

    return( current_window );
}

private  Gwindow  get_event_window(
    Window_id   window_id )
{
    Gwindow     window;

    window = lookup_window_for_window_id( window_id );

    /*--- since the underlying event handler may have changed the current
          window, we now set the current window to NULL, so the next call to
          set_current_window forces an update of the current window */

    set_current_window( NULL );

/*
    if( window == NULL )
        handle_internal_error( "get_event_window" );
*/

    return( window );
}

private  void  assign_mouse_position(
    Gwindow   window,
    int       x,
    int       y )
{
    current_mouse_x = x;
    current_mouse_y = y;

    if( GS_are_mouse_coordinates_in_screen_space() )
    {
        current_mouse_x -= window->x_origin;
        current_mouse_y -= window->y_origin;
    }
}


private  Gwindow  get_key_or_mouse_event_window(
    Window_id   window_id,
    int         x,
    int         y,
    int         modifier )
{
    Gwindow     window;

    window = get_event_window( window_id );

    if( window != NULL )
    {
        assign_mouse_position( window, x, y );
        keyboard_modifiers = modifier;
    }

    return( window );
}

private  void  update_the_window(
    Gwindow  window )
{
    --n_windows_to_update_on_idle;
    if( n_windows_to_update_on_idle == 0 )
    {
        G_remove_idle_function( check_update_windows, NULL );
    }

    if( window->update_callback != NULL )
    {
        (*window->update_callback)( window, window->update_data );
    }

    window->last_update_time = current_realtime_seconds();;

    window->n_update_timers_to_ignore += window->n_update_timers;
    window->n_update_timers = 0;
    window->update_required_flag = FALSE;
}

private  void  global_update_function(
    Window_id  window_id )
{
    Gwindow     window;

    window = get_event_window( window_id );
    if( window == NULL )
        return;

    update_the_window( window );
}

private  void  update_the_overlay_window(
    Gwindow  window )
{
    if( window->update_overlay_callback != NULL )
        (*window->update_overlay_callback)( window,
                                            window->update_overlay_data );

    window->overlay_update_required_flag = FALSE;
    window->last_overlay_update_time = current_realtime_seconds();;
    window->last_overlay_update_was_idle = FALSE;
}

private  void  global_update_overlay_function(
    Window_id  window_id )
{
    Gwindow     window;

    window = get_event_window( window_id );
    if( window == NULL )
        return;

    update_the_overlay_window( window );
}

private  void  global_resize_function(
    Window_id  window_id,
    int        x,
    int        y,
    int        x_size,
    int        y_size )
{
    Gwindow     window;

    window = get_event_window( window_id );

    if( window == NULL )
        return;

    window->x_origin = x;
    window->y_origin = y;
    window->x_size = x_size;
    window->y_size = y_size;

    window_was_resized( window );

    if( window->resize_callback != NULL )
        (*window->resize_callback)( window, x, y, x_size, y_size,
                                    window->resize_data );
}

private  void  global_key_down_function(
    Window_id  window_id,
    int        key,
    int        x,
    int        y,
    int        modifier )
{
    Gwindow     window;

    window = get_key_or_mouse_event_window( window_id, x, y, modifier );
    if( window == NULL )
        return;

    if( window->key_down_callback != NULL )
        (*window->key_down_callback)( window, key, window->key_down_data );
}

private  void  global_key_up_function(
    Window_id  window_id,
    int        key,
    int        x,
    int        y,
    int        modifier )
{
    Gwindow     window;

    window = get_key_or_mouse_event_window( window_id, x, y, modifier );
    if( window == NULL )
        return;

    if( window->key_up_callback != NULL )
        (*window->key_up_callback)( window, key, window->key_up_data );
}

private  void  global_mouse_movement_function(
    Window_id  window_id,
    int        x,
    int        y )
{
    Gwindow     window;

    window = get_event_window( window_id );
    if( window == NULL )
        return;

    assign_mouse_position( window, x, y );

    if( window->mouse_movement_callback != NULL )
        (*window->mouse_movement_callback)( window, x, y,
                                            window->mouse_movement_data );
}

private  void  global_left_mouse_down_function(
    Window_id  window_id,
    int        x,
    int        y,
    int        modifier )
{
    Gwindow     window;

    window = get_key_or_mouse_event_window( window_id, x, y, modifier );

    if( window == NULL )
        return;

    left_button_state = TRUE;

    if( window->left_mouse_down_callback != NULL )
        (*window->left_mouse_down_callback)( window, x, y,
                                             window->left_mouse_down_data );
}

private  void  global_left_mouse_up_function(
    Window_id  window_id,
    int        x,
    int        y,
    int        modifier )
{
    Gwindow     window;

    window = get_key_or_mouse_event_window( window_id, x, y, modifier );
    if( window == NULL )
        return;

    left_button_state = FALSE;

    if( window->left_mouse_up_callback != NULL )
        (*window->left_mouse_up_callback)( window, x, y,
                                           window->left_mouse_up_data );
}

private  void  global_middle_mouse_down_function(
    Window_id  window_id,
    int        x,
    int        y,
    int        modifier )
{
    Gwindow     window;

    window = get_key_or_mouse_event_window( window_id, x, y, modifier );
    if( window == NULL )
        return;

    middle_button_state = TRUE;

    if( window->middle_mouse_down_callback != NULL )
        (*window->middle_mouse_down_callback)( window, x, y,
                                               window->middle_mouse_down_data );
}

private  void  global_middle_mouse_up_function(
    Window_id  window_id,
    int        x,
    int        y,
    int        modifier )
{
    Gwindow     window;

    window = get_key_or_mouse_event_window( window_id, x, y, modifier );
    if( window == NULL )
        return;

    middle_button_state = FALSE;

    if( window->middle_mouse_up_callback != NULL )
        (*window->middle_mouse_up_callback)( window, x, y,
                                             window->middle_mouse_up_data );
}

private  void  global_right_mouse_down_function(
    Window_id  window_id,
    int        x,
    int        y,
    int        modifier )
{
    Gwindow     window;

    window = get_key_or_mouse_event_window( window_id, x, y, modifier );
    if( window == NULL )
        return;

    right_button_state = TRUE;

    if( window->right_mouse_down_callback != NULL )
        (*window->right_mouse_down_callback)( window, x, y,
                                              window->right_mouse_down_data );
}

private  void  global_right_mouse_up_function(
    Window_id  window_id,
    int        x,
    int        y,
    int        modifier )
{
    Gwindow     window;

    window = get_key_or_mouse_event_window( window_id, x, y, modifier );
    if( window == NULL )
        return;

    right_button_state = FALSE;

    if( window->right_mouse_up_callback != NULL )
        (*window->right_mouse_up_callback)( window, x, y,
                                            window->right_mouse_up_data );
}

private  void  global_iconify_function(
    Window_id  window_id )
{
    Gwindow     window;

    window = get_event_window( window_id );
    if( window == NULL )
        return;

    if( window->iconify_callback != NULL )
        (*window->iconify_callback)( window, window->iconify_data );
}

private  void  global_deiconify_function(
    Window_id  window_id )
{
    Gwindow     window;

    window = get_event_window( window_id );
    if( window == NULL )
        return;

    if( window->deiconify_callback != NULL )
        (*window->deiconify_callback)( window, window->deiconify_data );
}

private  void  global_enter_function(
    Window_id  window_id )
{
    Gwindow     window;

    window = get_event_window( window_id );
    if( window == NULL )
        return;

    if( window->enter_callback != NULL )
        (*window->enter_callback)( window, window->enter_data );
}

private  void  global_leave_function(
    Window_id  window_id )
{
    Gwindow     window;

    window = get_event_window( window_id );
    if( window == NULL )
        return;

    if( window->leave_callback != NULL )
        (*window->leave_callback)( window, window->leave_data );
}

private  void  global_quit_function(
    Window_id  window_id )
{
    Gwindow     window;

    window = get_event_window( window_id );
    if( window == NULL )
        return;

    if( window->quit_callback != NULL )
        (*window->quit_callback)( window, window->quit_data );
}

private  void  initialize_callbacks( void )
{
    GS_set_update_function( global_update_function );
    GS_set_update_overlay_function( global_update_overlay_function );
    GS_set_resize_function( global_resize_function );
    GS_set_key_down_function( global_key_down_function );
    GS_set_key_up_function( global_key_up_function );
    GS_set_mouse_movement_function( global_mouse_movement_function );
    GS_set_left_mouse_down_function( global_left_mouse_down_function );
    GS_set_left_mouse_up_function( global_left_mouse_up_function );
    GS_set_middle_mouse_down_function( global_middle_mouse_down_function );
    GS_set_middle_mouse_up_function( global_middle_mouse_up_function );
    GS_set_right_mouse_down_function( global_right_mouse_down_function );
    GS_set_right_mouse_up_function( global_right_mouse_up_function );
    GS_set_iconify_function( global_iconify_function );
    GS_set_deiconify_function( global_deiconify_function );
    GS_set_enter_function( global_enter_function );
    GS_set_leave_function( global_leave_function );
    GS_set_quit_function( global_quit_function );
}

public  void  G_set_update_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, void * ),
    void                    *func_data )
{
    window->update_callback = func;
    window->update_data = func_data;
}

public  void  G_set_update_overlay_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, void * ),
    void                    *func_data )
{
    window->update_overlay_callback = func;
    window->update_overlay_data = func_data;
}

public  void  G_set_resize_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, int, int, int, void * ),
    void                    *func_data )
{
    window->resize_callback = func;
    window->resize_data = func_data;
}

public  void  G_set_key_down_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, void * ),
    void                    *func_data )
{
    window->key_down_callback = func;
    window->key_down_data = func_data;
}

public  void  G_set_key_up_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, void * ),
    void                    *func_data )
{
    window->key_up_callback = func;
    window->key_up_data = func_data;
}

public  void  G_set_mouse_movement_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, int, void * ),
    void                    *func_data )
{
    window->mouse_movement_callback = func;
    window->mouse_movement_data = func_data;
}

public  void  G_set_left_mouse_down_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, int, void * ),
    void                    *func_data )
{
    window->left_mouse_down_callback = func;
    window->left_mouse_down_data = func_data;
}

public  void  G_set_left_mouse_up_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, int, void * ),
    void                    *func_data )
{
    window->left_mouse_up_callback = func;
    window->left_mouse_up_data = func_data;
}

public  void  G_set_middle_mouse_down_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, int, void * ),
    void                    *func_data )
{
    window->middle_mouse_down_callback = func;
    window->middle_mouse_down_data = func_data;
}

public  void  G_set_middle_mouse_up_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, int, void * ),
    void                    *func_data )
{
    window->middle_mouse_up_callback = func;
    window->middle_mouse_up_data = func_data;
}

public  void  G_set_right_mouse_down_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, int, void * ),
    void                    *func_data )
{
    window->right_mouse_down_callback = func;
    window->right_mouse_down_data = func_data;
}

public  void  G_set_right_mouse_up_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, int, void * ),
    void                    *func_data )
{
    window->right_mouse_up_callback = func;
    window->right_mouse_up_data = func_data;
}

public  void  G_set_iconify_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, void * ),
    void                    *func_data )
{
    window->iconify_callback = func;
    window->iconify_data = func_data;
}

public  void  G_set_deiconify_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, void * ),
    void                    *func_data )
{
    window->deiconify_callback = func;
    window->deiconify_data = func_data;
}

public  void  G_set_window_enter_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, void * ),
    void                    *func_data )
{
    window->enter_callback = func;
    window->enter_data = func_data;
}

public  void  G_set_window_leave_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, void * ),
    void                    *func_data )
{
    window->leave_callback = func;
    window->leave_data = func_data;
}

public  void  G_set_window_quit_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, void * ),
    void                    *func_data )
{
    window->quit_callback = func;
    window->quit_data = func_data;
}

public  void  initialize_callbacks_for_window(
    Gwindow                 window )
{
    window->update_required_flag = FALSE;
    window->last_update_time = -1.0e30;
    window->n_update_timers = 0;
    window->n_update_timers_to_ignore = 0;

    window->overlay_update_required_flag = FALSE;
    window->last_overlay_update_time = -1.0e30;
    window->last_overlay_update_was_idle = FALSE;

    window->min_update_time = default_min_update_time;

    window->update_callback = NULL;
    window->update_overlay_callback = NULL;
    window->resize_callback = NULL;
    window->key_down_callback = NULL;
    window->key_up_callback = NULL;
    window->mouse_movement_callback = NULL;
    window->left_mouse_down_callback = NULL;
    window->left_mouse_up_callback = NULL;
    window->middle_mouse_down_callback = NULL;
    window->middle_mouse_up_callback = NULL;
    window->right_mouse_down_callback = NULL;
    window->right_mouse_up_callback = NULL;
    window->iconify_callback = NULL;
    window->deiconify_callback = NULL;
    window->enter_callback = NULL;
    window->leave_callback = NULL;
    window->quit_callback = NULL;
}

private  void  timer_update_window(
    void   *void_ptr )
{
    Gwindow  window;

    window = (Gwindow) void_ptr;

    if( window->n_update_timers_to_ignore > 0 )
    {
        --window->n_update_timers_to_ignore;
        return;
    }

    --window->n_update_timers;

    if( window->update_required_flag )
    {
        update_the_window( window );
    }
}

/* ARGSUSED */

private  void  check_update_windows(
    void  *void_ptr )
{
    int       i;
    Gwindow   window;

    /*--- get_n_graphics_windows() may return different values as callbacks
          are called */

    for_less( i, 0, get_n_graphics_windows() )
    {
        window = get_nth_graphics_window( i );

        if( window->update_required_flag )
        {
            update_the_window( window );
        }
    }

}

public  void  G_set_update_flag(
    Gwindow  window )
{
    Real   current_time, time_remaining;

    if( window->update_required_flag )
        return;

    window->update_required_flag = TRUE;

    if( n_windows_to_update_on_idle == 0 )
    {
        G_add_idle_function( check_update_windows, NULL );
    }

    ++n_windows_to_update_on_idle;

    current_time = current_realtime_seconds();

    time_remaining = window->min_update_time -
                     (current_time - window->last_update_time);

    if( time_remaining <= 0.0 )
    {
        GS_set_update_flag( window->GS_window );
    }
    else
    {
        G_add_timer_function( time_remaining, timer_update_window,
                              (void *) window );
        ++window->n_update_timers;
    }
}

public  void  G_add_timer_function(
    Real          seconds,
    void          (*func) ( void * ),
    void          *data )
{
    GS_add_timer_function( seconds, func, data );
}

public  void  G_add_idle_function(
    void  (*func) ( void * ),
    void          *data )
{
    GS_add_idle_function( func, data );
}

public  void  G_remove_idle_function(
    void  (*func) ( void * ),
    void          *data )
{
    GS_remove_idle_function( func, data );
}

public  void  G_main_loop( void )
{
    initialize_callbacks();

    GS_event_loop();
}

public  void  G_set_mouse_position(
    int       x_screen,
    int       y_screen )
{
    GS_set_mouse_position( x_screen, y_screen );
}

public  BOOLEAN  G_is_mouse_in_window(
    Gwindow window )
{
    return( current_window == window && window != NULL );
}

public  void  G_get_mouse_screen_position(
    int            *x_screen_pos,
    int            *y_screen_pos )
{
    int       x_mouse, y_mouse;

    if( G_get_mouse_position( current_window, &x_mouse, &y_mouse ) )
    {
        *x_screen_pos = x_mouse + current_window->x_origin;
        *y_screen_pos = y_mouse + current_window->y_origin;
    }
    else
    {
        *x_screen_pos = 0;
        *y_screen_pos = 0;
    }
}

public  BOOLEAN  G_get_mouse_position(
    Gwindow        window,
    int            *x_pixel_pos,
    int            *y_pixel_pos )
{
    BOOLEAN        in_window;

    in_window = G_is_mouse_in_window( window );

    if( window != NULL )
    {
        *x_pixel_pos = current_mouse_x;
        *y_pixel_pos = current_mouse_y;
    }
    
    return( in_window );
}

public  BOOLEAN  G_get_mouse_position_0_to_1(
    Gwindow        window,
    Real           *x_pos,
    Real           *y_pos )
{
    int            x_pixel, y_pixel;
    BOOLEAN        in_window;

    in_window = G_get_mouse_position( window, &x_pixel, &y_pixel );

    if( window != (Gwindow) NULL )
    {
        *x_pos = ((Real) x_pixel + 0.5) / (Real) window->x_size;
        *y_pos = ((Real) y_pixel + 0.5) / (Real) window->y_size;
    }
    
    return( in_window );
}

public  void  G_set_default_update_min_interval(
    Real   seconds )
{
    if( seconds <= 0.0 )
        default_min_update_time = 0.0;
    else
        default_min_update_time = seconds;
}

public  void  G_set_window_update_min_interval(
    Gwindow   window,
    Real      seconds )
{
    if( seconds <= 0.0 )
        window->min_update_time = 0.0;
    else
        window->min_update_time = seconds;
}
