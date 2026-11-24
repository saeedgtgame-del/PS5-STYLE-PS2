EE_BIN = PS5_STYLE_MENU.ELF
EE_OBJS = src/main.o

GSKIT ?= $(PS2DEV)/gsKit
EE_INCS += -I$(PS2SDK)/ee/include
EE_INCS += -I$(PS2SDK)/common/include
EE_INCS += -I$(GSKIT)/include
EE_LDFLAGS += -L$(GSKIT)/lib
EE_LIBS = -lgskit -ldmakit -lpad -lgraph -ldraw -lmath3d -lpacket -ldebug -lm

all: $(EE_BIN)

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
