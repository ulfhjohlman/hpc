.PHONY : all
.PHONY : clean
.PHONY : cleanall
.PHONY : benchmark

OBJS = main.o

all : $(OBJS)
	gcc $(CFLAGS) main.o -o indirect_addressing

%.o : %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) indirect_addressing

cleanall: clean
	rm -f *~

benchmark : cleanall all
	@echo "Running ./indirect_addressing 5000 times: "
	@bash -c "time for i in {0..4999}; do ./indirect_addressing; done"
