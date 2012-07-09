# Modules to be included
MODULES := main sockets rhoban/communication sockets/demo rhoban/test tests \
    rhoban/communication/tests

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

SRC	  :=
OBJ	  :=
INCLUDES  := -I$(IN_DIR)/
CFLAGS 	  := -O2 -Wall
CLEANS	  :=
PREPARE	  :=

fetch-sources = $(eval include $(IN_DIR)/$1/Makefile.sources); \
    $(eval SRC += $(addprefix $(IN_DIR)/$1/,$(SOURCES))); \
    $(eval OBJ += $(subst .cpp,.o,$(addprefix $(OUT_DIR)/$1/,$(SOURCES)))) \
    $(foreach src,$(SOURCES),$(call make-goal,$(IN_DIR)/$1/$(src),$(subst .cpp,.o,$(addprefix $(OUT_DIR)/$1/,$(src)))))

$(foreach module,$(MODULES),$(call fetch-sources,$(module)))

SUB_MAKEFILES := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/Makefile))

override CFLAGS += $(OPTIMIZE) $(DEFS)

vpath %.cpp $(SRC_DIR)

all: prepare checkdirs $(OUT_DIR)/$(PROG) copycommons

include $(SUB_MAKEFILES)

prepare: $(PREPARE)

$(OUT_DIR)/$(PROG): $(OBJ)
	$(CPP) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

checkdirs: $(BUILD_DIR)

copycommons:
	cp -R $(COMMON_DIR)/* $(OUT_DIR)/

$(BUILD_DIR):
	@mkdir -p $@

clean: $(CLEANS)
	@rm -rf $(OUT_DIR)/*

define make-goal
$2: $1
	$(CPP) $(CFLAGS) $(DEFS) $(INCLUDES) -c $1 -o $2
endef

make-goals = $(eval include $(IN_DIR)/$1/Makefile.sources); \
    $(foreach src,$(SOURCES),$(eval $(call make-goal,$(IN_DIR)/$1/$(src),$(subst .cpp,.o,$(addprefix $(OUT_DIR)/$1/,$(src))))))

$(foreach module,$(MODULES),$(call make-goals,$(module)))
