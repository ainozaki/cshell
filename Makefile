TARGET = tursh
CFLAGS = -I./include -Wall
LDFLAGS = 

MAIN = tursh.c
SRCS = argv.c \
			 builtin.c \
			 exec.c \
			 input.c \
			 redirect.c \
			 signal_handle.c \
			 utils.c

SRCS_OBJ = $(SRCS:.c=.o) $(MAIN:.c=.o) 
HEADERS = include/argv.h \
					include/builtin.h \
					include/exec.h \
					include/input.h \
					include/redirect.h \
					include/signal_handle.h \
					include/utils.h

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

