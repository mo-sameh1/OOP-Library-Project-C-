all: clean compile link start

compile:
	g++ -c code.cpp  -I"src/include" -lpq

link:
	g++ code.o -o code -L"src/lib" -lpq

start:
	./code.exe

clean:
	rm -f code *.o