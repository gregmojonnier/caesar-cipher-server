CXX=clang++
CXXFLAGS=-Wall -g -std=c++14
LINK_FLAGS=-lgtest -lgmock -pthread

all: server tests

server: main.o socket_functions_helper.o caesar_cipher_data_interpreter.o
	$(CXX) -o server main.o socket_functions_helper.o caesar_cipher.o caesar_cipher_data_interpreter.o $(LINK_FLAGS)

main.o: src/main.cpp src/socket_server.h src/socket_server.cpp caesar_cipher.o
	$(CXX) $(CXXFLAGS) -c src/main.cpp

socket_functions_helper.o: src/socket_functions_helper.h src/socket_functions_helper.cpp
	$(CXX) $(CXXFLAGS) -c src/socket_functions_helper.cpp

tests: caesar_cipher_tests.o caesar_cipher.o socket_functions_helper.o caesar_cipher_data_interpreter.o caesar_cipher_data_interpreter_tests.o
	$(CXX) -o tests test/main.cpp caesar_cipher_tests.o caesar_cipher.o caesar_cipher_data_interpreter.o caesar_cipher_data_interpreter_tests.o socket_functions_helper.o $(LINK_FLAGS)

caesar_cipher_tests.o: test/caesar_cipher_tests.cpp
	$(CXX) $(CXXFLAGS) -c test/caesar_cipher_tests.cpp

caesar_cipher.o: src/caesar_cipher.h src/caesar_cipher.cpp
	$(CXX) $(CXXFLAGS) -c src/caesar_cipher.cpp

caesar_cipher_data_interpreter_tests.o: test/caesar_cipher_data_interpreter_tests.cpp
	$(CXX) $(CXXFLAGS) -c test/caesar_cipher_data_interpreter_tests.cpp

caesar_cipher_data_interpreter.o: src/caesar_cipher_data_interpreter.h src/caesar_cipher_data_interpreter.cpp src/idata_interpreter.h
	$(CXX) $(CXXFLAGS) -c src/caesar_cipher_data_interpreter.cpp

clean:
	rm *.o tests server
