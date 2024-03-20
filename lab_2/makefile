TARGET = ./main

CC = gcc

PREF_SRC = src/
PREF_OBJ = obj/

MAIN = main
VECTOR = vector
LIST = list

m = 0
MODE_NAME = "MODE"

.PHONY: all, clean, build, run

all: clean build

build: $(TARGET)

ifeq ($(m), 0)
SRC = $(PREF_SRC)$(MAIN).c $(PREF_SRC)$(VECTOR).c

$(TARGET): $(MAIN).o $(VECTOR).o
	gcc -o $(TARGET) $(PREF_OBJ)*.o	

$(MAIN).o: $(PREF_SRC)$(MAIN).c
	gcc -D$(MODE_NAME)=0 -c $(PREF_SRC)$(MAIN).c -o $(PREF_OBJ)$(MAIN).o

$(VECTOR).o: $(PREF_SRC)$(VECTOR).c
	gcc -c $(PREF_SRC)$(VECTOR).c -o $(PREF_OBJ)$(VECTOR).o
else
SRC = $(PREF_SRC)$(MAIN).c $(PREF_SRC)$(LIST).c

$(TARGET): $(MAIN).o $(LIST).o
	gcc -o $(TARGET) $(PREF_OBJ)*.o	

$(MAIN).o: $(PREF_SRC)$(MAIN).c
	gcc -D$(MODE_NAME)=1 -c $(PREF_SRC)$(MAIN).c -o $(PREF_OBJ)$(MAIN).o

$(LIST).o: $(PREF_SRC)$(LIST).c
	gcc -c $(PREF_SRC)$(LIST).c -o $(PREF_OBJ)$(LIST).o
endif

clean:
	rm $(TARGET) $(PREF_OBJ)*.o

run:
	$(TARGET)