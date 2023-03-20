CC = g++
 
# The build target 
TARGET = kry

all: $(TARGET)

$(TARGET): $(TARGET).cpp
			$(CC) -o $(TARGET) $(TARGET).cpp