 
#include  <volume_io/internal_volume_io.h>
#include  <GS_graphics.h>

private  void  initialize_lights( void )
{
    static  float ambient_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambient_light );
    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
}


public  void  GS_initialize_lights(
    GSwindow        window )
{
    initialize_lights();

    window->ambient_set = 0;
    window->n_lights_defined = 0;
    window->n_light_states = 0;
}

public  void  delete_lights(
    GSwindow        window )
{
    if( window->n_lights_defined > 0 )
        FREE( window->lights );

    if( window->n_light_states > 0 )
    {
        FREE( window->light_indices );
        FREE( window->light_states );
    }
}

private  void  set_ambient_light(
    Colour         colour )
{
    float     ambient_light[4];

    ambient_light[0] = (float) get_Colour_r_0_1(colour);
    ambient_light[1] = (float) get_Colour_g_0_1(colour);
    ambient_light[2] = (float) get_Colour_b_0_1(colour);
    ambient_light[3] = (float) get_Colour_a_0_1(colour);

    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambient_light );
}

/* ARGSUSED */

public  void  GS_set_ambient_light(
    GSwindow       window,
    Colour         colour )
{
    set_ambient_light( colour );
}

private  void  define_light(
    int             light_index,
    Light_types     type,
    Colour          colour,
    Vector          *direction,
    Point           *position,
    Real            spot_exponent,
    Real            spot_angle )
{
    int     gl_light_index;
    Vector  unit_direction;
    float   colour_list[4];
    float   position_list[4];
    static  float  null_colour[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    colour_list[0] = (float) get_Colour_r_0_1(colour);
    colour_list[1] = (float) get_Colour_g_0_1(colour);
    colour_list[2] = (float) get_Colour_b_0_1(colour);
    colour_list[3] = (float) get_Colour_a_0_1(colour);

    gl_light_index = GL_LIGHT0 + light_index;

    switch( type )
    {
    case DIRECTIONAL_LIGHT:
        glLightfv( (GLenum) gl_light_index, GL_AMBIENT, null_colour );
        glLightfv( (GLenum) gl_light_index, GL_DIFFUSE, colour_list );
        glLightfv( (GLenum) gl_light_index, GL_SPECULAR, colour_list );
        NORMALIZE_VECTOR( unit_direction, *direction );
        position_list[0] = -Vector_x(unit_direction);
        position_list[1] = -Vector_y(unit_direction);
        position_list[2] = -Vector_z(unit_direction);
        position_list[3] = 0.0f;
        glLightfv( (GLenum) gl_light_index, GL_POSITION, position_list );
        break;

    case POINT_LIGHT:
        glLightfv( (GLenum) gl_light_index, GL_AMBIENT, null_colour );
        glLightfv( (GLenum) gl_light_index, GL_DIFFUSE, colour_list );
        glLightfv( (GLenum) gl_light_index, GL_SPECULAR, colour_list );
        position_list[0] = Point_x(*position);
        position_list[1] = Point_y(*position);
        position_list[2] = Point_z(*position);
        position_list[3] = 0.0f;
        glLightfv( (GLenum) gl_light_index, GL_POSITION, position_list );
        break;

    case SPOT_LIGHT:
        glLightfv( (GLenum) gl_light_index, GL_AMBIENT, null_colour );
        glLightfv( (GLenum) gl_light_index, GL_DIFFUSE, colour_list );
        glLightfv( (GLenum) gl_light_index, GL_SPECULAR, colour_list );

        position_list[0] = Point_x(*position);
        position_list[1] = Point_y(*position);
        position_list[2] = Point_z(*position);
        position_list[3] = 1.0f;
        glLightfv( (GLenum) gl_light_index, GL_POSITION, position_list );

        NORMALIZE_VECTOR( unit_direction, *direction );
        position_list[0] = Vector_x(unit_direction);
        position_list[1] = Vector_y(unit_direction);
        position_list[2] = Vector_z(unit_direction);
        position_list[3] = 0.0f;
        glLightfv( (GLenum) gl_light_index, GL_SPOT_DIRECTION, position_list );

        glLightf( (GLenum) gl_light_index, GL_SPOT_EXPONENT,
                  (GLfloat) spot_exponent );

        glLightf( (GLenum) gl_light_index, GL_SPOT_CUTOFF,
                  (GLfloat) spot_angle );
        break;
    }
}

/* ARGSUSED */

public  void  GS_define_light(
    GSwindow        window,
    int             light_index,
    Light_types     type,
    Colour          colour,
    Vector          *direction,
    Point           *position,
    Real            spot_exponent,
    Real            spot_angle )
{
    int    ind;

    define_light( light_index, type, colour, direction, position,
                  spot_exponent, spot_angle );

    for_less( ind, 0, window->n_lights_defined )
    {
        if( window->lights[ind].light_index == light_index )
            break;
    }

    if( ind >= window->n_lights_defined )
    {
        SET_ARRAY_SIZE( window->lights, window->n_lights_defined,
                        window->n_lights_defined+1, 1 );
        ++window->n_lights_defined;
    }

    window->lights[ind].light_index = light_index;
    window->lights[ind].type = type;
    window->lights[ind].colour = colour;
    if( direction != NULL )
        window->lights[ind].direction = *direction;
    else
        fill_Vector( window->lights[ind].direction, 0.0, 0.0, 0.0 );

    if( position != NULL )
        window->lights[ind].position = *position;
    else
        fill_Vector( window->lights[ind].position, 0.0, 0.0, 0.0 );

    window->lights[ind].spot_exponent = spot_exponent;
    window->lights[ind].spot_angle = spot_angle;
}

private  void  set_light_state(
    int             light_index,
    BOOLEAN         state )
{
    short   gl_light_index;

    switch( light_index )
    {
    case 0:  gl_light_index = GL_LIGHT0;    break;
    case 1:  gl_light_index = GL_LIGHT1;    break;
    case 2:  gl_light_index = GL_LIGHT2;    break;
    case 3:  gl_light_index = GL_LIGHT3;    break;
    case 4:  gl_light_index = GL_LIGHT4;    break;
    case 5:  gl_light_index = GL_LIGHT5;    break;
    case 6:  gl_light_index = GL_LIGHT6;    break;
    case 7:  gl_light_index = GL_LIGHT7;    break;
    }

    if( state )
        glEnable( (GLenum) gl_light_index );
    else
        glDisable( (GLenum) gl_light_index );
}

/* ARGSUSED */

public  void  GS_set_light_state(
    GSwindow        window,
    int             light_index,
    BOOLEAN         state )
{
    int        ind;

    set_light_state( light_index, state );

    for_less( ind, 0, window->n_light_states )
    {
        if( window->light_indices[ind] == light_index )
            break;
    }

    if( ind >= window->n_light_states )
    {
        SET_ARRAY_SIZE( window->light_indices, window->n_light_states,
                        window->n_light_states+1, 1 );
        SET_ARRAY_SIZE( window->light_states, window->n_light_states,
                        window->n_light_states+1, 1 );
        ++window->n_light_states;

        window->light_indices[ind] = light_index;
    }

    window->light_states[ind] = state;
}

public  void  redefine_lights(
    GSwindow        window )
{
    int                ind;
    light_info_struct  *l;
    Transform          identity;

    GS_push_transform();

    make_identity_transform( &identity );
    GS_load_transform( &identity );

    initialize_lights();

    for_less( ind, 0, window->n_lights_defined )
    {
        l = &window->lights[ind];

        define_light( l->light_index, l->type, l->colour,
                      &l->direction, &l->position,
                      l->spot_exponent, l->spot_angle );
    }

    for_less( ind, 0, window->n_light_states )
    {
        set_light_state( window->light_indices[ind],
                         window->light_states[ind] );
    }

    GS_pop_transform();
}
