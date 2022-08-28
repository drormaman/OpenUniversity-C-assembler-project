prog: assembler.o preassembler.o macro.o files.o strings.o encode.o symbols.o methods.o output.o
	gcc -g -ansi -pedantic -Wall assembler.o macro.o preassembler.o files.o strings.o encode.o symbols.o methods.o output.o -o prog
assembler.o: ./assembler/assembler.c ./assembler/assembler.h ./preassembler/preassembler.h ./encode/encode.h ./symbols/symbols.h ./methods/methods.h ./output/output.h
	gcc -c -ansi -pedantic -Wall ./assembler/assembler.c -o assembler.o
preassembler.o: ./preassembler/preassembler.c ./preassembler/preassembler.h ./macro/macro.h ./files/files.h
	gcc -c -ansi -pedantic -Wall ./preassembler/preassembler.c -o preassembler.o
macro.o: ./macro/macro.c ./macro/macro.h ./files/files.h ./strings/strings.h
	gcc -c -ansi -pedantic -Wall ./macro/macro.c -o macro.o
files.o: ./files/files.c ./files/files.h ./strings/strings.h
	gcc -c -ansi -pedantic -Wall ./files/files.c -o files.o
strings.o: ./strings/strings.c ./strings/strings.h ./files/files.h
	gcc -c -ansi -pedantic -Wall ./strings/strings.c -o strings.o
encode.o: ./encode/encode.c ./encode/encode.h ./strings/strings.h ./files/files.h ./assembler/assembler.h ./symbols/symbols.h ./methods/methods.h
	gcc -c -ansi -pedantic -Wall ./encode/encode.c -o encode.o
symbols.o: ./symbols/symbols.c ./symbols/symbols.h ./assembler/assembler.h ./strings/strings.h
	gcc -c -ansi -pedantic -Wall ./symbols/symbols.c -o symbols.o
methods.o: ./methods/methods.c ./methods/methods.h ./strings/strings.h
	gcc -c -ansi -pedantic -Wall ./methods/methods.c -o methods.o
output.o: ./output/output.c ./output/output.h ./assembler/assembler.h ./strings/strings.h ./files/files.h ./symbols/symbols.h
	gcc -c -ansi -pedantic -Wall ./output/output.c -o output.o