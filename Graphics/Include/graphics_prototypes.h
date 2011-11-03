#ifndef  DEF_graphics_prototypes
#define  DEF_graphics_prototypes

public  void  initialize_display_interrupts(
    Gwindow        window );

public  void  G_set_drawing_interrupt_state(
    Gwindow          window,
    BOOLEAN         state );

public  BOOLEAN  G_get_drawing_interrupt_state(
    Gwindow          window );

public  void  G_set_drawing_interrupt_check_n_objects(
    Gwindow          window,
    int             interval );

public  void  G_set_drawing_interrupt_time(
    Gwindow          window,
    Real             interval );

public  void  G_start_interrupt_test(
    Gwindow          window );

public  BOOLEAN  G_get_interrupt_occurred(
    Gwindow          window );

public  void  G_clear_drawing_interrupt_flag(
    Gwindow          window );

public  void  set_continuation_flag(
    Gwindow         window,
    BOOLEAN         state );

public  void  initialize_surface_property(
    Gwindow        window );

public  BOOLEAN  view_is_stereo(
    Gwindow        window );

public  void  G_draw_polygons(
    Gwindow         window,
    polygons_struct *polygons );

public  void  G_draw_quadmesh(
    Gwindow         window,
    quadmesh_struct *quadmesh );

public  void  G_draw_lines(
    Gwindow         window,
    lines_struct    *lines );

public  Real  G_get_text_height(
    Font_types       type,
    Real             size );

public  Real  G_get_text_length(
    STRING           str,
    Font_types       type,
    Real             size );

public  void  G_draw_text(
    Gwindow         window,
    text_struct     *text );

public  void  G_draw_marker(
    Gwindow         window,
    marker_struct   *marker );

public  void  G_draw_pixels(
    Gwindow         window,
    pixels_struct   *pixels );

public  void  G_read_pixels(
    Gwindow         window,
    int             x_min,
    int             x_max,
    int             y_min,
    int             y_max,
    Colour          pixels[] );

public  void  draw_object(
    Gwindow         window,
    object_struct   *object );

public  BOOLEAN  G_get_left_mouse_button( void );

public  BOOLEAN  G_get_middle_mouse_button( void );

public  BOOLEAN  G_get_right_mouse_button( void );

public  BOOLEAN  G_get_shift_key_state( void );

public  BOOLEAN  G_get_ctrl_key_state( void );

public  BOOLEAN  G_get_alt_key_state( void );

public  void  G_set_update_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, void * ),
    void                    *func_data );

public  void  G_set_update_overlay_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, void * ),
    void                    *func_data );

public  void  G_set_resize_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, int, int, int, void * ),
    void                    *func_data );

public  void  G_set_key_down_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, void * ),
    void                    *func_data );

public  void  G_set_key_up_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, void * ),
    void                    *func_data );

public  void  G_set_mouse_movement_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, int, void * ),
    void                    *func_data );

public  void  G_set_left_mouse_down_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, int, void * ),
    void                    *func_data );

public  void  G_set_left_mouse_up_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, int, void * ),
    void                    *func_data );

public  void  G_set_middle_mouse_down_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, int, void * ),
    void                    *func_data );

public  void  G_set_middle_mouse_up_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, int, void * ),
    void                    *func_data );

public  void  G_set_right_mouse_down_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, int, void * ),
    void                    *func_data );

public  void  G_set_right_mouse_up_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, int, int, void * ),
    void                    *func_data );

public  void  G_set_iconify_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, void * ),
    void                    *func_data );

public  void  G_set_deiconify_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, void * ),
    void                    *func_data );

public  void  G_set_window_enter_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, void * ),
    void                    *func_data );

public  void  G_set_window_leave_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, void * ),
    void                    *func_data );

public  void  G_set_window_quit_function(
    Gwindow                 window,
    void                    (*func) ( Gwindow, void * ),
    void                    *func_data );

public  void  initialize_callbacks_for_window(
    Gwindow                 window );

public  void  G_set_update_flag(
    Gwindow  window );

public  void  G_add_timer_function(
    Real          seconds,
    void          (*func) ( void * ),
    void          *data );

public  void  G_add_idle_function(
    void  (*func) ( void * ),
    void          *data );

