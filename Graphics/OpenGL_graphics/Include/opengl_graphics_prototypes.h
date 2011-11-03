#ifndef  DEF_opengl_graphics_prototypes
#define  DEF_opengl_graphics_prototypes

public  Colour  make_rgba_Colour(
    int    r,
    int    g,
    int    b,
    int    a );

public  int  get_Colour_r(
    Colour   colour );

public  int  get_Colour_g(
    Colour   colour );

public  int  get_Colour_b(
    Colour   colour );

public  int  get_Colour_a(
    Colour   colour );

public  void  GS_set_point(
    Point  *point );

public  void  GS_set_normal(
    Vector  *normal );

public  void  GS_set_colour(
    Colour  colour );

public  void  GS_set_colour_index(
    Colour  colour );

public  void  GS_set_ambient_and_diffuse_mode(
    BOOLEAN  state );

public  void  GS_initialize_surface_property(
    GSwindow        window );

public  void  GS_set_surface_property(
    GSwindow       window,
    Colour         col,
    Surfprop       *surfprop );

public  void  GS_set_line_width(
    Real  line_width );

public  void  GS_curve(
    float  geom[4][3] );

public  void  GS_begin_point( void );

public  void  GS_end_point( void );

public  void  GS_begin_line( void );

public  void  GS_end_line( void );

public  void  GS_begin_closed_line( void );

public  void  GS_end_closed_line( void );

public  void  GS_begin_polygon( void );

public  void  GS_end_polygon( void );

public  void  GS_begin_quad_strip( void );

public  void  GS_end_quad_strip( void );

public  void  GS_set_raster_position(
    Real  x,
    Real  y,
    Real  z );

public  void  GS_set_pixel_zoom(
    Real  x_zoom,
    Real  y_zoom );

public  void  GS_draw_colour_map_pixels(
    int             x_viewport_min,
    int             y_viewport_min,
    pixels_struct   *pixels );

public  void  GS_draw_rgb_pixels(
    int             x_viewport_min,
    int             y_viewport_min,
    pixels_struct   *pixels );

public  void  GS_read_pixels(
    BOOLEAN         colour_map_state,
    int             x_min,
    int             x_max,
    int             y_min,
    int             y_max,
    Colour          pixels[] );

public  Real  GS_get_character_height(
    Font_types       type,
    Real             size );

public  void  GS_draw_text(
    Font_types   type,
    Real         size,
    STRING       string );

public  Real  GS_get_text_length(
    STRING           str,
    Font_types       type,
    Real             size );

public  void  GS_set_update_function(
    void  (*func)( Window_id ) );

public  void  GS_set_update_overlay_function(
    void  (*func)( Window_id ) );

public  void  GS_set_resize_function(
    void  (*func)( Window_id, int, int, int, int ) );

public  void  GS_set_key_down_function(
    void  (*func)( Window_id, int, int, int, int ) );

public  void  GS_set_key_up_function(
    void  (*func)( Window_id, int, int, int, int ) );

public  void  GS_set_mouse_movement_function(
    void  (*func)( Window_id, int, int ) );

public  void  GS_set_left_mouse_down_function(
    void  (*func)( Window_id, int, int, int ) );

public  void  GS_set_left_mouse_up_function(
    void  (*func)( Window_id, int, int, int ) );

public  void  GS_set_middle_mouse_down_function(
    void  (*func)( Window_id, int, int, int ) );

public  void  GS_set_middle_mouse_up_function(
    void  (*func)( Window_id, int, int, int ) );

public  void  GS_set_right_mouse_down_function(
    void  (*func)( Window_id, int, int, int ) );

public  void  GS_set_right_mouse_up_function(
    void  (*func)( Window_id, int, int, int ) );

public  void  GS_set_iconify_function(
    void  (*func)( Window_id ) );

public  void  GS_set_deiconify_function(
    void  (*func)( Window_id ) );

public  void  GS_set_enter_function(
    void  (*func)( Window_id ) );

public  void  GS_set_leave_function(
    void  (*func)( Window_id ) );

public  void  GS_set_quit_function(
    void  (*func)( Window_id ) );

public  void  GS_set_update_flag(
    GSwindow   window  );

public  void  GS_add_timer_function(
    Real          seconds,
    void          (*func) ( void * ),
    void          *data );

public  void  GS_add_idle_function(
    void  (*func) ( void * ),
    void          *data );

public  void  GS_remove_idle_function(
    void  (*func) ( void * ),
    void          *data );

public  void  GS_event_loop( void );

public  void  GS_set_mouse_position(
    int       x_screen,
    int       y_screen );

public  BOOLEAN  GS_are_mouse_coordinates_in_screen_space( void );

public  void  GS_initialize_lights(
    GSwindow        window );

public  void  delete_lights(
    GSwindow        window );

public  void  GS_set_ambient_light(
    GSwindow       window,
    Colour         colour );

public  void  GS_define_light(
    GSwindow        window,
    int             light_index,
    Light_types     type,
    Colour          colour,
    Vector          *direction,
    Point           *position,
    Real            spot_exponent,
    Real            spot_angle );

