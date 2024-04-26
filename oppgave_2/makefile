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
_DEPS = main.h eksamen_v24_oppgave2_all_lower.h eksamen_v24_oppgave2_all_upper.h eksamen_v24_oppgave2_anagram.h eksamen_v24_oppgave2_disjoint.h eksamen_v24_oppgave2_heterogram.h eksamen_v24_oppgave2_palindrom.h
DEPS = $(patsubst %, $(INCLDIR)/%, $(_DEPS))

_OBJS = main.o eksamen_v24_oppgave2_all_lower.o eksamen_v24_oppgave2_all_upper.o eksamen_v24_oppgave2_anagram.o eksamen_v24_oppgave2_disjoint.o eksamen_v24_oppgave2_heterogram.o eksamen_v24_oppgave2_palindrom.o
OBJS = $(patsubst %, $(OBJDIR)/%, $(_OBJS))

# RULES
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
	
.PHONY: clean
clean:
	$(RM) $(OBJDIR)/*.o *~core $(INCLDIR)/*~
