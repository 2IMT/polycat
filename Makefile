POLYCAT_RELEASE ?= 0

CXX ?= g++
STRIP ?= strip
PREFIX ?= /usr/local

POLYCAT_VERSION = 2.0.0

CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -MMD -MP \
	-DPOLYCAT_VERSION="\"$(POLYCAT_VERSION)\"" \
	-DPOLYCAT_PREFIX="\"$(PREFIX)\""

SRC_DIR := src
BUILD_DIR := build
DIST_DIR := dist
DIST_NAME := polycat-$(POLYCAT_VERSION)
DIST := $(DIST_DIR)/$(DIST_NAME)

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))
DEP_FILES := $(OBJ_FILES:.o=.d)

ifeq ($(POLYCAT_RELEASE),1)
	POST_BUILD := $(STRIP) $(BUILD_DIR)/polycat
	CFLAGS += -O2
else
	CFLAGS += -g -O0
endif

all: $(BUILD_DIR)/polycat

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/polycat: $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o $@
	$(POST_BUILD)

-include $(DEP_FILES)

.PHONY: clean dist install uninstall

clean:
	rm -rf $(BUILD_DIR)

dist:
	mkdir -p $(DIST)
	cp -r LICENSE README.md Makefile src res $(DIST)
	tar -cf $(DIST).tar -C $(DIST_DIR) $(DIST_NAME)
	gzip $(DIST).tar
	rm -rf $(DIST)

install: all
	install -Dm755 $(BUILD_DIR)/polycat $(DEST_DIR)$(PREFIX)/bin/polycat
	install -Dm644 LICENSE $(DEST_DIR)$(PREFIX)/share/licenses/polycat/LICENSE
	install -Dm644 res/polycat.ttf $(DEST_DIR)$(PREFIX)/share/fonts/TTF/polycat.ttf
	install -Dm644 res/polycat-config $(DEST_DIR)$(PREFIX)/share/polycat/polycat-config

uninstall:
	rm -f \
		$(DEST_DIR)$(PREFIX)/bin/polycat \
		$(DEST_DIR)$(PREFIX)/share/licenses/polycat/LICENSE \
		$(DEST_DIR)$(PREFIX)/share/fonts/TTF/polycat.ttf \
		$(DEST_DIR)$(PREFIX)/share/polycat/polycat-config
