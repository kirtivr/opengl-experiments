IDIR=../include
CC=g++
CFLAGS=-I$(IDIR)

ODIR=objs
LDIR=../lib

LIBS=-lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl

_DEPS=glad/src/glad.c
DEPS= $(patsubst %,$(IDIR)/%,$(_DEPS))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) $< $(DEPS) -o $@ $(CFLAGS) $(LIBS)

triangle: $(ODIR)/triangle.o
	$(CC) $@.cpp $(DEPS) -o $^ $(CFLAGS) $(LIBS)

redtriangle: $(ODIR)/redtriangle.o
	$(CC) $@.cpp $(DEPS) -o $^ $(CFLAGS) $(LIBS)

rectangle: $(ODIR)/rectangle.o
	$(CC) $@.cpp $(DEPS) -o $^ $(CFLAGS) $(LIBS)

uniform: $(ODIR)/uniform.o
	$(CC) $@.cpp $(DEPS) -o $^ $(CFLAGS) $(LIBS)

more_attributes: $(ODIR)/more_attributes.o
	$(CC) $@.cpp $(DEPS) -o $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(LDIR)/*~
