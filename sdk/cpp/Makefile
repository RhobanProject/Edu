# Modules to be included
MODULES := main sockets

# Program output
PROG := prog

CPP 	:= g++
LD 	:= g++
OBJCOPY := objcopy

OUT_DIR	  := build
SRC_DIR   := $(addprefix src/,$(MODULES))
BUILD_DIR := $(addprefix $(OUT_DIR)/,$(MODULES))
COMMON_DIR := ../common

DEFS	  :=
LIBS	  := -lpthread -lm
LIBS	  := -lpthread -lm
OS	  := linux

ifeq ($(OS), windows)
    LIBS += -lws2_32
endif

SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ       := $(patsubst src/%.cpp,$(OUT_DIR)/%.o,$(SRC))
INCLUDES  := -Isrc/
CFLAGS 	  := -O2 -Wall

SUB_MAKEFILES := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/Makefile))

include $(SUB_MAKEFILES)

override CFLAGS += $(OPTIMIZE) $(DEFS)

vpath %.cpp $(SRC_DIR)

all: checkdirs $(OUT_DIR)/$(PROG) copycommons

$(OUT_DIR)/$(PROG): $(OBJ)
	$(CPP) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

checkdirs: $(BUILD_DIR)

copycommons:
	cp -R $(COMMON_DIR)/* $(OUT_DIR)/

define make-goal
$1/%.o: $2/%.cpp
	$(CPP) $(CFLAGS) $(DEFS) $(INCLUDES) -c $$< -o $$@
endef

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(OUT_DIR)/*

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir),$(patsubst $(OUT_DIR)/%,src/%,$(bdir)))))
