# All Targets
all: start

# Tool invocations
# Executable "start" depends on the files ProjectTest.o and Tests.o.
start: bin/ProjectTest.o bin/Tests.o
	@echo 'Building target: start'
	@echo 'Invoking: C++ Linker'
	g++ -pthread -o bin/start bin/ProjectTest.o bin/Tests.o
	@echo 'Finished building target: start'
	@echo 'Executing Project Test'
	reset
	bin/start $(ARGS)
	rm -f bin/*

gdb: bin/ProjectTest.o bin/Tests.o
	g++ -pthread -o bin/start bin/ProjectTest.o bin/Tests.o
	gdb bin/start 
	rm -f bin/* 
	
# Depends on the source and header files
bin/ProjectTest.o: src/ProjectTest.cpp
	g++ -g -Wall -Werror -Weffc++ -std=c++11 -c -Iinclude -o bin/ProjectTest.o src/ProjectTest.cpp

# Depends on the source and header files
bin/Tests.o: src/Tests.cpp
	g++ -g -Wall -Werror -Weffc++ -std=c++11 -c -Iinclude -o bin/Tests.o src/Tests.cpp

#Clean the build directory
clean:
	rm -f bin/*
