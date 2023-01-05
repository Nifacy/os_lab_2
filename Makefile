all : compiled/main compiled/worker

compiled/main : compiled/ofiles/io_line_lib.o compiled/ofiles/pipe_lib.o compiled/ofiles/manager.o
	gcc compiled/ofiles/io_line_lib.o compiled/ofiles/pipe_lib.o compiled/ofiles/manager.o -o compiled/main

compiled/worker : compiled/ofiles/io_line_lib.o compiled/ofiles/remove_vowels_lib.o compiled/ofiles/worker.o
	gcc compiled/ofiles/io_line_lib.o compiled/ofiles/remove_vowels_lib.o compiled/ofiles/worker.o -o compiled/worker

compiled/ofiles/io_line_lib.o : src/io_line/io_line.c
	gcc -c src/io_line/io_line.c -o compiled/ofiles/io_line_lib.o

compiled/ofiles/pipe_lib.o : src/pipe/pipe.c
	gcc -c src/pipe/pipe.c -o compiled/ofiles/pipe_lib.o

compiled/ofiles/remove_vowels_lib.o : src/remove_vowels/remove_vowels.c
	gcc -c src/remove_vowels/remove_vowels.c -o compiled/ofiles/remove_vowels_lib.o

compiled/ofiles/manager.o : src/manager.c
	gcc -c src/manager.c -o compiled/ofiles/manager.o

compiled/ofiles/worker.o : src/worker.c
	gcc -c src/worker.c -o compiled/ofiles/worker.o
