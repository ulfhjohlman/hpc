.PHONY : all
.PHONY : clean
.PHONY : cleanall
.PHONY : check

OBJS = main.o
CC = gcc

all : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o leak

main.o : main.c
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f $(OBJS) leak

cleanall : clean
	rm -f *~

check : all
	valgrind ./leak
