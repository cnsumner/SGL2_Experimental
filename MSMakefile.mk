COMPILER_FLAGS = \
	/Od \
	/Zi	\
	/EHsc \

SOURCES = \
	test.cpp \

LIBRARIES =	\
	SDL2main.lib \
	SDL2.lib \

LINKER_FLAGS = \
	/OUT:build/test.exe \

test.exe:
	cl.exe $(COMPILER_FLAGS) $(SOURCES) $(LIBRARIES) /link $(LINKER_FLAGS) /SUBSYSTEM:CONSOLE \
	/LIBPATH:C:\SDL2-2.0.4\lib\x64