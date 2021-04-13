OBJDIR = ./build
SRC = .
TARGET = zapper.out
CFLAGS = -Werror -Wall -Wpedantic -Wextra -Wno-shadow -O0 -g
CC = cc

$(OBJDIR)/%.o: $(SRC)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

all: $(TARGET)

clean:
	rm $(OBJDIR)/*.o
	rm $(TARGET)

OBJLIST = $(OBJDIR)/killer.o \
          $(OBJDIR)/internet_process.o

$(TARGET): $(OBJLIST)
	$(CC) -o $(TARGET) $(OBJLIST)

$(OBJDIR)/killer.o: $(SRC)/killer.c
$(OBJDIR)/internet_process.o: $(SRC)/internet_process.c $(SRC)/internet_process.h
