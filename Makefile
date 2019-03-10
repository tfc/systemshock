.PHONY: all clean

include Config.mk

all: $(2D_OBJS) $(3D_OBJS) $(AFILE_OBJS) $(AFILE_CPP_OBJS) $(DSTRUCT_OBJS) $(FIX_OBJS) $(FIX_CPP_OBJS) $(INPUT_OBJS) $(LG_OBJS) $(PALETTE_OBJS) $(RES_OBJS) $(SND_OBJS) $(UI_OBJS) $(EDMS_OBJS) $(VOX_OBJS)
	echo All done.

%: %.c
	$(CC) -c $(CFLAGS) $(LIBS) -o $@ $<

%: %.cc
	$(CXX) -c $(CXXFLAGS) $(LIBS) -o $@ $<

clean:
	find src/ -type f -name '*.o' -delete -print
