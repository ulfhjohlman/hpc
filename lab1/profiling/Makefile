.PHONY : gprof
.PHONY : clean
.PHONY : gcov

FLAGS =  -fprofile-arcs -ftest-coverage

gprof : ./main.c
	gcc -pg -c ./main.c -o gprofFile.o
	gcc -pg gprofFile.o -o gprofFile 
	-./gprofFile
	gprof gprofFile gmon.out > profile.txt

gcov : ./main.c
	gcc $(FLAGS) main.c -o gcovOut
	-./gcovOut
	gcov main.c

clean : 
	rm -f gprofFile gprofFile.o gmon.out gcovOut

cleanall : clean
	rm -f profile.txt main.c.gcov *~
