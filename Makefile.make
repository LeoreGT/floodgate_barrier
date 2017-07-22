CC = g++

#Executable
Runner.o: main.cpp city.h street.h building.h
  $(CC) -o runner main.cpp city.h street.h building.h

clean:
	rm -f *.o

run:
	make clean
	make Runner.o
	./runner
