CC = x86_64-w64-mingw32-g++

CFLAGS = -Wall -I"include" -mWindows

LDFLAGS = -lntdll -lpsapi

TARGET = bin/NtExecutor.exe

SRCS = src/Alloc.cpp src/jsoncpp.cpp src/main.cpp src/Output.cpp src/RunSeed.cpp src/SetupArg.cpp

OBJS = $(SRCS:src/%.cpp=bin/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

bin/%.o: src/%.cpp | bin
	$(CC) $(CFLAGS) -c $< -o $@

bin:
	mkdir -p bin

clean:
	rm -f bin/*.o $(TARGET)

.PHONY: all clean

compile:
	mkdir -p bin/{userspace,driver}
	x86_64-w64-mingw32-gcc src/userspace/selffuzz_test.c -I ../ -o bin/userspace/selffuzz_test.exe -Wall -mwindows
	x86_64-w64-mingw32-gcc src/driver/vuln_test.c -I ../ -o bin/driver/vuln_test.exe -Wall -lntdll -lpsapi
	x86_64-w64-mingw32-gcc src/driver/agent.c -I ../ -o bin/driver/agent.exe -Wall -lntdll -lpsapi

.PHONY: all clean
