SOURCES      := aes.c test.c
OBJECTS	     := $(SOURCES:.c=.o)
INCLUDE_DIRS := ../include/
CFLAGS	     = -Wall -Wextra -std=c99 -O2
CPPFLAGS     = -I$(INCLUDE_DIRS)
LDFLAGS	     =
EXEC 	     := aes

.PHONY.: all clean mrproper help

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c %.h
	$(CC) -o $@ -c $< $(CFLAGS) $(CPPFLAGS)

clean:
	@rm -f *.o
	@echo "$(OBJECTS) has been deleted"

mrproper: clean
	@rm -f $(EXEC)
	@echo "$(EXEC) has been deleted"

help:
	@echo -e "make all\tBuilds mnk-game.c"
	@echo -e "make clean\tRemoves all .o files"
	@echo -e "make mrproper\tDoes a clean and removes the executable file"
	@echo -e "make help\tDisplays this text"


