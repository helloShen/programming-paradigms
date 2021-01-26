
CC = gcc
GFLAGS = -O0 -g #-O0 turn off optimization
				#-g generate source-level debug information

main:

assembly: mult2.c
	$(CC) $(GFLAGS) -S mult2.c	#-S only run preprocess and compilation steps


.PHONY: clean
clean: 
	-rm *.s

