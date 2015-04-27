SOURCES      := aes_tables.c aes.c
SOURCES2     := lookupTables.c aes.c
OBJECTS	     := $(SOURCES:.c=.o)
OBJECTS2     := $(SOURCES2:.c=.o)
INCLUDE_DIRS := ../include/
CFLAGS	   	  = -Wall -Wextra -std=c99 -O2
CPPFLAGS     = -I$(INCLUDE_DIRS)
LDFLAGS	     =
EXEC 	     := whitebox
EXEC2        := genTables

.PHONY.: all tables clean mrproper help

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

tables: $(EXEC2)

$(EXEC2): $(OBJECTS2)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)


%.o: %.c %.h
	$(CC) -o $@ -c $< $(CFLAGS) $(CPPFLAGS)

clean:
	@rm -f *.o
	@echo "Deleted all .o files"

mrproper: clean
	@rm -f $(EXEC)
	@echo "$(EXEC) has been deleted"
	@rm -f $(EXEC2)
	@echo "$(EXEC2) has been deleted"

help:
	@echo -e "make all\tBuilds the program"
	@echo -e "make tables\tComputes lookup tables"
	@echo -e "make clean\tRemoves all .o files"
	@echo -e "make mrproper\tDoes a clean and removes the executable file"


