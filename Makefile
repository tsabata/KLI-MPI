#macros
CC=g++
CCFLAGS=-Wall -pedantic -Wno-long-long -O0 -ggdb
LBFLAGS=
Remove=rm -rf
Objects=src/objects/main.o
RUN=./kli
CPATH=src/objects/

     
#generates final binary and documentation
all: $(Objects)
	make compile
     
#build into final binary
compile: $(RUN)
     
clean:
	$(Remove) src/objects $(RUN)
     
#rules to make objects ----------------------------------------------------------------------------------
$(RUN): $(Objects)
	$(CC) $(CCFLAGS) $(Objects) -o $(RUN) $(LBFLAGS)
     
src/objects/main.o: src/main.cpp
	mkdir -p src/objects
	$(CC) $(CCFLAGS) -c src/main.cpp -o src/objects/main.o $(LBFLAGS)

