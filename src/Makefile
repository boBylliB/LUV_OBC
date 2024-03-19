BIDIR = BeagleBone/headers
LIDIR = Laptop/headers
CC = gcc
BCFLAGS = -I $(BIDIR) -I .
LCFLAGS = -I $(LIDIR) -I .

BODIR = BeagleBone
LODIR = Laptop
LDIR = ../lib

LIBS = -lm

_BDEPS = 
BDEPS = $(patsubst %,$(BIDIR)/%,$(_BDEPS))
_LDEPS = RC_Handler.h Movement_Controller.h PID.h
LDEPS = $(patsubst %,$(LIDIR)/%,$(_LDEPS))

_BOBJ = LUV_BeagleBone.o
BOBJ = $(patsubst %,$(BODIR)/%,$(_BOBJ))
_LOBJ = LUV_Laptop.o RC_Handler.o Movement_Controller.o PID.o
LOBJ = $(patsubst %,$(LODIR)/%,$(_LOBJ))

$(BODIR)/%.o: BeagleBone/%.c $(BDEPS)
	$(CC) -c -o $@ $< $(BCFLAGS)
$(LODIR)/%.o: Laptop/%.c $(LDEPS)
	$(CC) -c -o $@ $< $(LCFLAGS)

luvbeaglebone: $(BOBJ)
	$(CC) -o $@ $^ $(BCFLAGS) $(LIBS)
luvlaptop: $(LOBJ)
	$(CC) -o $@ $^ $(LCFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(BODIR)/*.o *~ core $(INCDIR)/*~
	rm -f $(LODIR)/*.o *~ core $(INCDIR)/*~
