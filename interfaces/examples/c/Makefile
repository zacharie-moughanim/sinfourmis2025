
all: build

build: check_file fourmis.so

fourmis.so:
	gcc -o fourmis.so -shared -fPIC fourmis.c

check_file:
	@if [ ! -f sinfourmis.h ]; then echo "sinfourmis.h not found"; exit 1; fi

clean:
	rm -f fourmis.so