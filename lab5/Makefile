.PHONY : dijkstra
.PHONY : clean
.PHONY : cleanall
.PHONY : tar

CC = mpicc
OBJS = main.o
CFLAGS = -O2 -march=native 
LIBS = 

dijkstra : $(OBJS)
	$(CC) $(CFLAGS) -o dijkstra $(OBJS) $(LIBS)

main.o : main.c
	$(CC) $(CFLAGS) -c main.c

clean :
	rm -f $(OBJS) dijkstra*

cleanall : clean
	rm -f *~ *.h.gch *.tar *.tar.gz
	rm -f gmon.out *.gcda *.gcno

tar :
	tar -cf hjohlman.tar Makefile main.c report.markdown
	gzip hjohlman.tar

tarMac :
	tar --disable-copyfile -cf hjohlman.tar Makefile main.c report.markdown
	gzip hjohlman.tar
