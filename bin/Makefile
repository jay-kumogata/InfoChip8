CC = gcc
TARBALL = InfoChip8_01J

# InfoChip8
.CFILES =	../InfoChip8.cpp \
		../CPU.cpp \
		../PPU.cpp \
		../IO.cpp \
		../APU.cpp \
		./System_w32.cpp \
		./Sound_w32.cpp \

.OFILES	=	$(.CFILES:.cpp=.o)

CCFLAGS = -mno-cygwin 
LDFILGS = -mno-cygwin -mwindows -lstdc++ -lwinmm -lgdi32 -ldsound

all: InfoChip8

InfoChip8: $(.OFILES)
	$(CC) $(INCLUDES) -o $@ $(.OFILES) $(LDFILGS) -lm -lz

.cpp.o:
	$(CC) $(INCLUDES) -c $(CCFLAGS) $*.cpp  -o $@

clean:
	rm -f $(.OFILES) ./*~ ../*~ *.stackdump

cleanall:
	rm -f $(.OFILES) ./*~ ../*~ *.stackdump InfoChip8.exe

release: clean all

tar:
	( cd ..; \
	tar cvf $(TARBALL).tar ./*; \
	gzip $(TARBALL).tar \
	)

install:
	install ./InfoChip8 /usr/local/bin
