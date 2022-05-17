# Sistemas Operativos 2021/2022. Projeto 1. Grupo SO-004
# Realizado por: Cosmin Trandafir Nº 57101
#                João Serafim     Nº 56376
#                Martim Baptista  Nº 56273
OBJ_dir = obj
OBJ = client.o driver.o main.o memory.o process.o restaurant.o configuration.o mesignal.o synchronization.o metime.o log.o stats.o
HEADERS_dir = include

magnaeats: $(OBJ)
	gcc $(addprefix $(OBJ_dir)/,$(OBJ)) -o bin/magnaeats -lrt -lpthread
%.o: src/%.c
	gcc $< -c -I $(HEADERS_dir) -o $(OBJ_dir)/$@
clean:
	rm -f $(OBJ_dir)/* bin/*