.PHONY : cell_distances
.PHONY : clean
.PHONY : cleanall
.PHONY : cleanallextra
.PHONY : tar

OBJS = main.o
CFLAGS = -Ofast -march=native -fopenmp
LIBS = -lm -lgomp

cell_distances : $(OBJS)
	$(CC) $(CFLAGS) -o cell_distances $(OBJS) $(LIBS)

main.o : main.c
	$(CC) $(CFLAGS) -c main.c

clean :
	rm -f $(OBJS) cell_distances*

cleanall : clean
	rm -f *~ *.h.gch *.tar *.tar.gz

cleanallextra : cleanall
	rm -f -r distances/ extracted/ reports/ gmon.out *.gcda *.gcno
tar :
	tar -cf hjohlman.tar Makefile main.c report.markdown
	gzip hjohlman.tar

tarMac :
	tar --disable-copyfile -cf hjohlman.tar Makefile main.c report.markdown
	gzip hjohlman.tar
