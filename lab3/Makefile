.PHONY : all
.PHONY : clean
.PHONY : cleanall

OBJS = main.o
CFLAGS = -O3 -march=native #-fopenmp
LIBS = #-lgomp

all : $(OBJS)
	$(CC) $(CFLAGS) -o cell_distances $(OBJS) $(LIBS)

main.o : main.c
	$(CC) $(CFLAGS) -c main.c

clean :
	rm -f $(OBJS) cell_distances*

cleanall : clean
	rm -f *~ *.h.gch
