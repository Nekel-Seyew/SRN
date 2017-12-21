all: srn_random.o

srn_random.o:
	clang -O3 -Wall pt_random.c ../../asprintf.c/asprintf.c -c -pthread

debug:
	clang --debug -Wall pt_random.c ../../asprintf.c/asprintf.c -c -pthread

clean:
	rm -f *.o
	rm -f test
test: debug
	clang --debug -Wall pt_random.o test.c -o test -pthread
