CC = gcc
CFLAGS = -Wall -Wextra
OBJS = branching_method.o tabular_method.o libutils.o hashtable.o
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS) main.c
	$(CC) $(CFLAGS) $(OBJS) main.c -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJS) $(TARGET)