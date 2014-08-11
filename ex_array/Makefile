PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64

CFLAGS = -std=c++11 -g -O0 -Wall -Werror
SPEEDCHECK = -std=c++11 -O3 -Wall -Werror -DEXARRAYDEBUG=1 
GPROF = -std=c++11 -g -O3   -DINITDEBUG=0 -Wall -Werror -pg
UTILITY = fheap.o cakemap.o cake02.o 
ALL = cake02.cpp fheap.cpp cakemap.cpp 
STUFF = cake02.cpp cake02.h
DEBUG = -DINITDEBUG=2 -DLOGGER=1 -DASSERTHAPPY=1 -DEXARRAYDEBUG=1
EXARRAY = -DEXARRAYDEBUG=1

extest: ex_tester.cpp ex_array.h ex_array_impl.h
	g++ $(CFLAGS) $(EXARRAY) ex_tester.cpp -o ex_test

fast:  ex_tester.cpp ex_array.h ex_array_impl.h
	g++ $(SPEEDCHECK)  ex_tester.cpp -o ex_test


spch: cake02.cpp fheap.cpp cakemap.cpp
	g++ $(SPEEDCHECK) $(ALL) -o ckf 



submit: fheap.o cakemap.o cake02.o 
	g++ $(SPEEDCHECK) $(ALL) -o cakequest

prof: cake02.cpp fheap.cpp cakemap.cpp
	g++ $(GPROF) $(ALL) -o caker


cake02.o: fheap.o cake02.cpp cake02.h 
	g++ -c $(CFLAGS) $(DEBUG) $(STUFF)  


gen: crazygen.cpp ../../Utile/utile.h
	g++ $(SPEED) crazygen.cpp -o crazygen

cakemap.o: cakemap.cpp cakemap.h 
	g++ -c $(CFLAGS)  $(DEBUG) cakemap.cpp cakemap.h 

fheap.o: fheap.cpp
	g++ -c $(CFLAGS) $(DEBUG)  fheap.cpp

clean:
	rm *.o ex_tester cakefast cakesub cakelog.log


