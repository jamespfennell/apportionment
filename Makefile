

bin/apportionment: build/main.o build/apportionment.o build/csv.o
	clang++ -stdlib=libc++ build/*.o -o bin/apportionment

build/main.o: src/main.cpp include/heap.hpp include/apportionment.hpp include/csv.hpp
	clang++ -std=c++11 -c src/main.cpp -o build/main.o

build/apportionment.o: src/apportionment.cpp include/apportionment.hpp
	clang++ -std=c++11 -c src/apportionment.cpp -o build/apportionment.o

build/csv.o: src/csv.cpp include/csv.hpp
	clang++ -std=c++11 -c src/csv.cpp -o build/csv.o
