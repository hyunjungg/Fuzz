# Compiler
CC = x86_64-w64-mingw32-g++

# Compiler flags
CFLAGS = -Wall -I"include" -g

# Linker flags
LDFLAGS = -lntdll -lpsapi

# Target executable
TARGET = bin/NtExecutor.exe

# Source files (all .cpp files in the root directory)
SRCS = Alloc.cpp jsoncpp.cpp main.cpp Output.cpp RunSeed.cpp SetupArg.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)
OBJS := $(addprefix bin/, $(notdir $(OBJS)))

# Default target
all: $(TARGET)

# Rule to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Rule to compile .cpp files to .o files
bin/%.o: %.cpp | bin
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure bin directory exists
bin:
	mkdir -p bin

# Clean target to remove compiled files
clean:
	rm -f bin/*.o $(TARGET)

.PHONY: all clean

# Additional compile command example (not necessary for current structure)
compile:
	mkdir -p bin/{userspace,driver}
	x86_64-w64-mingw32-gcc src/userspace/selffuzz_test.c -I ../ -o bin/userspace/selffuzz_test.exe -Wall -mwindows
	x86_64-w64-mingw32-gcc src/driver/vuln_test.c -I ../ -o bin/driver/vuln_test.exe -Wall -lntdll -lpsapi
	x86_64-w64-mingw32-gcc src/driver/agent.c -I ../ -o bin/driver/agent.exe -Wall -lntdll -lpsapi

.PHONY: all clean
