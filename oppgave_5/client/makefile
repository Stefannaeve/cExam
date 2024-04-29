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
_DEPS = main.h client.h
DEPS = $(patsubst %, $(INCLDIR)/%, $(_DEPS))

_OBJS = main.o client.o
OBJS = $(patsubst %, $(OBJDIR)/%, $(_OBJS))

# RULES
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
	
valgrind:
	make
	valgrind --leak-check=yes --show-leak-kinds=all --track-origins=yes ./$(TARGET) eksamen_v24_oppgave4_pg2265.txt
	
client1:
	make
	./$(TARGET) -server 127.0.0.1 -port 8080 -phone 1234

client2:
	make
	./$(TARGET) -server 127.0.0.1 -port 8081 -phone 4321
	
.PHONY: clean
clean:
	$(RM) $(OBJDIR)/*.o *~core $(INCLDIR)/*~
