prog: assembler.o preassembler.o macro.o files.o strings.o firstphase.o symbols.o methods.o
	gcc -g -ansi -pedantic -Wall assembler.o macro.o preassembler.o files.o strings.o firstphase.o symbols.o methods.o -o prog
assembler.o: ./assembler/assembler.c ./assembler/assembler.h ./preassembler/preassembler.h ./firstphase/firstphase.h ./symbols/symbols.h ./methods/methods.h
	gcc -c -ansi -pedantic -Wall ./assembler/assembler.c -o assembler.o
preassembler.o: ./preassembler/preassembler.c ./preassembler/preassembler.h ./macro/macro.h ./files/files.h
	gcc -c -ansi -pedantic -Wall ./preassembler/preassembler.c -o preassembler.o
macro.o: ./macro/macro.c ./macro/macro.h ./files/files.h
	gcc -c -ansi -pedantic -Wall ./macro/macro.c -o macro.o
files.o: ./files/files.c ./files/files.h
	gcc -c -ansi -pedantic -Wall ./files/files.c -o files.o
strings.o: ./strings/strings.c ./strings/strings.h ./files/files.h
	gcc -c -ansi -pedantic -Wall ./strings/strings.c -o strings.o
firstphase.o: ./firstphase/firstphase.c ./firstphase/firstphase.h ./strings/strings.h ./files/files.h ./assembler/assembler.h ./symbols/symbols.h ./methods/methods.h
	gcc -c -ansi -pedantic -Wall ./firstphase/firstphase.c -o firstphase.o
symbols.o: ./symbols/symbols.c ./symbols/symbols.h ./assembler/assembler.h ./strings/strings.h
	gcc -c -ansi -pedantic -Wall ./symbols/symbols.c -o symbols.o
methods.o: ./methods/methods.c ./methods/methods.h ./strings/strings.h
	gcc -c -ansi -pedantic -Wall ./methods/methods.c -o methods.o