.PHONY : all
.PHONY : clean
.PHONY : cleanall
.PHONY : benchmark

OBJS = main.o mul_cpx2.o
CC = gcc

all : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o inlining

main.o : main.c
	$(CC) $(CFLAGS) -c $< -o $@

mul_cpx2.o : mul_cpx2.c
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f $(OBJS) inlining

cleanall : clean
	rm -f *~

benchmark : all
	@echo "Running ./inlining 3000 times: "
	@bash -c "time for i in {0..2999}; do ./inlining; done"



