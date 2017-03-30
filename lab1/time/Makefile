.PHONY : all
.PHONY : clean
.PHONY : cleanall
.PHONY : time

OBJS = main.o
OPTFLAGS = O0 O1 O2 O3 Os Og

all : $(OBJS)
	$(CC) $(CFLAGS) main.o -o sum

time : CFLAGS = 
time : main.o
	for flag in $(OPTFLAGS); do \
	$(CC) -$$flag -c main.c -o main.o ;\
	$(CC) -$$flag main.o -o sum ;\
	if [ $$flag = O0 ] ; \
	then \
		echo "Running optimization level $$flag, averaging over 3 runs" ;\
		time for i in {0..2}; do ./sum; done \
	else\
		echo "Running optimization level $$flag, averaging over 1000 runs" ;\
		time for i in {0..999}; do ./sum; done \
	fi;\
	done

	

%.o : %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
	rm -f $(OBJS) sum

cleanall : clean
	rm -f *~
