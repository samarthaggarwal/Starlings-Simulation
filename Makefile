CXX=g++
LFLAGS       = -std=c++11
CXXFLAGS     = -std=c++11 -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH1     = -I.
LIBS         = -lGL -lGLU -lglut
DEFINES      = -DQT_NO_DEBUG -DQT_GUI_LIB
TARGET       = starlings

# directories
SRCDIR   = src
OBJDIR   = build
BINDIR   = bin


all : $(BINDIR)/starlings

$(BINDIR)/starlings:  $(OBJDIR)/starlings.o
	$(CXX) $(LFLAGS) -o $@ $^ $(LIBS)

$(OBJDIR)/starlings.o: $(SRCDIR)/starlings.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@ $(INCPATH)


.PHONY: clean

clean: 
	rm -f *.o $(TARGET)
	rm -r $(OBJDIR)/*
