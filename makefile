TODO make sure is correct
magnaeats: client.o driver.o main.o memory.o process.o restaurant.o
	gcc obj/client.o obj/driver.o obj/main.o obj/memory.o obj/process.o obj/restaurant.o -o magnaeats -lrt

client.o: client.c main.c memory.c
	gcc -c src/client.c src/main.c src/memory.c -c -I include

driver.o: driver.c main.c memory.c
	gcc -c src/driver.c src/main.c src/memory.c -c -I include

main.o: main.c memory.c
	gcc src/main.c src/memory.c -c -I include

memory.o: memory.c
	gcc src/memory.c -c -I include

process.o: process.c main.c memory.c
	gcc src/process.c src/main.c src/memory.c -c -I include

restaurant.o: restaurant.c main.c memory.c
	gcc src/restaurant.c src/main.c src/memory.c -c -I include
