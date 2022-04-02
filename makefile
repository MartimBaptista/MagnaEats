OBJ_dir = obj
OBJ = client.o driver.o main.o memory.o process.o restaurant.o
HEADERS_dir = include

magnaeats: $(OBJ)
	gcc $(addprefix $(OBJ_dir)/,$(OBJ)) -o bin/magnaeats -lrt

%.o: src/%.c
	gcc $< -c -I $(HEADERS_dir) -o $(OBJ_dir)/$@

clean:
	rm -f $(OBJ_dir)/* bin/*