TARGET = tursh
CFLAGS = -I./include -Wall
LDFLAGS = 

MAIN = tursh.c
SRCS = exec.c
SRCS_OBJ = $(SRCS:.c=.o) $(MAIN:.c=.o) 
HEADERS = include/exec.h

$(TARGET): $(SRCS_OBJ)
	gcc -o $@ $+

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

run:
	./$(TARGET)
format:
	clang-format -i $(MAIN) $(SRCS) $(HEADERS)

clean:
	rm -f *.o $(TARGET) $(SRCS_OBJ)

.PHONY: format clean

