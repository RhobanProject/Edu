# Modules to be included
MODULES := main sockets rhoban/communication rhoban

# Program output
PROG := prog

CPP 	:= g++
LD 	:= g++
OBJCOPY := objcopy

IN_DIR	  := src
OUT_DIR	  := build
SRC_DIR   := $(addprefix $(IN_DIR)/,$(MODULES))
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
OBJ       := $(patsubst $(IN_DIR)/%.cpp,$(OUT_DIR)/%.o,$(SRC))
INCLUDES  := -I$(IN_DIR)/
CFLAGS 	  := -O2 -Wall
CLEANS	  :=
PREPARE	  :=

SUB_MAKEFILES := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/Makefile))

override CFLAGS += $(OPTIMIZE) $(DEFS)

vpath %.cpp $(SRC_DIR)

all: checkdirs prepare $(OUT_DIR)/$(PROG) copycommons

include $(SUB_MAKEFILES)

prepare: $(PREPARE)

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

clean: $(CLEANS)
	@rm -rf $(OUT_DIR)/*

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir),$(patsubst $(OUT_DIR)/%,$(IN_DIR)/%,$(bdir)))))
