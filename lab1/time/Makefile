.PHONY : all
.PHONY : clean
.PHONY : cleanall
.PHONY : time
.PHONY : assembler

OBJS = main.o
OPTFLAGS = O0 O1 O2 O3 Os Og
ASSEMBLER = sumO0.s sumO1.s sumO2.s sumO3.s sumOs.s sumOg.s

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
	
assembler : main.o
	$(CC) -O0 -S main.c -o sumO0.s
	$(CC) -O1 -S main.c -o sumO1.s
	$(CC) -O2 -S main.c -o sumO2.s
	$(CC) -O3 -S main.c -o sumO3.s
	$(CC) -Os -S main.c -o sumOs.s
	$(CC) -Og -S main.c -o sumOg.s

%.o : %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
	rm -f $(OBJS) sum $(ASSEMBLER)

cleanall : clean
	rm -f *~
