SERVER_EXE = server
SERVER_SOURCES = Server.cpp
RETRIEVER_EXE = retriever
RETRIEVER_SOURCES = Retriever.cpp

CC = g++
CXXFLAGS = -std=c++14 -g -Wall -Wextra

all:
	$(CC) -pthread $(CXXFLAGS) $(SERVER_SOURCES) -o $(SERVER_EXE)
	$(CC) $(CXXFLAGS) $(RETRIEVER_SOURCES) -o $(RETRIEVER_EXE)
clean:
	rm $(RETRIEVER_EXE) $(SERVER_EXE)