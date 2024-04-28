TARGET = main
INCLDIR = ./include
SRCDIR = src
BINDIR = bin
LIBS = 
CC = gcc
CFLAGS = -O0 -g
CFLAGS += -I$(INCLDIR)
RM = rm -f

OBJDIR = obj

# PATHS
_DEPS = eksamen_v24_oppgave4.h sha1.c
DEPS = $(patsubst %, $(INCLDIR)/%, $(_DEPS))

_OBJS = eksamen_v24_oppgave4.o sha1.o
OBJS = $(patsubst %, $(OBJDIR)/%, $(_OBJS))

# RULES
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
	
valgrind:
	make
	valgrind --leak-check=yes --show-leak-kinds=all --track-origins=yes ./$(TARGET) eksamen_v24_oppgave4_pg2265.txt

run:
	make
	./$(TARGET) eksamen_v24_oppgave4_pg2265.txt
	
.PHONY: clean
clean:
	$(RM) $(OBJDIR)/*.o *~core $(INCLDIR)/*~ $(TARGET)
