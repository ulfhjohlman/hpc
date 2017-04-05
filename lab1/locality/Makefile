.PHONY : all
.PHONY : clean
.PHONY : cleanall
.PHONY : benchmark

OBJS = main.o

all : $(OBJS)
	gcc $(CFLAGS) main.o -o locality

%.o : %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) locality

cleanall: clean
	rm -f *~

benchmark : cleanall all
	@echo "Running ./locality 300 times: "
	@bash -c "time for i in {0..299}; do ./locality; done"
