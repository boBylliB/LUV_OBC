IDIR = headers
BIDIR = BeagleBone/headers
LIDIR = Laptop/headers
CC = gcc
CFLAGS = -I $(IDIR)
BCFLAGS = -I $(BIDIR) -I $(IDIR)
LCFLAGS = -I $(LIDIR) -I $(IDIR)

ODIR = ./
BODIR = BeagleBone
LODIR = Laptop
LDIR = ../lib

LIBS = -lm

_BDEPS = PWMControl.h sbus.h SimplifiedMovement.h
BDEPS = $(patsubst %,$(BIDIR)/%,$(_BDEPS))
_LDEPS = RC_Handler.h Movement_Controller.h
LDEPS = $(patsubst %,$(LIDIR)/%,$(_LDEPS))

_OBJ = PID.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
_BOBJ = LUV_BeagleBone.o PWMControl.o SimplifiedMovement.o
BOBJ = $(patsubst %,$(BODIR)/%,$(_BOBJ)) $(OBJ)
_LOBJ = LUV_Laptop.o RC_Handler.o Movement_Controller.o
LOBJ = $(patsubst %,$(LODIR)/%,$(_LOBJ)) $(OBJ)

$(BODIR)/%.o: BeagleBone/%.c $(BDEPS)
	$(CC) -c -o $@ $< $(BCFLAGS)
$(LODIR)/%.o: Laptop/%.c $(LDEPS)
	$(CC) -c -o $@ $< $(LCFLAGS)
$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

luvbeaglebone: $(BOBJ)
	$(CC) -o $@ $^ $(BCFLAGS) $(LIBS)
luvlaptop: $(LOBJ)
	$(CC) -o $@ $^ $(LCFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(BODIR)/*.o *~ core $(INCDIR)/*~
	rm -f $(LODIR)/*.o *~ core $(INCDIR)/*~
