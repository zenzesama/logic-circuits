CC = gcc
CFLAGS = -Wall -Wextra
TARGET = circuit
SOURCES = counter.c logic.c gates.c flipFlops.c print.c hacks.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)
