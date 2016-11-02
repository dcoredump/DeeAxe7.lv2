#DEBUG=-DDEBUG=1 -DMSFADEBUG=1
#DEBUG=-DMSFADEBUG=1
CC=g++
BUNDLE = DeeAxe7.lv2
INSTALL_DIR = /home/pi/zynthian/zynthian-plugins/mod-lv2
TARGET=deeaxe7.so
OBJECTS=synth_unit.o ringbuffer.o patch.o \
	lfo.o dx7note.o freqlut.o sin.o exp2.o \
	fm_core.o pitchenv.o env.o fm_op_kernel.o
CFLAGS=-O3 -fPIC -DPIC $(DEBUG) -std=c++11 -I. -I./msfa -I/usr/local/include/lvtk-2
LDFLAGS=-shared -L/usr/local/lib -llvtk_plugin2 -llilv-0

all: $(BUNDLE) Makefile

clean:
	rm -f *.o *~ *.bak *.gch *.peg *.so
	rm -rf $(BUNDLE)

install: $(BUNDLE) Makefile
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
	rm -rf ${HOME}/.lv2/$(BUNDLE)
	cp -R $(BUNDLE) $(INSTALL_DIR)

deeaxe7.peg: DeeAxe7.ttl
	lv2peg DeeAxe7.ttl deeaxe7.peg

$(BUNDLE): manifest.ttl DeeAxe7.ttl deeaxe7.so
	rm -rf $(BUNDLE)
	mkdir $(BUNDLE)
	cp $^ $(BUNDLE)
	cp -R modgui $(BUNDLE)
#	mkdir $(BUNDLE)/presets
#	cd $(BUNDLE)/presets
#	../../../bin/deeaxe7sysex2lv2 ../../../patches/rom1A.syx
	
deeaxe7.so: $(OBJECTS) deeaxe7.o
	$(CC) $(OBJECTS) deeaxe7.o $(LDFLAGS) -o $@

deeaxe7.o: deeaxe7.cpp deeaxe7.peg deeaxe7.h
	$(CC) -c $(CFLAGS) deeaxe7.cpp

synth_unit.o: msfa/synth_unit.cc msfa/synth_unit.h
	$(CC) -c $(CFLAGS) msfa/synth_unit.cc

ringbuffer.o: msfa/ringbuffer.cc
	$(CC) -c $(CFLAGS) msfa/ringbuffer.cc

patch.o: msfa/patch.cc
	$(CC) -c $(CFLAGS) msfa/patch.cc

lfo.o: msfa/lfo.cc
	$(CC) -c $(CFLAGS) msfa/lfo.cc

dx7note.o: msfa/dx7note.cc
	$(CC) -c $(DEBUG) $(CFLAGS) msfa/dx7note.cc

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
