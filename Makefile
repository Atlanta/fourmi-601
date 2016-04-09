# Makefile Generique

EXT = c
CXX = gcc
EXEC = fourmi

CXXFLAGS = -Werror –ansi –pedantic –Wall
LDFLAGS =

BINDIR = bin
SRCDIR = src
OBJDIR = obj
SRC = $(wildcard $(SRCDIR)/*.$(EXT))
OBJ = $(SRC:.$(EXT)=.o)
OBJ := $(addprefix $(OBJDIR)/, $(OBJ))

all: $(EXEC)

$(EXEC): $(OBJ)
	@$(CXX) -o $@ $^ $(LDFLAGS)
	@mv $(EXEC) $(BINDIR)

$(OBJDIR)/%.o: %.$(EXT)
	@$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	@rm -rf $(OBJDIR)/*.o
	@rm -f $(EXEC)

install: $(EXEC)
	@cp $(EXEC) /usr/bin/
