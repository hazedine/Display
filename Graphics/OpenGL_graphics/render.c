 
#include  <volume_io/internal_volume_io.h>
#include  <GS_graphics.h>

public  void  GS_set_shade_model(
    Shading_types  type )
{
    if( type == GOURAUD_SHADING )
        glShadeModel( GL_SMOOTH );
    else
        glShadeModel( GL_FLAT );
}

public  void  GS_turn_off_blend_function( void )
{
    glDisable( GL_BLEND );
    glBlendFunc( GL_ONE, GL_ZERO );
}

public  void  GS_turn_on_blend_function( void )
{
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_BLEND );
}

/* ARGSUSED */

public  void  GS_set_lighting_state(
    GSwindow     window,
    BOOLEAN      state )
{
    if( state )
        glEnable( GL_LIGHTING );
    else
        glDisable( GL_LIGHTING );
}

public  void  GS_backface_culling_state(
    BOOLEAN     state )
{
    if( state )
    {
        glEnable( GL_CULL_FACE );
        glCullFace( GL_BACK );
    }
    else
        glDisable( GL_CULL_FACE );
}

/* ARGSUSED */

public  void  GS_set_n_curve_segments(
    int      n_segments )
{
#ifdef  TO_DO
    curveprecision( n_segments );
#endif
}
