.PHONY : all
.PHONY : allandrun
.PHONY : clean
.PHONY : cleanall
.PHONY : newton

OBJS = main.o makePPM.o doIteration.o
CFLAGS = -Ofast -march=native
LIBS = -pthread -lpthread -lm

all : $(OBJS)
	$(CC) $(CFLAGS) -o newton $(OBJS) $(LIBS)

newton : $(OBJS)
		$(CC) $(CFLAGS) -o newton $(OBJS) $(LIBS)

main.o : main.c makePPM.h doIteration.h
	$(CC) $(CFLAGS) -c main.c $(LIBS)

makePPM.o : makePPM.c makePPM.h
	$(CC) $(CFLAGS) -c makePPM.c $(LIBS)

doIteration.o :  doIteration.c  doIteration.h
	$(CC) $(CFLAGS) -c doIteration.c $(LIBS)

allandrun : all
	./newton -t1 -l1000 5


allandtime : all
	time ./newton -t1 -l100 3

clean :
	rm -f $(OBJS) newton* *.h.gch

cleanall : clean
	rm -f *~
