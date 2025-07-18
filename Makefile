TARGET = main
SRCS = main.cpp fsm.cpp

CXX = g++
CXXFLAGS = -std=c++17 -Wall

$(TARGET): $(SRCS) 
	$(CXX) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)