public  void  G_remove_idle_function(
    void  (*func) ( void * ),
    void          *data );

public  void  G_main_loop( void );

public  void  G_set_mouse_position(
    int       x_screen,
    int       y_screen );

public  BOOLEAN  G_is_mouse_in_window(
    Gwindow window );

public  void  G_get_mouse_screen_position(
    int            *x_screen_pos,
    int            *y_screen_pos );

public  BOOLEAN  G_get_mouse_position(
    Gwindow        window,
    int            *x_pixel_pos,
    int            *y_pixel_pos );

public  BOOLEAN  G_get_mouse_position_0_to_1(
    Gwindow        window,
    Real           *x_pos,
    Real           *y_pos );

public  void  G_set_default_update_min_interval(
    Real   seconds );

public  void  G_set_window_update_min_interval(
    Gwindow   window,
    Real      seconds );

public  void  initialize_graphics_struct(
    graphics_struct   *graphics );

public  void  delete_graphics_struct(
    graphics_struct   *graphics );

public  void  set_graphics_viewport(
    graphics_struct   *graphics,
    int               viewport_index,
    int               x_min,
    int               x_max,
    int               y_min,
    int               y_max );

public  void  get_graphics_viewport(
    graphics_struct   *graphics,
    int               viewport_index,
    int               *x_min,
    int               *x_max,
    int               *y_min,
    int               *y_max );

public  void  set_graphics_viewport_background(
    graphics_struct   *graphics,
    int               viewport_index,
    Colour            background_colour,
    int               background_colour_index );

public  void  add_object_to_viewport(
    graphics_struct   *graphics,
    int               viewport_index,
    Bitplane_types    bitplane,
    object_struct     *object );

public  void  remove_object_from_viewport(
    graphics_struct   *graphics,
    int               viewport_index,
    Bitplane_types    bitplane,
    object_struct     *object );

public  void  set_viewport_update_flag( 
    graphics_struct   *graphics,
    int               viewport_index,
    Bitplane_types    bitplane );

public  BOOLEAN  get_viewport_update_flag( 
    graphics_struct   *graphics,
    int               viewport_index,
    Bitplane_types    bitplane,
    int               buffer );

public  void  set_bitplanes_clear_flag( 
    graphics_struct   *graphics,
    Bitplane_types    bitplane );

public  void  set_viewport_bitplane_objects_visibility(
    graphics_struct   *graphics,
    int               viewport_index,
    Bitplane_types    bitplane,
    BOOLEAN           visibility );

public  void  set_viewport_objects_visibility(
    graphics_struct   *graphics,
    int               viewport_index,
    BOOLEAN           visibility );

public  BOOLEAN  redraw_out_of_date_viewports(
    graphics_struct   *graphics,
    Gwindow           window,
    int               current_buffer );

public  void   initialize_graphics_lights(
    Gwindow  window );

public  void  G_set_ambient_light(
    Gwindow        window,
    Colour         colour );

public  void  G_define_light(
    Gwindow         window,
    int             light_index,
    Light_types     type,
    Colour          colour,
    Vector          *direction,
    Point           *position,
    Real            spot_exponent,
    Real            spot_angle );

public  void  G_set_light_state(
    Gwindow         window,
    int             light_index,
    BOOLEAN         state );

public  Random_mask_type  get_random_order_mask(
    int  size );

public  void  G_set_shaded_state(
    Gwindow      window,
    BOOLEAN      state );

public  void  G_set_shading_type(
    Gwindow        window,
    Shading_types  type );

public  BOOLEAN  G_get_lighting_state(
    Gwindow      window );

public  void  G_set_lighting_state(
    Gwindow      window,
    BOOLEAN      state );

public  void  G_backface_culling_state(
    Gwindow     window,
    BOOLEAN     state );

public  void  G_set_render_lines_as_curves_state(
    Gwindow     window,
    BOOLEAN     state );

public  void  G_set_n_curve_segments(
    Gwindow  window,
    int      n_segments );

public  void  G_set_markers_labels_visibility(
    Gwindow    window,
    BOOLEAN    state );

public  void  initialize_window_view(
    Gwindow   window );

public  void  set_view_for_eye(
    Gwindow         window,
    int             which_eye );

