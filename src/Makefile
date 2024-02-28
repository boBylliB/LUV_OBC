BIDIR = BeagleBone/headers
LIDIR = Laptop/headers
CC = gcc
CFLAGS = -I$(IDIR)

BODIR = BeagleBone/obj
LODIR = Laptop/obj
LDIR = ../lib

LIBS = -lm

_BDEPS = 
BDEPS = $(patsubst %,$(BIDIR)/%,$(_BDEPS)) settings.h
_LDEPS = RC_Handler.h Movement_Controller.h PID.h
LDEPS = $(patsubst %,$(LIDIR)/%,$(_LDEPS)) settings.h

_BOBJ = LUV_BeagleBone.o
BOBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
_LOBJ = LUV_Laptop.o RC_Handler.o Movement_Controller.o PID.o
LOBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(BODIR)/%.o: %.c $(BDEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
$(LODIR)/%.o: %.c $(LDEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

luvbeaglebone: $(BOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
luvlaptop: $(LOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(BODIR)/*.o *~ core $(INCDIR)/*~
	rm -f $(LODIR)/*.o *~ core $(INCDIR)/*~