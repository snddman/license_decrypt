CXX = g++
CXXFLAGS = -Wall -std=c++11 -O2
LDFLAGS = -lssl -lcrypto

TARGET = license_decrypt
SOURCES = main.cpp

all: $(TARGET)
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean