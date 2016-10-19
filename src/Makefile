TARGET=dx7.so
SRC=dx7.cc \
	msfa/synth_unit.cc msfa/ringbuffer.cc msfa/patch.cc \
	msfa/lfo.cc msfa/dx7note.cc msfa/freqlut.cc msfa/sin.cc msfa/exp2.cc \
	msfa/fm_core.cc msfa/pitchenv.cc msfa/env.cc msfa/fm_op_kernel.cc

CFLAGS=-fPIC -DPIC -I./msfa
LDFLAGS=-O2 -shared

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(JSFLAGS) -o $@ $(SRC)
