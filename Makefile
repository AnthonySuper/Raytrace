CXX := g++
OBJDIR := ./obj
SRCDIR := ./src
TESTDIR := ./test
CXXFLAGS := -Iinclude -std=c++14 -O3 -ffast-math -march=native -funroll-loops -flto
LDFLAGS := -pthread
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TESTSRCS := $(wildcard $(TESTDIR)/*.cpp)
TESTOBJS := $(TESTSRCS:$(TESTDIR)/%.cpp=$(OBJDIR)/%.o)
TESTTARGET := catch_test
TARGET := raytracer

$(TARGET): main.cpp $(OBJECTS)
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@


.PHONY: clean docs

all: $(OBJDIR) $(TARGET) $(TESTTARGET)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TESTTARGET): $(TESTOBJS) $(OBJECTS) test.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -Ilib $^ -o $@

$(TESTOBJS): $(OBJDIR)/%.o : $(TESTDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -Ilib -c $< -o $@

clean:
	rm -rf obj
	rm -f $(TARGET)
	rm -f $(TESTTARGET)

docs:
	Doxygen ./Doxyfile
