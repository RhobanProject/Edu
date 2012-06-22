# Modules to be included
MODULES := main util sockets motion joystick sendJoystick

PROG := prog

CPP := g++
LD := g++
OBJCOPY := objcopy

SRC_DIR   := $(addprefix src/,$(MODULES))
BUILD_DIR := $(addprefix build/,$(MODULES))

DEFS	  :=
LIBS	  := -lpthread -lm

SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ       := $(patsubst src/%.cpp,build/%.o,$(SRC))
INCLUDES  := -Isrc/
CFLAGS 	  := -O2 -Wall

SUB_MAKEFILES := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/Makefile))

include $(SUB_MAKEFILES)

override CFLAGS += $(OPTIMIZE) $(DEFS)

vpath %.cpp $(SRC_DIR)

all: checkdirs build/$(PROG)

build/$(PROG): $(OBJ)
	$(CPP) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

checkdirs: $(BUILD_DIR)

define make-goal
$1/%.o: $2/%.cpp
	$(CPP) $(CFLAGS) $(DEFS) $(INCLUDES) -c $$< -o $$@
endef

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf build/*

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir),$(patsubst build/%,src/%,$(bdir)))))
