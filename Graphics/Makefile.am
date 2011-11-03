SUBDIRS = GLUT_windows G_graphics OpenGL_graphics

EXTRA_DIST = Include/GS_graphics.h Include/graphics.h \
	Include/graphics_base.h Include/graphics_prototypes.h \
	Include/graphics_struct.h Include/random_order.h \
	Include/save.h

noinst_LIBRARIES = libbicgl.a

# Adding a dummy source file to register is a work-around for a bug in 
# automake, which does not (in version 1.4) allow linking together
# only objects in subdirectories.

libbicgl_a_SOURCES = dummy.c

dummy.c:
	touch $@

CLEANFILES = dummy.c


libbicgl_a_LIBADD = $(OPENGL_X_OBJECTS)



PRINT_X_COLOURS = print_X_colours



GLX_SRC = GLX_windows/glx_windows.c \
          GLX_windows/stored_font.c

GLUT_SRC = GLUT_windows/glut_windows.c \
           GLUT_windows/copy_x_colours.c

X_SRC = X_windows/x_windows.c

GL_SRC = \
         IrisGL_graphics/colour_def.c \
         IrisGL_graphics/draw.c \
         IrisGL_graphics/event_loop.c \
         IrisGL_graphics/lights.c \
         IrisGL_graphics/render.c \
         IrisGL_graphics/view.c \
         IrisGL_graphics/windows.c

OPENGL_SRC = \
             OpenGL_graphics/colour_def.c \
             OpenGL_graphics/draw.c \
             OpenGL_graphics/event_loop.c \
             OpenGL_graphics/lights.c \
             OpenGL_graphics/render.c \
             OpenGL_graphics/view.c \
             OpenGL_graphics/windows.c

G_SRC = G_graphics/draw.c \
        G_graphics/draw_objects.c \
        G_graphics/event_loop.c \
        G_graphics/graphics_structs.c \
        G_graphics/lights.c \
        G_graphics/random_order.c \
        G_graphics/render.c \
        G_graphics/view.c \
        G_graphics/windows.c


#OPENGL_X_SRC = $(G_SRC) $(OPENGL_SRC) $(GLX_SRC) $(X_SRC)
OPENGL_X_SRC = $(G_SRC) $(OPENGL_SRC) $(GLUT_SRC)

OPENGL_X_OBJECTS = $(OPENGL_X_SRC:.c=.o)


#-----

MAKE_PROTOTYPES = echo make_prototypes

PROTOTYPE_FILE = Include/graphics_prototypes.h

$(PROTOTYPE_FILE): $(G_SRC)
	@$(MAKE_PROTOTYPES) $@ $(G_SRC)

#-----

X_PROTOTYPE_FILE = X_windows/Include/x_window_prototypes.h

$(X_PROTOTYPE_FILE): $(X_SRC)
	@$(MAKE_PROTOTYPES) $@ $(X_SRC)

#-----

GLX_PROTOTYPE_FILE = GLX_windows/Include/glx_window_prototypes.h

$(GLX_PROTOTYPE_FILE): $(GLX_SRC)
	@$(MAKE_PROTOTYPES) $@ $(GLX_SRC)

#-----

GLUT_PROTOTYPE_FILE = GLUT_windows/Include/glut_window_prototypes.h

$(GLUT_PROTOTYPE_FILE): $(GLUT_SRC)
	@$(MAKE_PROTOTYPES) $@ $(GLUT_SRC)

#-----

OPENGL_PROTOTYPE_FILE = OpenGL_graphics/Include/opengl_graphics_prototypes.h

$(OPENGL_PROTOTYPE_FILE): $(OPENGL_SRC)
	@$(MAKE_PROTOTYPES) $@ $(OPENGL_SRC)

#-----

GL_PROTOTYPE_FILE = IrisGL_graphics/Include/irisgl_graphics_prototypes.h

$(GL_PROTOTYPE_FILE): $(GL_SRC)
	@$(MAKE_PROTOTYPES) $@ $(GL_SRC)

