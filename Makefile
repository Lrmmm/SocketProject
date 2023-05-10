CXX = gcc
CXXFLAGS = -Wall -Wextra
SRCDIR = ./src
OBJDIR = ./output
BINDIR = ./output/bin
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
TARGET = $(BINDIR)/echo_server

$(TARGET) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: clean