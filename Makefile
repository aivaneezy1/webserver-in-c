# Compiler
CC = gcc
# Compiler flags
CFLAGS = -Wall -Werror
OUTPUT = main
# Source files
#file1.c file2.c
SRC = server.c init_server.c client_accept.c client_connect.c req_parser.c server.h

# Rule to build the executale
$(OUTPUT): $(SRC)
	$(CC) -o $(OUTPUT) $(SRC)

clean:
	rm -f $(OUTPUT)
