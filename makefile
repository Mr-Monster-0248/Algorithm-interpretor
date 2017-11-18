src = sources/analyze.c sources/load.c sources/main.c sources/operations.c
compiler = gcc
output = ExeTest
valgrind = valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all

run:
	${compiler} ${src} -o ${output}

Wall:
	${compiler} ${src} -o ${output} -Wall

debug: Wall
	${valgrind} ./ExeTest
