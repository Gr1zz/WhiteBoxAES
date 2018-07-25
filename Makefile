CSOURCES      := main.c aes.c
CXXSOURCES    := genTables.cpp RandMat.cpp aes.c
OBJECTS	      := $(CSOURCES:.c=.o)
OBJECTS2	  := $(CXXSOURCES:.cpp=.o)
OBJECTS2	  := $(CXXSOURCES:.c=.o)
CFLAGS	   	   = -Wall -Wextra -std=c99 -O2
CPPFLAGS       = -lntl -lgmp -lm
LDFLAGS	       =
EXEC 	      := whitebox
EXEC2         := genTables

.PHONY.: all tables clean mrproper help

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $(CSOURCES) $(CFLAGS) $(LDFLAGS)

tables: $(EXEC2)

$(EXEC2): $(OBJECTS2)
	g++ -o $@ $(OBJECTS2) $(OBJECTS3) $(CPPFLAGS)


%.o: %.c %.h
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp %.h
	g++ -o $@ -c $< $(CPPFLAGS)

clean:
	@rm -f *.o
	@echo "Deleted all .o files"

mrproper: clean
	@rm -f $(EXEC)
	@echo "$(EXEC) has been deleted"
	@rm -f $(EXEC2)
	@echo "$(EXEC2) has been deleted"

help:
	@echo -e "make all\tBuilds the whitebox"
	@echo -e "make genTables\tGenerates the tables"
	@echo -e "make clean\tRemoves all .o files"
	@echo -e "make mrproper\tDoes a clean and removes the executable file"


