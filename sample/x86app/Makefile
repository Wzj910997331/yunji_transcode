CUR_PATH                  := $(shell pwd)
SRC_PATH                  := $(CUR_PATH)
HOME_PATH                 := $(abspath $(CUR_PATH)/../..)

include $(HOME_PATH)/build/config.mak

OUT_PATH                  := $(AXCL_OUT_PATH)
OBJ_OUT_PATH              := $(AXCL_PRJ_OUT_PATH)/objs
SRC_RELATIVE_PATH         := $(subst $(AXCL_HOME_PATH)/,,$(SRC_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)
DEMO_BIN_PATH             := $(CUR_PATH)/bin

# output
MOD_NAME                  := axcl_demo
OUTPUT                    := $(TARGET_OUT_PATH)/.obj

# source
SRCS                      :=
SRCCPPS                   := $(wildcard $(SRC_PATH)/*.cpp)

CINCLUDE                  := -I$(SRC_PATH) \
                             -I$(AXCL_INC_PATH) \
                             -I$(AXCL_HOME_PATH)/3rdparty/cmdline/include

OBJS                      := $(SRCS:%.c=$(OUTPUT)/%.o)
CPPOBJS                   := $(SRCCPPS:%.cpp=$(OUTPUT)/%.o)

DEPS                      := $(OBJS:%.o=%.d)
CPPDEPS                   := $(CPPOBJS:%.o=%.d)

# exec
TARGET                    := $(TARGET_OUT_PATH)/$(MOD_NAME)

# build flags
CPPFLAGS                  := --std=c++17
CFLAGS                    += $(CPPFLAGS)
CFLAGS                    += -DAXCL_BUILD_VERSION=\"$(SDK_VERSION)\"

ifeq ($(debug),yes)
CFLAGS                    += -Wall -O0 -ggdb3
else
CFLAGS                    += -Wall -O2
endif

# dependency
CLIB                      := -lstdc++ -pthread
CLIB                      += -Wl,-rpath-link=$(AXCL_LIB_PATH)
CLIB                      += -L$(AXCL_LIB_PATH) -laxcl_comm -laxcl_rt -lspdlog

# install
INSTALL_TARGET            := $(TARGET)
INSTALL_TARGET            += $(DEMO_BIN_PATH)/*
INSTALL_DIR               := $(MOD_TARGET_PATH)/bin/axcl_demo

# link
LINK = $(CC)

include $(AXCL_BUILD_PATH)/rules.mak
