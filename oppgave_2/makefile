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
_DEPS = main.h eksamen_v24_oppgave2_all_lower.h eksamen_v24_oppgave2_all_upper.h eksamen_v24_oppgave2_anagram.h eksamen_v24_oppgave2_disjoint.h eksamen_v24_oppgave2_heterogram.h eksamen_v24_oppgave2_palindrom.h program.h
DEPS = $(patsubst %, $(INCLDIR)/%, $(_DEPS))

_OBJS = main.o eksamen_v24_oppgave2_all_lower.o eksamen_v24_oppgave2_all_upper.o eksamen_v24_oppgave2_anagram.o eksamen_v24_oppgave2_disjoint.o eksamen_v24_oppgave2_heterogram.o eksamen_v24_oppgave2_palindrom.o program.o
OBJS = $(patsubst %, $(OBJDIR)/%, $(_OBJS))

# RULES
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

check:
	xxd -b8 eksamen_v24_oppgave2.bin

valgrind:
	make
	valgrind --leak-check=yes --show-leak-kinds=all --track-origins=yes ./$(TARGET)

run:
	make
	./$(TARGET)
	
.PHONY: clean
clean:
	$(RM) $(OBJDIR)/*.o *~core $(INCLDIR)/*~
