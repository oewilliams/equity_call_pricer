CC=g++
CFLAGS= -c -std=c++17 -Wall -O3
LDFLAGS= -O3
SOURCES=main.cpp
DEPS=EquityCallPricer.h IRGenerator.h FXGenerator.h rng.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=equity_call_pricer

all: $(SOURCES) $(EXECUTABLE)


%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@