all: main

model.o: model.cpp model.h rply/rply.h
	g++ $(CFLAGS) -c model.cpp

cube.o: cube.cpp cube.h
	g++ $(CFLAGS) -c cube.cpp
	
quaternion.o: quaternion.cpp quaternion.h
	g++ $(CFLAGS) -c quaternion.cpp

trackball.o: trackball.cpp trackball.h
	g++ $(CFLAGS) -c trackball.cpp

rply.o:	rply/rply.c rply/rply.h
	gcc $(CFLAGS) -c rply/rply.c
	
main.o: main.cpp
	g++ $(CFLAGS) -c main.cpp
	
main: main.o quaternion.o rply.o model.o cube.o trackball.o
	g++ $(CFLAGS) -o main main.o model.o cube.o quaternion.o rply.o trackball.o -lGL -lGLU -lglut -lm

clean:
	rm *.o main
