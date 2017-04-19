.PHONY : all
.PHONY : allandrun
.PHONY : clean
.PHONY : cleanall

OBJS = main.o makePPM.o doIteration.o
CFLAGS = -O3
LIBS = -lpthread -std=c99

all : $(OBJS)
	$(CC) $(CFLAGS) -o newton $(OBJS) $(LIBS)

main.o : main.c makePPM.h doIteration.h
	$(CC) $(CFLAGS) -c main.c

makePPM.o : makePPM.c makePPM.h
	$(CC) $(CFLAGS) -c makePPM.c

doIteration.o :  doIteration.c  doIteration.h
	$(CC) $(CFLAGS) -c  doIteration.c

allandrun : all
	./newton -t1 -l1000 5


allandtime : all
	time ./newton -t1 -l1000 5

clean :
	rm -f $(OBJS) newton* *.h.gch

cleanall : clean
	rm -f *~
