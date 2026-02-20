#vars
TARGET := wfc
DEPENDENCIES := 
 
EXTENSION := cpp
HEADEREXT := h
CC := g++

INCLUDEDIR := include
OBJDIR := obj
SRCDIR := src
BINDIR := bin

SRCFILES := $(shell find $(SRCDIR) -type f \( -iname "*.$(EXTENSION)" \) -exec basename \{} \;)
HEADERFILES := $(shell find $(INCLUDEDIR) -type f \( -iname "*.$(HEADEREXT)" \) -exec basename \{} \;)
OBJFILES := $(SRCFILES:%.$(EXTENSION)=%.o)

MACROS :=
BASEFLAGS := $(addprefix -D ,$(MACROS)) --std=c++17 -Wall
DEBUGFLAGS := $(BASEFLAGS) -g
RELEASEFLAGS := $(BASEFLAGS) -O2

#accumulators
FLAGS :=

#builds
standard: FLAGS = $(BASEFLAGS)
standard: clean setup $(TARGET)

debug: FLAGS = $(DEBUGFLAGS)
debug: clean setup $(TARGET)

release: FLAGS = $(RELEASEFLAGS)
release: clean setup $(TARGET)

#build target
$(TARGET): $(OBJFILES)
	$(CC) $(FLAGS) $(addprefix $(OBJDIR)/,$^) -o $(BINDIR)/$@ $(DEPENDENCIES)


#compile object files
%.o: $(SRCDIR)/%.$(EXTENSION)
	$(CC) $(FLAGS) -o $(addprefix $(OBJDIR)/,$@) -c $^ -I $(INCLUDEDIR)
%.o: $(SRCDIR)/**/%.$(EXTENSION)
	$(CC) $(FLAGS) -o $(addprefix $(OBJDIR)/,$@) -c $^ -I $(INCLUDEDIR)


#clean directory
clean:
	@rm -rf $(OBJDIR)
	@rm -rf $(BINDIR)


#setup directory
setup: 
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)



#print makefile info
info:
		@echo TARGET = $(TARGET)
		@echo EXTENSION = $(EXTENSION)
		@echo INCLUDEDIR = $(INCLUDEDIR)
		@echo OBJDIR = $(OBJDIR)
		@echo SRCDIR = $(SRCDIR)
		@echo BINDIR = $(BINDIR)
		@echo SRCFILES = $(SRCFILES)
		@echo HEADERFILES = $(HEADERFILES)
		@echo MACROS = $(MACROS)
		@echo BASEFLAGS = $(BASEFLAGS)
		@echo DEBUGFLAGS = $(DEBUGFLAGS)
		@echo RELEASEFLAGS = $(RELEASEFLAGS)
		@echo CC = $(CC)

