#ifndef  DEF_GRAPHICS_BASE
#define  DEF_GRAPHICS_BASE

#include  <bicpl.h>

#define  RETURN_KEY    ((char) 13)
#define  DELETE_KEY    ((char) 127)
#define  BACKSPACE_KEY ((char) 8)

#define  LEFT_ARROW_KEY   245
#define  RIGHT_ARROW_KEY  246
#define  DOWN_ARROW_KEY   247
#define  UP_ARROW_KEY     248
#define  LEFT_SHIFT_KEY   249
#define  RIGHT_SHIFT_KEY  250
#define  LEFT_CTRL_KEY    251
#define  RIGHT_CTRL_KEY   252
#define  LEFT_ALT_KEY     253
#define  RIGHT_ALT_KEY    254

typedef  enum  {
                   NO_EVENT,
                   TERMINATE_INTERACTION_EVENT,
                   KEY_DOWN_EVENT,
                   KEY_UP_EVENT,
                   MOUSE_MOVEMENT_EVENT,
                   LEFT_MOUSE_DOWN_EVENT,
                   LEFT_MOUSE_UP_EVENT,
                   MIDDLE_MOUSE_DOWN_EVENT,
                   MIDDLE_MOUSE_UP_EVENT,
                   RIGHT_MOUSE_DOWN_EVENT,
                   RIGHT_MOUSE_UP_EVENT,
                   REDRAW_OVERLAY_EVENT,
                   WINDOW_REDRAW_EVENT,
                   WINDOW_RESIZE_EVENT,
                   WINDOW_ICONIZED_EVENT,
                   WINDOW_DEICONIZED_EVENT,
                   WINDOW_QUIT_EVENT,
                   WINDOW_LEAVE_EVENT,
                   WINDOW_ENTER_EVENT,
                   N_EVENT_TYPES
               } Event_types;

typedef  struct
{
    int           key_pressed;
    int           x_mouse;
    int           y_mouse;
    int           x_position;
    int           y_position;
    int           x_size;
    int           y_size;
} event_info_struct;

typedef  enum  {
                 NORMAL_PLANES,
                 OVERLAY_PLANES,
                 N_BITPLANE_TYPES } Bitplane_types;

#endif
