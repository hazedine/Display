#include  <volume_io/internal_volume_io.h>
#include  <X11/Xlib.h>
#include  <stdio.h>

/* This function relied upon an undocumented feature of GLUT - the
 * ability to reference the X display directly through the global
 * variable "__glutDisplay".  New implementations of GLUT
 * (esp. FreeGlut) do not implement this.  It appears to be mostly
 * harmless to simply "stub out" this function.
 */
#ifndef USE_COPY_X_COLOURS

public void copy_X_colours(int n_colours_to_copy)
{
    /* Do nothing */
}

#else

extern  void  glut_set_colour_entry(
    int     ind,
    Real    r,
    Real    g,
    Real    b );

/* ----------------------------------------------------

This extern is borrowed from an undocumented feature of GLUT, that is, the
availability of the X display, if compiled to use X windows.

------------------------------------------------------- */

extern    Display            *__glutDisplay;

public  void  copy_X_colours(
    int  n_colours_to_copy )
{
    int                screen;
    Window             root_window;
    XWindowAttributes  attrib;
    Colormap           cmap;
    XColor             def;
    int                ind;
    Real               r, g, b;

    /*--- for some reason on SGI/OpenGL, this MIN is needed */

#ifdef __sgi
    n_colours_to_copy = MIN( n_colours_to_copy, 256 );
#endif

    screen = DefaultScreen( __glutDisplay );

    root_window =  RootWindow( __glutDisplay, screen ),

    XGetWindowAttributes( __glutDisplay, root_window, &attrib );

    cmap = attrib.colormap;

    for( ind = 0;  ind < n_colours_to_copy;  ++ind )
    {
        def.pixel = (unsigned long) ind;
        XQueryColor( __glutDisplay, cmap, &def );

        r = (Real) def.red / (Real) 65535.0;
        g = (Real) def.green / (Real) 65535.0;
        b = (Real) def.blue / (Real) 65535.0;

        glut_set_colour_entry( ind, r, g, b );
    }
}
#endif 


