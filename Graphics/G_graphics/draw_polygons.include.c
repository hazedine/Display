#if  !defined(DEF_ONE_COLOUR) && defined(DEF_NORMALS)

#define  SET_COLOUR( colour ) \
    { \
        scaled_colour = scale_colour( colour, diffuse_coefficient ); \
        set_colour( window, scaled_colour ); \
    }

#else

#define  SET_COLOUR( colour ) set_colour( window, colour )

#endif

{
    int                ind, pt_index, start_index, end_index, n_polygons;
    int                *indices;
    Point              *points;
#if  !defined(DEF_ONE_COLOUR) && defined(DEF_NORMALS)
    Real               diffuse_coefficient =
                                  (Real) Surfprop_d( polygons->surfprop );
    Colour             scaled_colour;
#endif


#ifdef  DEF_NORMALS
    Vector             *normals = polygons->normals;
#endif
    Smallest_int       *visibilities;

    points = polygons->points;
    indices = polygons->indices;
    n_polygons = polygons->n_items;
    visibilities = polygons->visibilities;

#ifdef  DEF_NORMALS
    set_surface_property( window, polygons->colours[0], &polygons->surfprop );
#endif

#ifdef  DEF_ONE_COLOUR
    SET_COLOUR( polygons->colours[0] );
#else
    GS_set_ambient_and_diffuse_mode( ON );
#endif

#ifdef  DEF_WIREFRAME
    BEGIN_DRAW_OBJECTS( window, window->interrupt_interval, n_polygons, TRUE )
#else
    BEGIN_DRAW_OBJECTS( window, window->interrupt_interval, n_polygons, FALSE )
#endif

        if( visibilities == NULL || visibilities[OBJECT_INDEX] )
        {
            start_index = START_INDEX( polygons->end_indices, OBJECT_INDEX );
            end_index = polygons->end_indices[OBJECT_INDEX];

#ifdef  DEF_PER_ITEM_COLOURS
            SET_COLOUR( polygons->colours[OBJECT_INDEX] );
#endif

#ifdef DEF_WIREFRAME
            GS_begin_closed_line();
#else
            GS_begin_polygon();
#endif
            for_less( pt_index, start_index, end_index )
            {
                ind = indices[pt_index];

#ifdef  DEF_PER_VERTEX_COLOURS
                SET_COLOUR( polygons->colours[ind] );
#endif

#ifdef  DEF_NORMALS
                GS_set_normal( &normals[ind] );
#endif
                GS_set_point( &points[ind] );
            }

#ifdef DEF_WIREFRAME
            GS_end_closed_line();
#else
            GS_end_polygon();
#endif
        }

    END_DRAW_OBJECTS

#ifndef  DEF_ONE_COLOUR
    GS_set_ambient_and_diffuse_mode( OFF );
#endif
}

#undef  SET_COLOUR
