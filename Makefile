#
# TODO: Move `libmongoclient.a` to /usr/local/lib so this can work on production servers
#
 
CC := gcc # This is the main compiler
ASM := nasm
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
TARGET := bin/runner
 
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g
ASMFLAGS := -g -f elf64
LIB=`pkg-config --libs allegro-5 allegro_image-5 allegro_dialog-5`
#INC=-I/usr/include/allegro5



$(TARGET): $(OBJECTS)
	@mkdir -p bin
	$(CC) $(CFLAGS) build/filter_image.o build/glowny.o -o $(TARGET) $(LIB)
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo ; $(CC) $(CFLAGS) $(INC) $(LIB) -c -o $@ $<
	$(ASM) $(ASMFLAGS) -o build/filter_image.o src/filter_image.s

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET) $(TEST_TARGET)

.PHONY: clean
