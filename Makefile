CC=gcc
CSTD=c99
CFLAGS=-Wall -pedantic
input=daemon

all:
	${CC} -std=${CSTD} ${CFLAGS} ${input}.c -o${input}
