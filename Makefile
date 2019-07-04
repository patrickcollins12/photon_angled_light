#  g++ src/ColorC.cpp test/color_test_unit.cpp -o color_test_unit -I src -std=c++11
#  && ./color_test_unit && rm ./color_test_unit 
CXX      := -g++
CXXFLAGS := -std=c++11
# -pedantic-errors -Wall -Wextra -Werror
#LDFLAGS  := -L/usr/lib -lstdc++ -lm

BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TEST_INCS:= -I/opt/local/include
TEST_LIBS:= -L/opt/local/lib -lgtest
TARGET   := color_test_unit
TEST_TARGET := $(APP_DIR)/test_exe
TEST_ARGS:=$(TEST_INCS) $(TEST_LIBS) 

INCLUDE  := -Isrc/
SRC      :=                      \
   $(wildcard src/*.cpp)         \

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

# $(info $$SRC is [${SRC}])
# $(info $$OBJECTS is [${OBJECTS}])

# test/color_gtest.cpp: $(TEST_TARGET)

test: $(OBJECTS) test/color_gtest.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(TEST_ARGS) -o $(TEST_TARGET) $(OBJECTS) test/color_gtest.cpp
	@./$(TEST_TARGET)
# @rm $(TEST_TARGET)


# color_test_catch: 
# 	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -o $(APP_DIR)/$(TARGET) $(OBJECTS)
#     @g++ $(CPP_FLAGS) $(INCLUDES) -o run_test $(OBJS) $(TEST_SRC)
#     @./run_test

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -o $(APP_DIR)/$(TARGET) $(OBJECTS)

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*
