CC = g++
CFLAGS = -g -std=c++17
RM = rm -f

APP = 	mutable-string evaluate print-digits print-binary print-decimal \
		dice sublists 8queens travel linked-list inheritance template

.PHONY: all clean

all: 

mutable-string: mutable-string.cpp
	$(CC) $(CFLAGS) -o mutable-string mutable-string.cpp

evaluate: evaluate.cpp
	$(CC) $(CFLAGS) -o evaluate evaluate.cpp

print-digits: print-digits.cpp
	$(CC) $(CFLAGS) -o print-digits print-digits.cpp

print-binary: print-binary.cpp
	$(CC) $(CFLAGS) -o print-binary print-binary.cpp

print-decimal: print-decimal.cpp
	$(CC) $(CFLAGS) -o print-decimal print-decimal.cpp

permute: permute.cpp
	$(CC) $(CFLAGS) -o permute permute.cpp

dice: dice.cpp
	$(CC) $(CFLAGS) -o dice dice.cpp

sublists: sublists.cpp
	$(CC) $(CFLAGS) -o sublists sublists.cpp

8queens: 8queens.cpp
	$(CC) $(CFLAGS) -o 8queens 8queens.cpp

travel: travel.cpp
	$(CC) $(CFLAGS) -o travel travel.cpp

linked-list: linked-list.cpp
	$(CC) $(CFLAGS) -o linked-list linked-list.cpp

inheritance: employee.h employee.cpp lawyer.h lawyer.cpp
	$(CC) $(CFLAGS) -o inheritance employee.cpp lawyer.cpp

template: template.cpp
	$(CC) $(CFLAGS) -o template template.cpp

clean:
	$(RM) $(APP) 