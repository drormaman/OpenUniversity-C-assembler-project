prog: assembler.o preassembler.o macro.o files.o
	gcc -g -ansi -pedantic -Wall assembler.o macro.o preassembler.o files.o -o prog
assembler.o: ./assembler/assembler.c ./assembler/assembler.h ./preassembler/preassembler.h
	gcc -c -ansi -pedantic -Wall ./assembler/assembler.c -o assembler.o
preassembler.o: ./preassembler/preassembler.c ./preassembler/preassembler.h ./macro/macro.h ./files/files.h
	gcc -c -ansi -pedantic -Wall ./preassembler/preassembler.c -o preassembler.o
macro.o: ./macro/macro.c ./macro/macro.h ./files/files.h
	gcc -c -ansi -pedantic -Wall ./macro/macro.c -o macro.o
files.o: ./files/files.c ./files/files.h
	gcc -c -ansi -pedantic -Wall ./files/files.c -o files.o