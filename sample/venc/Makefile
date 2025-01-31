CUR_PATH                  := $(shell pwd)
SRC_PATH                  := $(CUR_PATH)
HOME_PATH                 := $(abspath $(CUR_PATH)/../..)

include $(HOME_PATH)/build/config.mak

OUT_PATH                  := $(AXCL_OUT_PATH)
OBJ_OUT_PATH              := $(AXCL_PRJ_OUT_PATH)/objs
SRC_RELATIVE_PATH         := $(subst $(AXCL_HOME_PATH)/,,$(SRC_PATH))
TARGET_OUT_PATH           := $(OBJ_OUT_PATH)/$(SRC_RELATIVE_PATH)

# output
MOD_NAME                  := axcl_sample_venc
OUTPUT                    := $(TARGET_OUT_PATH)/.obj

# source
SRCS                      := $(wildcard $(SRC_PATH)/*.c) \
                             $(wildcard $(SRC_PATH)/common/*.c)

SRCCPPS                   := $(wildcard $(SRC_PATH)/*.cpp) \
                             $(wildcard $(AXCL_HOME_PATH)/sample/sys/sample_sys.cpp) \
                             $(wildcard $(AXCL_HOME_PATH)/toolkit/threadx.cpp) \
                             $(wildcard $(AXCL_HOME_PATH)/toolkit/event.cpp)

CINCLUDE                  := -I$(SRC_PATH) \
                             -I$(SRC_PATH)/common \
                             -I$(AXCL_INC_PATH) \
                             -I$(AXCL_HOME_PATH)/sample \
                             -I$(AXCL_HOME_PATH)/toolkit \
                             -I$(AXCL_HOME_PATH)/3rdparty/cmdline/include

OBJS                      := $(SRCS:%.c=$(OUTPUT)/%.o)
CPPOBJS                   := $(SRCCPPS:%.cpp=$(OUTPUT)/%.o)

DEPS                      := $(OBJS:%.o=%.d)
DEPS                      += $(shell awk '{gsub(/```/,"");print}' help.md | xxd -i > help.dat)
CPPDEPS                   := $(CPPOBJS:%.o=%.d)

# exec
TARGET                    := $(TARGET_OUT_PATH)/$(MOD_NAME)

# build flags
CFLAGS                    += $(CPPFLAGS)
CFLAGS                    += -DAXCL_BUILD_VERSION=\"$(SDK_VERSION)\"

CFLAGS                    += -Wall -Werror -O2 -ggdb3


# dependency
CLIB                      := -lstdc++ -pthread -lm
CLIB                      += -Wl,-rpath-link=$(AXCL_LIB_PATH)
CLIB                      += -L$(AXCL_LIB_PATH) -laxcl_rt -laxcl_sys -laxcl_venc

# install
INSTALL_TARGET            := $(TARGET)
INSTALL_DIR               := $(MOD_TARGET_PATH)/bin/

# link
LINK = $(CC)

include $(AXCL_BUILD_PATH)/rules.mak