public  void  G_set_view_type(
    Gwindow         window,
    View_types      view_type );

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
    Real           window_height );

public  void  G_set_modeling_transform(
    Gwindow         window,
    Transform       *transform );

public  void  G_transform_point(
    Gwindow         window,
    Point           *point,
    View_types      view_type,
    int             *x_pixel,
    int             *y_pixel );

public  void  update_transforms(
    Gwindow         window );

public  void  G_push_transform(
    Gwindow      window,
    Transform    *transform );

public  void  G_pop_transform(
    Gwindow    window );

public  void  window_was_resized(
    Gwindow    window );

public  void  G_set_viewport(
    Gwindow        window,
    int            x_min,
    int            x_max,
    int            y_min,
    int            y_max );

public  int  get_n_graphics_windows( void );

public  Gwindow  get_nth_graphics_window(
    int   i );

public  void  set_current_window( Gwindow   window );

public  Gwindow  get_current_window( void );

public  Status  G_create_window(
    STRING         title,
    int            x_pos,
    int            y_pos,
    int            width,
    int            height,
    BOOLEAN        colour_map_desired,
    BOOLEAN        double_buffer_desired,
    BOOLEAN        depth_buffer_desired,
    int            n_overlay_planes_desired,
    Gwindow        *window );

public  Status  G_delete_window(
    Gwindow   window );

public  void  G_set_window_title(
    Gwindow   window,
    STRING    title );

public  void  G_terminate( void );

public  BOOLEAN  G_is_double_buffer_supported( void );

public  BOOLEAN  G_get_double_buffer_state(
    Gwindow        window );

public  void  G_set_double_buffer_state(
    Gwindow        window,
    BOOLEAN        flag );

public  BOOLEAN  G_is_depth_buffer_supported( void );

public  void  G_set_zbuffer_state(
    Gwindow         window,
    BOOLEAN         flag );

public  BOOLEAN  G_get_zbuffer_state(
    Gwindow         window );

public  BOOLEAN  G_get_colour_map_state(
    Gwindow        window );

public  void  G_set_colour_map_state(
    Gwindow        window,
    BOOLEAN        flag );

public  int  G_get_n_colour_map_entries(
    Gwindow        window );

public  void  G_set_colour_map_entry(
    Gwindow         window,
    int             ind,
    Colour          colour );

public  void  restore_bitplanes(
    Gwindow         window,
    Bitplane_types  bitplane );

public  int  G_get_monitor_width( void );

public  int  G_get_monitor_height( void );

public  Real  G_get_monitor_widths_to_eye( void );

public  void  G_get_window_size(
    Gwindow        window,
    int            *x_size,
    int            *y_size );

public  void  G_get_window_position(
    Gwindow        window,
    int            *x_pos,
    int            *y_pos );

public  void  G_set_background_colour(
    Gwindow         window,
    Colour          colour );

public  Colour  G_get_background_colour(
    Gwindow         window );

public  void  G_clear_window(
    Gwindow    window );

public  void  G_clear_viewport(
    Gwindow    window,
    Colour     colour );

public  void  G_set_automatic_clear_state(
    Gwindow    window,
    BOOLEAN    state );

public  void  G_update_window( Gwindow   window );

public  void  check_window_cleared(
     Gwindow    window  );

public  BOOLEAN  G_has_overlay_planes( void );

public  BOOLEAN  G_window_has_overlay_planes(
    Gwindow   window );

public  void  G_set_bitplanes(
    Gwindow          window,
    Bitplane_types   bitplanes );

public  Bitplane_types  G_get_bitplanes(
    Gwindow          window );

public  BOOLEAN  G_can_switch_double_buffering( void );

public  BOOLEAN  G_can_switch_colour_map_mode( void );

public  BOOLEAN  G_has_transparency_mode( void );

public  BOOLEAN  G_has_rgb_mode( void );

public  int  G_get_n_overlay_planes( void );

public  void  G_set_overlay_colour_map(
    Gwindow         window,
    int             ind,
    Colour          colour );

public  void  G_append_to_last_update(
     Gwindow   window );

public  void  G_continue_last_update(
     Gwindow   window );

public  void  G_set_transparency_state(
    Gwindow        window,
    BOOLEAN        state );
#endif
