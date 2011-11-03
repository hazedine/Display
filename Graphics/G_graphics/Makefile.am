noinst_LIBRARIES = libdummy.a
libdummy_a_SOURCES = draw.c draw_objects.c event_loop.c \
	graphics_structs.c lights.c random_order.c render.c view.c windows.c

noinst_HEADERS = draw_polygons.include.c draw_quadmesh.include.c

INCLUDES = -I$(top_srcdir)/Graphics/OpenGL_graphics/Include \
	-I$(top_srcdir)/Graphics/Include \
	-I$(top_srcdir)/Graphics/GLUT_windows/Include \
	$(MNI_INCLUDES)
