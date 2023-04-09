CC = g++ -std=c++11
 
# The build target 
TARGET = kry

all: $(TARGET)

$(TARGET): $(TARGET).cpp
			$(CC) -o $(TARGET) $(TARGET).cpp