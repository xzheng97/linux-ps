CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = 537ps
SCAN_BUILD_DIR = scan-build-out

all: main.o listPid.o pidCheck.o
	$(CC) -o $(EXE) main.o listPid.o pidCheck.o

main.o: main.c listPid.h pidCheck.h
	$(CC) $(WARNING_FLAGS) -c main.c

listPid.o: listPid.c listPid.h
	$(CC) $(WARNING_FLAGS) -c listPid.c

pidCheck.o: pidCheck.c pidCheck.h
	$(CC) $(WARNING_FLAGS) -c pidCheck.c
clean:
	rm -f $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)

scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html 
