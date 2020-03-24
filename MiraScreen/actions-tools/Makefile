BINS = actions-tools

all: $(BINS)

actions-tools: actions-tools.o z.o checksum.o firm.o
actions-tools: CPPFLAGS+=-g
actions-tools: LDLIBS=-lz

clean:
	rm -vfr $(BINS) *.o

