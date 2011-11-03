#ifndef  DEF_glut_window_prototypes
#define  DEF_glut_window_prototypes

public  void  WS_initialize( void );

public  Status  WS_create_window(
    STRING                 title,
    int                    initial_x_pos,
    int                    initial_y_pos,
    int                    initial_x_size,
    int                    initial_y_size,
    BOOLEAN                colour_map_mode,
    BOOLEAN                double_buffer_flag,
    BOOLEAN                depth_buffer_flag,
    int                    n_overlay_planes,
    BOOLEAN                *actual_colour_map_mode,
    BOOLEAN                *actual_double_buffer_flag,
    BOOLEAN                *actual_depth_buffer_flag,
    int                    *actual_n_overlay_planes,
    WSwindow               window );

public  BOOLEAN  WS_set_double_buffer_state(
    WSwindow               window,
    BOOLEAN                double_buffer_flag );

public  BOOLEAN  WS_set_colour_map_state(
    WSwindow               window,
    BOOLEAN                colour_map_flag );

public  void  WS_set_window_title(
    WSwindow   window,
    STRING     title );

public  void  WS_delete_window(
    WSwindow  window );

public  BOOLEAN  WS_window_has_overlay_planes(
    WSwindow  window );

public  void  WS_set_current_window(
    WSwindow          window );

public  Window_id   WS_get_window_id(
    WSwindow  window );

public  void  WS_set_bitplanes(
    WSwindow          window,
    Bitplane_types    bitplanes );

public  int    WS_get_n_overlay_planes( void );

public  void  WS_get_window_position(
    int          *x_pos,
    int          *y_pos );

public  void  WS_get_window_size(
    int          *x_size,
    int          *y_size );

public  void  glut_set_colour_entry(
    int      ind,
    Real     r,
    Real     g,
    Real     b );

public  void  WS_set_colour_map_entry(
    WSwindow          window,
    Bitplane_types    bitplane,
    int               ind,
    Colour            colour );

public  void  WS_set_overlay_colour_map_entry(
    WSwindow          window,
    int               ind,
    Colour            colour );

public  void  WS_swap_buffers( void );

public  void  WS_draw_text(
    Font_types  type,
    Real        size,
    STRING      string );

public  Real  WS_get_character_height(
    Font_types       type,
    Real             size );

public  Real  WS_get_text_length(
    STRING           str,
    Font_types       type,
    Real             size );

public  void  WS_get_screen_size(
    int   *x_size, 
    int   *y_size  );

public  void  WS_set_mouse_position(
    int       x_screen,
    int       y_screen );

public  void  WS_set_update_function(
    void  (*func)( Window_id ) );

public  void  WS_set_update_overlay_function(
    void  (*func)( Window_id ) );

public  void  WS_set_resize_function(
    void  (*func)( Window_id, int, int, int, int ) );

public  void  WS_set_key_down_function(
    void  (*func)( Window_id, int, int, int, int ) );

public  void  WS_set_key_up_function(
    void  (*func)( Window_id, int, int, int, int ) );

public  void  WS_set_mouse_movement_function(
    void  (*func)( Window_id, int, int ) );

public  void  WS_set_left_mouse_down_function(
    void  (*func)( Window_id, int, int, int ) );

public  void  WS_set_left_mouse_up_function(
    void  (*func)( Window_id, int, int, int ) );

public  void  WS_set_middle_mouse_down_function(
    void  (*func)( Window_id, int, int, int ) );

public  void  WS_set_middle_mouse_up_function(
    void  (*func)( Window_id, int, int, int ) );

public  void  WS_set_right_mouse_down_function(
    void  (*func)( Window_id, int, int, int ) );

public  void  WS_set_right_mouse_up_function(
    void  (*func)( Window_id, int, int, int ) );

public  void  WS_set_iconify_function(
    void  (*func)( Window_id ) );

public  void  WS_set_deiconify_function(
    void  (*func)( Window_id ) );

public  void  WS_set_enter_function(
    void  (*func)( Window_id ) );

public  void  WS_set_leave_function(
    void  (*func)( Window_id ) );

public  void  WS_set_quit_function(
    void  (*func)( Window_id ) );

public  void  WS_add_timer_function(
    Real          seconds,
    void          (*func) ( void * ),
    void          *data );

public  void  WS_add_idle_function(
    void  (*func) ( void * ),
    void          *data );

public  void  WS_remove_idle_function(
    void  (*func) ( void * ),
    void          *data );

public  void  WS_event_loop( void );

public  void  WS_set_update_flag(
    WSwindow   window  );

public  void  copy_X_colours(
    int  n_colours_to_copy );
#endif
