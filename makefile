CXX=clang++
CXXFLAGS=-Wall -g -std=c++14
LINK_FLAGS=-lgtest -lgmock -pthread

all: server tests

server: server.o caesar_cipher.o
	$(CXX) -o server server.o caesar_cipher.o $(LINK_FLAGS)

server.o: src/server.h src/server.cpp
	$(CXX) $(CXXFLAGS) -c src/server.cpp

tests: caesar_cipher_tests.o caesar_cipher.o
	$(CXX) -o tests test/main.cpp caesar_cipher_tests.o caesar_cipher.o $(LINK_FLAGS)

caesar_cipher_tests.o: test/caesar_cipher_tests.cpp
	$(CXX) $(CXXFLAGS) -c test/caesar_cipher_tests.cpp

caesar_cipher.o: src/caesar_cipher.h src/caesar_cipher.cpp
	$(CXX) $(CXXFLAGS) -c src/caesar_cipher.cpp

clean:
	rm *.o tests server
