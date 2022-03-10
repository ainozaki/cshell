TARGET = tursh
CFLAGS = -I./include/ -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common
LDFLAGS = 

SRCS = 
SRCS_OBJ = $(SRCS:.c=.o) 
HEADERS = $(SRCS:.c=.h)
MAIN = tursh.c

$(TARGET): $(SRCS_OBJ)
	gcc -o $@ $+

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

format:
	clang-format -i $(MAIN)

clean:
	rm -f *.o $(TARGET) $(SRCS_OBJ)

.PHONY: format clean

