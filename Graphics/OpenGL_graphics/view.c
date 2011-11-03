 
#include  <volume_io/internal_volume_io.h>
#include  <GS_graphics.h>

typedef  double  Matrix[16];

private  void  make_matrix(
    Transform  *trans,
    Matrix  gl_trans )
{
    int      i, j;

    for_less( i, 0, 4 )
    {
        for_less( j, 0, 4 )
            gl_trans[IJ(j,i,4)] = Transform_elem( *trans, i, j );
    }
}

public  void  GS_load_transform(
    Transform  *transform )
{
    Matrix  gl_trans;

    make_matrix( transform, gl_trans );

    glLoadMatrixd( gl_trans );
}

public  void  GS_mult_transform(
    Transform  *transform )
{
    Matrix  gl_trans;

    make_matrix( transform, gl_trans );

    glMultMatrixd( gl_trans );
}

public  void  GS_get_transform(
    Transform  *trans )
{
    int         i, j, type;
    Matrix   gl_trans;

    glGetIntegerv( GL_MATRIX_MODE, &type );

    if( type == GL_MODELVIEW )
        glGetDoublev( GL_MODELVIEW_MATRIX, gl_trans );
    else if( type == GL_PROJECTION )
        glGetDoublev( GL_PROJECTION_MATRIX, gl_trans );
    else
        handle_internal_error( "GS_get_transform" );

    for_less( i, 0, 4 )
    {
        for_less( j, 0, 4 )
            Transform_elem( *trans, i, j ) = gl_trans[IJ(j,i,4)];
    }
}

public  void  GS_ortho(
    Real   x_min,
    Real   x_max,
    Real   y_min,
    Real   y_max,
    Real   z_min,
    Real   z_max )
{
    glLoadIdentity();
    glOrtho( x_min, x_max, y_min, y_max, z_min, z_max );
}

public  void  GS_ortho_2d(
    int   x_min,
    int   x_max,
    int   y_min,
    int   y_max )
{

    glLoadIdentity();

#ifdef MESA_OPENGL

#ifdef NOT_NEEDED
{
    static  BOOLEAN  first = TRUE;
    static  Real     start, end;

    if( first )
    {
        first = FALSE;
        if( getenv( "START" ) == NULL ||
            sscanf( getenv("START"), "%lf", &start ) != 1 )
            start = 0.0;
        if( getenv( "END" ) == NULL ||
            sscanf( getenv("END"), "%lf", &end ) != 1 )
            end = 1.0;
    }

    glOrtho( (Real) x_min + start, (Real) x_max + end,
             (Real) y_min + start, (Real) y_max + end, -1.0, 1.0 );
}
#endif

    glOrtho( (Real) x_min, (Real) x_max + 1.0,
             (Real) y_min, (Real) y_max + 1.0, -1.0, 1.0 );
#else

#ifdef __alpha
    glOrtho( (Real) x_min - 0.5, (Real) x_max + 1.0,
             (Real) y_min - 0.5, (Real) y_max + 1.0, -1.0, 1.0 );
#else
    glOrtho( (Real) x_min, (Real) x_max + 1.0,
             (Real) y_min, (Real) y_max + 1.0, -1.0, 1.0 );
#endif

#endif
}

public  void  GS_frustum(
    Real   x_min,
    Real   x_max,
    Real   y_min,
    Real   y_max,
    Real   z_min,
    Real   z_max )
{
    glLoadIdentity();
    glFrustum( x_min, x_max, y_min, y_max, z_min, z_max );
}

/* ----------------------------- MNI Header -----------------------------------
@NAME       : initialize_window_view
@INPUT      : window
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Initializes the window view, called on creation of the window.
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : 1993            David MacDonald
@MODIFIED   : 
---------------------------------------------------------------------------- */

/* ARGSUSED */

public  void  GS_initialize_window_view(
    GSwindow   window )
{
}

public  void  GS_push_transform( void )
{
    glPushMatrix();
}

public  void  GS_pop_transform( void )
{
    glPopMatrix();
}

public  void  GS_set_viewport(
    int            x_min,
    int            x_max,
    int            y_min,
    int            y_max )
{
    glViewport( x_min, y_min, x_max - x_min + 1, y_max - y_min + 1 );
}

public  void  clear_overlay_planes( void )
{
#ifdef TO_DO
#endif
}

public  void  GS_set_matrix_mode(
    Matrix_modes   mode )
{
    int  gl_mode;

    switch( mode )
    {
    case VIEWING_MATRIX:
        gl_mode = GL_MODELVIEW;
        break;
    case PROJECTION_MATRIX:
        gl_mode = GL_PROJECTION;
        break;
    }

    glMatrixMode( (GLenum) gl_mode );
}
