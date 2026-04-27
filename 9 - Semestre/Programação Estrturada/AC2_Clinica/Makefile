# Makefile — Sistema de Gerenciamento Clinica Veterinaria
# Uso: make        (compila)
#       make run   (compila e executa)
#       make clean (remove binarios)

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99
TARGET  = clinica
SRCS    = main.c animal.c consulta.c arquivo.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) animais.bin consultas.bin
