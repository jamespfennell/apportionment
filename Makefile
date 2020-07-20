

bin/apportionment: build/main.o build/apportionment.o
	clang++ -stdlib=libc++ build/main.o -o bin/apportionment

build/main.o: src/main.cpp include/heap.hpp include/apportionment.hpp src/csv.cpp
	clang++ -std=c++11 -c src/main.cpp -o build/main.o

build/apportionment.o: src/apportionment.cpp include/apportionment.hpp
	clang++ -std=c++11 -c src/apportionment.cpp -o build/apportionment.o
