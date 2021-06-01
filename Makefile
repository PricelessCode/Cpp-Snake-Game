CC = g++
CCFLAGS = -std=c++11

% : %.cpp
	$(CC) $(CCFLAGS) -o $@ $< -lncursesw

all:
	g++ -std=c++11 -o main.cpp fSnakeGame.cpp -lncursesw