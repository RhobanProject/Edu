# Modules to be included
MODULES := main util dynamixel motor move

PROG := robot

CC := gcc
LD := gcc
OBJCOPY := objcopy

SRC_DIR   := $(addprefix src/,$(MODULES))
BUILD_DIR := $(addprefix build/,$(MODULES))

DEFS	  := -DF_CPU=$(F_CPU)
LIBS	  := -lpthread -lm

SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c))
OBJ       := $(patsubst src/%.c,build/%.o,$(SRC))
INCLUDES  := -Isrc/

override CFLAGS  := -g -Wall $(OPTIMIZE) $(DEFS) -O2

vpath %.c $(SRC_DIR)

all: checkdirs build/$(PROG)

build/$(PROG): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

checkdirs: $(BUILD_DIR)

define make-goal
$1/%.o: %.c
	$(CC) $(CFLAGS) $(DEFS) $(INCLUDES) -c $$< -o $$@
endef

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf build/*

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))
