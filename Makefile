code_files := src/http.c src/parser.c src/network.c

build: $(code_files)
	mkdir -p "bin/"
	gcc $(code_files) -o bin/http
