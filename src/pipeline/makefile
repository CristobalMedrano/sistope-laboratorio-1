CC = gcc -g
FLAGS = -Wall -g3
EXTERNAL_LIBRARIES = -lm -ljpeg

TARGETS=read grayscale convolution binarize classify write

all: $(TARGETS)

$(TARGETS):
	$(CC) $(FLAGS) $@.c -o $@.out $(EXTERNAL_LIBRARIES)