
COMPILE = clang++ -std=c++11 -stdlib=libc++ 
# clang++ -std=c++11 -c src/* -MM

bin/apportionment: build/main.o build/apportionment.o build/csv.o
	$(COMPILE) build/*.o -o bin/apportionment

build/main.o: src/main.cpp include/apportionment.hpp include/csv.hpp include/heap.hpp
	$(COMPILE) -c src/main.cpp -o build/main.o

build/apportionment.o: src/apportionment.cpp include/apportionment.hpp include/heap.hpp
	$(COMPILE) -c src/apportionment.cpp -o build/apportionment.o

build/csv.o: src/csv.cpp include/csv.hpp
	$(COMPILE) -c src/csv.cpp -o build/csv.o

clean:
	rm -f bin/*
	rm -f build/*

.PHONY: clean
