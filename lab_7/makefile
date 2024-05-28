TARGET = ./main

CC = gcc

PREF_SRC = src/
PREF_OBJ = obj/

MAIN = main
FRONT = front
BACK = back

.PHONY = all, clean, build, run

all: clean build

clean:
	rm $(TARGET) $(PREF_OBJ)*.o

build: $(TARGET)

run: $(TARGET)
	$(TARGET)

$(TARGET): $(PREF_OBJ)$(MAIN).o $(PREF_OBJ)$(FRONT).o $(PREF_OBJ)$(BACK).o
	$(CC) -o $(TARGET) $(PREF_OBJ)*.o

$(PREF_OBJ)$(MAIN).o: $(PREF_SRC)$(MAIN).c
	$(CC) -c $(PREF_SRC)$(MAIN).c -o $(PREF_OBJ)$(MAIN).o

$(PREF_OBJ)$(FRONT).o: $(PREF_SRC)$(FRONT).c
	$(CC) -c $(PREF_SRC)$(FRONT).c -o $(PREF_OBJ)$(FRONT).o

$(PREF_OBJ)$(BACK).o: $(PREF_SRC)$(BACK).c
	$(CC) -c $(PREF_SRC)$(BACK).c -o $(PREF_OBJ)$(BACK).o	

