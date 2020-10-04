IDIR=../include
CC=g++
CFLAGS=-Wall -Werror -I$(IDIR)

ODIR=/home/kirtivr/opengl/src/objs
LDIR=../lib

LIBS=-lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl

_DEPS=$(IDIR)/glad/src/glad.c
DEPS= $(patsubst %,$(IDIR)/%,$(_DEPS))

$(ODIR)/glad.o: $(IDIR)/glad/src/glad.c
	$(CC) $(CFLAGS) -c -fpic $< -o $@

$(ODIR)/libglad.so: $(ODIR)/glad.o
	$(CC) -shared -o $@ $<

triangle: $(ODIR)/triangle.o
	$(CC) $@.cpp $(DEPS) -o $^ $(CFLAGS) $(LIBS)

redtriangle: $(ODIR)/redtriangle.o
	$(CC) $@.cpp $(DEPS) -o $^ $(CFLAGS) $(LIBS)

rectangle: $(ODIR)/rectangle.o
	$(CC) $@.cpp $(DEPS) -o $^ $(CFLAGS) $(LIBS)

uniform: $(ODIR)/uniform.o
	$(CC) $@.cpp $(DEPS) -o $^ $(CFLAGS) $(LIBS)

$(ODIR)/shader.o: shader.cpp $(ODIR)/libglad.so
	$(CC) $(CFLAGS) -c -fpic $< -L$(ODIR) -Wl,-rpath=$(ODIR) -lglad $(LIBS) -o $(ODIR)/shader.o

$(ODIR)/libshader.so: $(ODIR)/shader.o
	$(CC) -shared -o $@ $<

more_attributes: more_attributes.cpp $(ODIR)/libshader.so $(ODIR)/libglad.so
	$(CC) $@.cpp -o $(ODIR)/$@.o $(CFLAGS) $(LIBS) -L$(ODIR) -Wl,-rpath=$(ODIR) -lglad -lshader 

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.so *~ core $(LDIR)/*~ fragment_shaders/*~ vertex_shaders/*~
