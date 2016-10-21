#DEBUG=-DDEBUG=1
CC=g++
BUNDLE = dx7.lv2
INSTALL_DIR = /home/pi/zynthian/zynthian-plugins/mod-lv2
TARGET=dx7.so
OBJECTS=synth_unit.o ringbuffer.o patch.o \
	lfo.o dx7note.o freqlut.o sin.o exp2.o \
	fm_core.o pitchenv.o env.o fm_op_kernel.o
CFLAGS=-O2 -fPIC -DPIC $(DEBUG) -std=c++11 -I. -I./msfa -I/usr/local/include/lvtk-2
LDFLAGS=-shared -L/usr/local/lib -llvtk_plugin2 -llilv-0

all: $(BUNDLE)

clean:
	rm -f *.o *~ *.bak *.gch *.peg *.so dx7-test
	rm -rf $(BUNDLE)

install: $(BUNDLE)
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
	cp -R $(BUNDLE) $(INSTALL_DIR)

dx7.peg: dx7.ttl
	lv2peg dx7.ttl dx7.peg

$(BUNDLE): manifest.ttl dx7.ttl dx7.so
	rm -rf $(BUNDLE)
	mkdir $(BUNDLE)
	cp $^ $(BUNDLE)

dx7-test: $(OBJECTS) dx7-test.o dx7.peg
	$(CC) $(OBJECTS) -lm dx7-test.o -o $@

dx7-test.o: dx7-test.cpp dx7.h
	$(CC) -c $(CFLAGS) dx7-test.cpp

dx7.so: $(OBJECTS) dx7.o
	$(CC) $(OBJECTS) dx7.o $(LDFLAGS) -o $@

dx7.o: dx7.cpp dx7.peg dx7.h
	$(CC) -c $(CFLAGS) dx7.cpp

synth_unit.o: msfa/synth_unit.cc msfa/synth_unit.h
	$(CC) -c $(CFLAGS) msfa/synth_unit.cc

ringbuffer.o: msfa/ringbuffer.cc
	$(CC) -c $(CFLAGS) msfa/ringbuffer.cc

patch.o: msfa/patch.cc
	$(CC) -c $(CFLAGS) msfa/patch.cc

lfo.o: msfa/lfo.cc
	$(CC) -c $(CFLAGS) msfa/lfo.cc

dx7note.o: msfa/dx7note.cc
	$(CC) -c $(CFLAGS) msfa/dx7note.cc

freqlut.o: msfa/freqlut.cc
	$(CC) -c $(CFLAGS) msfa/freqlut.cc

sin.o: msfa/sin.cc
	$(CC) -c $(CFLAGS) msfa/sin.cc

exp2.o: msfa/exp2.cc
	$(CC) -c $(CFLAGS) msfa/exp2.cc

fm_core.o: msfa/fm_core.cc
	$(CC) -c $(CFLAGS) msfa/fm_core.cc

pitchenv.o: msfa/pitchenv.cc
	$(CC) -c $(CFLAGS) msfa/pitchenv.cc

env.o: msfa/env.cc
	$(CC) -c $(CFLAGS) msfa/env.cc

fm_op_kernel.o: msfa/fm_op_kernel.cc
	$(CC) -c $(CFLAGS) msfa/fm_op_kernel.cc
