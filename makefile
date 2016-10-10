CXX=clang++
CXXFLAGS=-Wall -g -std=c++14
LINK_FLAGS=-lgtest -lgmock -pthread

all: server tests

server: main.o socket_functions_helper.o
	$(CXX) -o server main.o socket_functions_helper.o caesar_cipher.o $(LINK_FLAGS)

main.o: src/main.cpp src/socket_server.h src/socket_server.cpp caesar_cipher.o
	$(CXX) $(CXXFLAGS) -c src/main.cpp

socket_functions_helper.o: src/socket_functions_helper.h src/socket_functions_helper.cpp
	$(CXX) $(CXXFLAGS) -c src/socket_functions_helper.cpp

tests: caesar_cipher_tests.o caesar_cipher.o socket_server_tests.o socket_functions_helper.o
	$(CXX) -o tests test/main.cpp caesar_cipher_tests.o caesar_cipher.o socket_server_tests.o socket_functions_helper.o $(LINK_FLAGS)

socket_server_tests.o: test/socket_server_tests.cpp test/socket_functions_helper_mock.h
	$(CXX) $(CXXFLAGS) -c test/socket_server_tests.cpp

caesar_cipher_tests.o: test/caesar_cipher_tests.cpp
	$(CXX) $(CXXFLAGS) -c test/caesar_cipher_tests.cpp

caesar_cipher.o: src/caesar_cipher.h src/caesar_cipher.cpp
	$(CXX) $(CXXFLAGS) -c src/caesar_cipher.cpp

clean:
	rm *.o tests server
