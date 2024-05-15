TARGET = ./main

CC = gcc

PREF_SRC = src/
PREF_OBJ = obj/

MAIN = main
FUNC = functions

.PHONY = all, clean, build, run

all: clean build

clean:
	rm $(TARGET) $(PREF_OBJ)*.o

build: $(TARGET)

run: $(TARGET)
	$(TARGET)

$(TARGET): $(PREF_OBJ)$(MAIN).o $(PREF_OBJ)$(FUNC).o
	$(CC) -o $(TARGET) $(PREF_OBJ)*.o

$(PREF_OBJ)$(MAIN).o: $(PREF_SRC)$(MAIN).c
	$(CC) -c $(PREF_SRC)$(MAIN).c -o $(PREF_OBJ)$(MAIN).o

$(PREF_OBJ)$(FUNC).o: $(PREF_SRC)$(FUNC).c
	$(CC) -c $(PREF_SRC)$(FUNC).c -o $(PREF_OBJ)$(FUNC).o