public  void  GS_set_light_state(
    GSwindow        window,
    int             light_index,
    BOOLEAN         state );

public  void  redefine_lights(
    GSwindow        window );

public  void  GS_set_shade_model(
    Shading_types  type );

public  void  GS_turn_off_blend_function( void );

public  void  GS_turn_on_blend_function( void );

public  void  GS_set_lighting_state(
    GSwindow     window,
    BOOLEAN      state );

public  void  GS_backface_culling_state(
    BOOLEAN     state );

public  void  GS_set_n_curve_segments(
    int      n_segments );

public  void  GS_load_transform(
    Transform  *transform );

public  void  GS_mult_transform(
    Transform  *transform );

public  void  GS_get_transform(
    Transform  *trans );

public  void  GS_ortho(
    Real   x_min,
    Real   x_max,
    Real   y_min,
    Real   y_max,
    Real   z_min,
    Real   z_max );

public  void  GS_ortho_2d(
    int   x_min,
    int   x_max,
    int   y_min,
    int   y_max );

public  void  GS_frustum(
    Real   x_min,
    Real   x_max,
    Real   y_min,
    Real   y_max,
    Real   z_min,
    Real   z_max );

public  void  GS_initialize_window_view(
    GSwindow   window );

public  void  GS_push_transform( void );

public  void  GS_pop_transform( void );

public  void  GS_set_viewport(
    int            x_min,
    int            x_max,
    int            y_min,
    int            y_max );

public  void  clear_overlay_planes( void );

public  void  GS_set_matrix_mode(
    Matrix_modes   mode );

public  Window_id  GS_get_window_id(
    GSwindow  window );

public  void  GS_set_current_window(
    GSwindow        window );

public  void  GS_initialize( void );

public  void  GS_terminate( void );

public  BOOLEAN  GS_can_switch_double_buffering( void );

public  BOOLEAN  GS_can_switch_colour_map_mode( void );

public  Status  GS_create_window(
    GSwindow       window,
    STRING         title,
    int            x_pos,
    int            y_pos,
    int            width,
    int            height,
    BOOLEAN        colour_map_desired,
    BOOLEAN        double_buffer_desired,
    BOOLEAN        depth_buffer_desired,
    int            n_overlay_planes_desired,
    BOOLEAN        *actual_colour_map_flag,
    BOOLEAN        *actual_double_buffer_flag,
    BOOLEAN        *actual_depth_buffer_flag,
    int            *actual_n_overlay_planes );

public  void  print_info( void );

public  void  GS_set_window_title(
    GSwindow   window,
    STRING     title );

public  BOOLEAN  GS_has_transparency_mode( void );

public  BOOLEAN  GS_has_rgb_mode( void );

public  BOOLEAN  GS_set_double_buffer_state(
    GSwindow       window,
    BOOLEAN        flag );

public  BOOLEAN  GS_set_colour_map_state(
    GSwindow       window,
    BOOLEAN        flag );

public  int  GS_get_n_colour_map_entries(
    GSwindow  window,
    BOOLEAN   double_buffer_state );

public  void  GS_set_colour_map_entry(
    GSwindow        window,
    Bitplane_types  bitplanes,
    int             ind,
    Colour          colour );

public  BOOLEAN  GS_is_double_buffer_supported( void );

public  BOOLEAN  GS_is_depth_buffer_supported( void );

public  void  GS_set_depth_function(
    Depth_functions  func );

public  void  GS_set_depth_buffer_state(
    BOOLEAN         flag );

public  Status  GS_delete_window(
    GSwindow   window );

public  int  GS_get_monitor_width( void );

public  int  GS_get_monitor_height( void );

public  void  GS_get_window_size(
    int        *x_size,
    int        *y_size );

public  void  GS_get_window_position(
    int        *x_position,
    int        *y_position );

public  void  GS_clear_depth_buffer( void );

public  void  GS_clear_overlay( void );

public  void  GS_clear_window(
    GSwindow        window,
    int             x_size,
    int             y_size,
    Bitplane_types  current_bitplanes,
    BOOLEAN         colour_map_state,
    BOOLEAN         zbuffer_state,
    Colour          colour );

public  void  GS_clear_viewport(
    GSwindow        window,
    int             x_viewport_min,
    int             x_viewport_max,
    int             y_viewport_min,
    int             y_viewport_max,
    int             x_size,
    int             y_size,
    Bitplane_types  current_bitplanes,
    BOOLEAN         colour_map_state,
    BOOLEAN         zbuffer_state,
    Colour          colour );

public  void  GS_flush( void );

public  void  GS_append_to_last_update(
    GSwindow   window,
    BOOLEAN    zbuffer_state,
    int        x_size,
    int        y_size );

public  int  GS_get_num_overlay_planes( void );

public  void  GS_set_bitplanes( 
    GSwindow         window,
    Bitplane_types   bitplanes );

public  void  GS_set_overlay_colour_map(
    GSwindow         window,
    int              ind,
    Colour           colour );

public  void  GS_swap_buffers( void );
#endif
