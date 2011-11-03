#include  <volume_io/internal_volume_io.h>
#include  <graphics.h>

public  void  draw_object(
    Gwindow         window,
    object_struct   *object )
{
    if( object->visibility )
    {
        switch( object->object_type )
        {
        case LINES:
            G_draw_lines( window, get_lines_ptr(object) );
            break;
        case MARKER:
            G_draw_marker( window, get_marker_ptr(object) );
            break;
        case PIXELS:
            G_draw_pixels( window, get_pixels_ptr(object) );
            break;
        case POLYGONS:
            G_draw_polygons( window, get_polygons_ptr(object) );
            break;
        case QUADMESH:
            G_draw_quadmesh( window, get_quadmesh_ptr(object) );
            break;
        case TEXT:
            G_draw_text( window, get_text_ptr(object) );
            break;
        }
    }
}
