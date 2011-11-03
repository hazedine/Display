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
    int           j, m, n, n_objects;
    int           item_index;
#ifdef  DEF_WIREFRAME
    int           m_size;
#ifdef  DEF_PER_ITEM_COLOURS
    int           colour_index;
#endif
#endif
    Colour        *colours;
    int           n_size, used_j;
#ifdef  DEF_WIREFRAME
    int           used_i;
    int           i;
    Point         *ptr;
#else
    Point         *left_points, *right_points;
#endif
#ifdef  DEF_NORMALS
#ifdef DEF_WIREFRAME
    Vector        *normals;
#else
    Vector        *left_normals, *right_normals;
#endif
#endif
#if  !defined(DEF_ONE_COLOUR) && defined(DEF_NORMALS)
    Real               diffuse_coefficient =
                                     (Real) Surfprop_d( quadmesh->surfprop );
    Colour             scaled_colour;
#endif

    m = quadmesh->m;
    n = quadmesh->n;

    if( quadmesh->n_closed )
        n_size = n;
    else
        n_size = n - 1;

#ifdef  DEF_WIREFRAME
    n_objects = m + n;
    if( quadmesh->m_closed )
        m_size = m;
    else
        m_size = m - 1;
#else
    if( quadmesh->m_closed )
        n_objects = m;
    else
        n_objects = m - 1;
#endif

    colours = quadmesh->colours;

#ifdef  DEF_NORMALS
    set_surface_property( window, colours[0], &quadmesh->surfprop );
#endif

#ifdef  DEF_ONE_COLOUR
    SET_COLOUR( colours[0] );
#else
    GS_set_ambient_and_diffuse_mode( ON );
#endif

#ifdef  DEF_WIREFRAME
    BEGIN_DRAW_OBJECTS( window, 1, n_objects, TRUE )
#else
    BEGIN_DRAW_OBJECTS( window, 1, n_objects, FALSE )
#endif

#ifdef  DEF_WIREFRAME
        if( (int) OBJECT_INDEX < m )
        {
            item_index = (int) OBJECT_INDEX;
#ifdef  DEF_PER_ITEM_COLOURS
            colour_index = item_index;
            if( !quadmesh->m_closed && colour_index == m-1 )
                colour_index = m-2;
#endif

            GS_begin_line();

                ptr = &quadmesh->points[IJ(item_index,0,n)];
#ifdef DEF_NORMALS
                normals = &quadmesh->normals[IJ(item_index,0,n)];
#endif

                for_inclusive( j, 0, n_size )
                {
                    used_j = j % n;
#ifdef  DEF_PER_ITEM_COLOURS
                    SET_COLOUR( colours[IJ(colour_index,used_j,n_size)] );
#endif
#ifdef  DEF_PER_VERTEX_COLOURS
                    SET_COLOUR( colours[IJ(item_index,used_j,n)] );
#endif

#ifdef DEF_NORMALS
                    GS_set_normal( &normals[used_j] );
#endif

                    GS_set_point( &ptr[used_j] );
                }

            GS_end_line();
        }
        else
        {
            item_index = (int) OBJECT_INDEX - m;

#ifdef  DEF_PER_ITEM_COLOURS
            colour_index = item_index;
            if( !quadmesh->n_closed && colour_index == n-1 )
                colour_index = n-2;
#endif

            GS_begin_line();

                ptr = &quadmesh->points[IJ(0,item_index,n)];
#ifdef DEF_NORMALS
                normals = &quadmesh->normals[IJ(0,item_index,n)];
#endif

                for_inclusive( i, 0, m_size )
                {
                    used_i = i % m;
#ifdef  PER_ITEM_COLOURS
                    SET_COLOUR( colours[IJ(used_i,colour_index,n_size)] );
#endif
#ifdef  PER_VERTEX_COLOURS
                    SET_COLOUR( colours[IJ(used_i,item_index,n)] );
#endif
#ifdef DEF_NORMALS
                    GS_set_normal( &normals[used_i*n] );
#endif
                    GS_set_point( &ptr[used_i*n] );
                }

            GS_end_line();
        }
#else   /* DEF_WIREFRAME */

        item_index = (int) OBJECT_INDEX;

        GS_begin_quad_strip();

        left_points = &quadmesh->points[ IJ(item_index,0,n) ];
        right_points = &quadmesh->points[ IJ((item_index+1)%m,0,n) ];
#ifdef  DEF_NORMALS
        left_normals = &quadmesh->normals[ IJ(item_index,0,n) ];
        right_normals = &quadmesh->normals[ IJ((item_index+1)%m,0,n) ];
#endif

        for_inclusive( j, 0, n_size )
        {
            used_j = j % n;
#ifdef  DEF_PER_ITEM_COLOURS
            SET_COLOUR( colours[IJ(item_index,used_j,n_size)] );
#endif
#ifdef  DEF_PER_VERTEX_COLOURS
            SET_COLOUR( colours[IJ(item_index,used_j,n)] );
#endif
#ifdef  DEF_NORMALS
            GS_set_normal( &left_normals[used_j] );
#endif
            GS_set_point( &left_points[used_j] );

#ifdef  DEF_PER_VERTEX_COLOURS
            SET_COLOUR( colours[IJ((item_index+1)%m,used_j,n)] );
#endif
#ifdef  DEF_NORMALS
            GS_set_normal( &right_normals[used_j] );
#endif
            GS_set_point( &right_points[used_j] );
        }

        GS_end_quad_strip();

#endif  /* else DEF_WIREFRAME */

    END_DRAW_OBJECTS

#ifndef  DEF_ONE_COLOUR
    GS_set_ambient_and_diffuse_mode( OFF );
#endif
}

#undef  SET_COLOUR
