noinst_LIBRARIES = libdummy.a
libdummy_a_SOURCES = colour_def.c draw.c event_loop.c lights.c render.c view.c windows.c

EXTRA_DIST = Include/GS_graphics.h Include/opengl_graphics_prototypes.h

INCLUDES = -I$(top_srcdir)/Graphics/OpenGL_graphics/Include \
	-I$(top_srcdir)/Graphics/Include \
	-I$(top_srcdir)/Graphics/GLUT_windows/Include \
	$(MNI_INCLUDES)
