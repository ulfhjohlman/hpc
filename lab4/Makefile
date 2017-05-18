.PHONY : heat_diffusion
.PHONY : clean
.PHONY : cleanall
.PHONY : tar

OBJS = main.o
CFLAGS = -O2 -march=native -I/usr/local/cuda-7.5/targets/x86_64-linux/include
LIBS = -lOpenCL -L/usr/local/cuda-7.5/targets/x86_64-linux/lib -lm

heat_diffusion : $(OBJS)
	$(CC) $(CFLAGS) -o heat_diffusion $(OBJS) $(LIBS)

main.o : main.c
	$(CC) $(CFLAGS) -c main.c

clean :
	rm -f $(OBJS) heat_diffusion*

cleanall : clean
	rm -f -r extracted/ reports/ gmon.out *.gcda *.gcno *~ *.h.gch *.tar *.tar.gz

tar :
	tar -cf hjohlman.tar Makefile main.c report.markdown
	gzip hjohlman.tar

tarMac :
	tar --disable-copyfile -cf hjohlman.tar Makefile main.c report.markdown
	gzip hjohlman.tar
