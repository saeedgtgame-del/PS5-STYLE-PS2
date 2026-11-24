EE_BIN = PS5_STYLE_MENU.ELF
EE_OBJS = src/main.o
EE_LIBS = -lgskit -ldma -lpacket -lpad -lgraph -ldraw -lmath3d -ldebug -lm
EE_INCS += -I$(PS2SDK)/ee/include
EE_INCS += -I$(PS2SDK)/common/include

all: $(EE_BIN)

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
