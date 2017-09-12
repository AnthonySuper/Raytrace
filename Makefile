CXX := g++
OBJDIR := ./obj
SRCDIR := ./src
TESTDIR := ./test_srcs
CXXFLAGS := -Iinclude -std=c++14 -Wall -Ofast -ffast-math -march=native -funroll-loops -flto
LDFLAGS := -pthread
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TESTSRCS := $(wildcard $(TESTDIR)/*.cpp)
TESTOBJS := $(TESTSRCS:$(TESTDIR)/%.cpp=$(OBJDIR)/%.o)
TESTTARGET := catch_test
TARGET := raytrace

$(TARGET): main.cpp $(OBJECTS)
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@


.PHONY: clean docs

all: $(OBJDIR) $(TARGET) $(TESTTARGET)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TESTTARGET): $(TESTOBJS) $(OBJECTS) test.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -Itest_srcs $^ -o $@

$(TESTOBJS): $(OBJDIR)/%.o : $(TESTDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -Itest_srcs -c $< -o $@

clean:
	rm -rf obj
	rm -f $(TARGET)
	rm -f $(TESTTARGET)

docs:
	Doxygen ./Doxyfile
