OBJ_dir = obj
OBJ = client.o driver.o main.o memory.o process.o restaurant.o
HEADERS_dir = include
HEADERS = client.h driver.h main.h memory.h process.h restaurant.h

magnaeats: $(OBJ)
	gcc $(addprefix $(OBJ_dir)/,$(OBJ)) -o bin/magnaeats -lrt

%.o: src/%.c
	gcc $< -c -I include -o $(OBJ_dir)/$@

clean:
	rm