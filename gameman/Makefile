COMP = g++
LINK = ld
TP_DIR = thirdparty
RUNTIME_DIR = runtime

MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MKFILE_DIR := $(dir $(MKFILE_PATH))

CPARAM = -std=gnu++17
IPARAM = -I/$(MKFILE_DIR)/runtime \
					-I/$(MKFILE_DIR)/config \
					-I/$(MKFILE_DIR)/thirdparty \
					-I/home/nata/Libraries
LPARAM = -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lvulkan

TP_SRC_FILES := $(shell find $(TP_DIR) -name *.cpp)
ENG_SRC_FILES := $(shell find $(RUNTIME_DIR) -name *.cpp)
ENG_OBJ_FILES := $(patsubst Engine/%.cpp, Bin/Engine/%.o, $(ENG_SRC_FILES))

OUT_FILE_NAME := bin/engine/x64-linux/engine.sh

$(OUT_FILE_NAME): $(ENG_SRC_FILES) $(TP_SRC_FILES)
	mkdir -p $(dir $@) && \
	$(COMP) $(CPARAM) $(IPARAM) $^ -o $@ $(LPARAM)