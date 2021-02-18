# C Compiler
CC = gcc
# C Compiler flags
CFLAGS = -std=gnu11 -Wall -Wextra -Wpedantic -Wshadow -g3 -fsanitize=address
#Target executable name 
TARGET = my_ls
#Target executable name 
DIRLIST = dirList

all: $(TARGET)

$(TARGET): $(TARGET).o $(DIRLIST).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o $(DIRLIST).o

$(DIRLIST).o: $(DIRLIST).c $(DIRLIST).h
	gcc $(CFLAGS) -c $(DIRLIST).c

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c


clean:
	rm *.o $(TARGET)

spotless:
	clean rm $(TARGET).exe
