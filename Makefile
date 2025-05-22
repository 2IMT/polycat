POLYCAT_RELEASE ?= 0

CXX ?= g++
STRIP ?= strip

POLYCAT_VERSION = 2.0.0

CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -MMD -MP -DPOLYCAT_VERSION="\"$(POLYCAT_VERSION)\""

SRC_DIR := src
BUILD_DIR := build
BIN := polycat
DIST_DIR := dist
DIST := $(DIST_DIR)/$(BIN)-$(POLYCAT_VERSION)

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))
DEP_FILES := $(OBJ_FILES:.o=.d)

ifeq ($(POLYCAT_RELEASE),1)
	POST_BUILD := $(STRIP) $(BUILD_DIR)/$(BIN)
	CFLAGS += -O2
else
	CFLAGS += -g -O0
endif

all: $(BUILD_DIR)/$(BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/$(BIN): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o $@
	$(POST_BUILD)

-include $(DEP_FILES)

.PHONY: clean dist

clean:
	rm -rf $(BUILD_DIR)

dist:
	mkdir -p $(DIST)
	cp -r LICENSE README.md Makefile src res $(DIST)
	tar -cf $(DIST).tar $(DIST)
	gzip $(DIST).tar
	rm -rf $(DIST)
