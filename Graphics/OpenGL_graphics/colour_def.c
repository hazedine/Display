#include  <volume_io/internal_volume_io.h>

public  Colour  make_rgba_Colour(
    int    r,
    int    g,
    int    b,
    int    a )
{
    Colour          c;
    unsigned  char  *byte_ptr;

    c = 0;         /* to avoid used-before-set compiler messages */

    byte_ptr = (unsigned char *) &c;

    byte_ptr[3] = (unsigned char) a;
    byte_ptr[2] = (unsigned char) b;
    byte_ptr[1] = (unsigned char) g;
    byte_ptr[0] = (unsigned char) r;

    return( c );
}

public  int  get_Colour_r(
    Colour   colour )
{
    unsigned  char  *b;

    b = (unsigned char *) &colour;

    return( (int) b[0] );
}

public  int  get_Colour_g(
    Colour   colour )
{
    unsigned  char  *b;

    b = (unsigned char *) &colour;

    return( (int) b[1] );
}

public  int  get_Colour_b(
    Colour   colour )
{
    unsigned  char  *b;

    b = (unsigned char *) &colour;

    return( (int) b[2] );
}

public  int  get_Colour_a(
    Colour   colour )
{
    unsigned  char  *b;

    b = (unsigned char *) &colour;

    return( (int) b[3] );
}
