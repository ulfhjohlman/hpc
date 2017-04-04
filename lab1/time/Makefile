.PHONY : all
.PHONY : clean
.PHONY : cleanall
.PHONY : time

OBJS = main.o
OPTFLAGS = O0 O1 O2 O3 Os Og

all : $(OBJS)
	$(CC) $(CFLAGS) main.o -o sum

time : main.o
	@$(CC) -O0 -c main.c -o main.o ;
	@$(CC) -O0 main.o -o sum ;
	@echo "Running optimization level O0, 3 runs" ;
	@bash -c "time for i in {0..2}; do ./sum; done"
	@$(CC) -O1 -c main.c -o main.o ;
	@$(CC) -O1 main.o -o sum ;
	@echo "Running optimization level O1, 1000 runs" ;
	@bash -c "time for i in {0..999}; do ./sum; done" ; 
	@$(CC) -O2 -c main.c -o main.o ;
	@$(CC) -O2 main.o -o sum ;
	@echo "Running optimization level O2, 1000 runs" ;
	@bash -c "time for i in {0..999}; do ./sum; done" ; 
	@$(CC) -O3 -c main.c -o main.o ;
	@$(CC) -O3 main.o -o sum ;
	@echo "Running optimization level O3, 1000 runs" ;
	@bash -c "time for i in {0..999}; do ./sum; done" ; 
	@$(CC) -Os -c main.c -o main.o ;
	@$(CC) -Os main.o -o sum ;
	@echo "Running optimization level Os, 1000 runs" ;
	@bash -c "time for i in {0..999}; do ./sum; done" ; 
	@$(CC) -Og -c main.c -o main.o ;
	@$(CC) -Og main.o -o sum ;
	@echo "Running optimization level Og, 1000 runs" ;
	@bash -c "time for i in {0..999}; do ./sum; done" ; 
	

%.o : %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
	rm -f $(OBJS) sum

cleanall : clean
	rm -f *~
