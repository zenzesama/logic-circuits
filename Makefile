CC = gcc
CFLAGS = -Wall -Wextra
TARGET = circuit
SOURCES = main.c logic.c gates.c flipFlops.c print.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)
