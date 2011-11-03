#ifndef  DEF_GRAPHICS
#define  DEF_GRAPHICS

#include  <GS_graphics.h>
#include  <bicpl.h>
#include  <random_order.h>

typedef  struct   window_struct
{
    GSwindow               GS_window;
    int                    x_origin, y_origin;          
    int                    x_size, y_size;          
    Colour                 background_colour;
    Colour                 background_colour_index;
    View_types             current_view_type;
    Transform              projection_matrices[(int) N_VIEW_TYPES];
    Transform              left_projection_matrices[(int) N_VIEW_TYPES];
    Transform              right_projection_matrices[(int) N_VIEW_TYPES];
    Transform              viewing_matrices[(int) N_VIEW_TYPES];
    Transform              modeling_transform;
    int                    n_overlay_planes;
    Bitplane_types         current_bitplanes;
    BOOLEAN                automatic_clear_flag;
    BOOLEAN                bitplanes_cleared[N_BITPLANE_TYPES];

    Real                   front_clip_distance;
    Real                   back_clip_distance;
    BOOLEAN                perspective_flag;
    Real                   perspective_distance;
    BOOLEAN                stereo_flag;
    Real                   eye_separation;
    Real                   window_width;
    Real                   window_height;

    BOOLEAN                double_buffer_available;
    BOOLEAN                double_buffer_state;

    BOOLEAN                zbuffer_available;
    BOOLEAN                zbuffer_state;
    BOOLEAN                colour_map_state;

    BOOLEAN                shaded_mode_state;
    Shading_types          shading_type;
    BOOLEAN                lighting_state;
    BOOLEAN                transparency_state;
    BOOLEAN                backface_culling_state;
    BOOLEAN                render_lines_as_curves_state;
    int                    n_curve_segments;
    BOOLEAN                marker_labels_visibility;

    int                    x_viewport_min;
    int                    x_viewport_max;
    int                    y_viewport_min;
    int                    y_viewport_max;

    BOOLEAN                interrupt_allowed;
    BOOLEAN                continuation_flag;
    BOOLEAN                interrupt_occurred;
    int                    interrupt_interval;
    Real                   interrupt_time;
    Real                   interrupt_time_interval;
    int                    next_item;
    int                    n_items_done;

    BOOLEAN                update_required_flag;
    Real                   last_update_time;
    BOOLEAN                last_update_was_idle;
    int                    n_update_timers;
    int                    n_update_timers_to_ignore;

    BOOLEAN                overlay_update_required_flag;
    Real                   last_overlay_update_time;
    BOOLEAN                last_overlay_update_was_idle;

    Real                   min_update_time;

    void    (*update_callback) ( struct window_struct *, void * );
    void    *update_data;

    void    (*update_overlay_callback) ( struct window_struct *, void * );
    void    *update_overlay_data;

    void    (*resize_callback) ( struct window_struct *, int, int, int, int,
                                 void * );
    void    *resize_data;

    void    (*key_down_callback) ( struct window_struct *, int, void * );
    void    *key_down_data;

    void    (*key_up_callback) ( struct window_struct *, int, void * );
    void    *key_up_data;

    void    (*mouse_movement_callback) ( struct window_struct *, int, int,
                                         void * );
    void    *mouse_movement_data;

    void    (*left_mouse_down_callback) ( struct window_struct *, int, int,
                                          void * );
    void    *left_mouse_down_data;

    void    (*left_mouse_up_callback) ( struct window_struct *, int, int,
                                          void * );
    void    *left_mouse_up_data;

    void    (*middle_mouse_down_callback) ( struct window_struct *, int, int,
                                          void * );
    void    *middle_mouse_down_data;

    void    (*middle_mouse_up_callback) ( struct window_struct *, int, int,
                                          void * );
    void    *middle_mouse_up_data;

    void    (*right_mouse_down_callback) ( struct window_struct *, int, int,
                                          void * );
    void    *right_mouse_down_data;

    void    (*right_mouse_up_callback) ( struct window_struct *, int, int,
                                          void * );
    void    *right_mouse_up_data;

    void    (*iconify_callback) ( struct window_struct *, void * );
    void    *iconify_data;

    void    (*deiconify_callback) ( struct window_struct *, void * );
    void    *deiconify_data;

    void    (*enter_callback) ( struct window_struct *, void * );
    void    *enter_data;

    void    (*leave_callback) ( struct window_struct *, void * );
    void    *leave_data;

    void    (*quit_callback) ( struct window_struct *, void * );
    void    *quit_data;
} window_struct;

typedef  struct  window_struct   *Gwindow;

#include  <graphics_struct.h>

#ifndef  public
#define       public
#define       public_was_defined_here
#endif

#include  <graphics_prototypes.h>

#ifdef  public_was_defined_here
#undef       public
#undef       public_was_defined_here
#endif

#endif
