#PROG=simple_sgl.cpp
#PROG=splinecurve.cpp
#PROG=randomwalk.cpp
#PROG=drawstring.cpp
#PROG=fontdesigner.cpp
#PROG=towershanoi.cpp

CC=g++
CFLAGS=-Wall -std=c++14
LFLAGS=-mconsole -mwindows -lmingw32 -lSDL2main -lSDL2 -lwinmm -lole32 \
       -loleaut32 -limm32 -lversion

#a.exe: ${PROG} sgl2_window.hpp
a.exe: ${PROG} sgl2.hpp sgl2.h
	${CC} ${CFLAGS} ${PROG} ${LFLAGS}

clean:
	del /q *.o *.exe *~

run: a.exe
	.\a.exe

edit:
	cvim ${PROG}
