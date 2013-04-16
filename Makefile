CXXFLAGS += -Wall -g3 -std=c++0x
CXXLFLAGS += -lpthread -luuid
TARGET = practice

.PHONY: clean tag

$(TARGET): practice.o
	$(CXX) $^ -o $@ $(CXXLFLAGS)

practice.o: practice.cpp Utils.h
	$(CXX) -c $(CXXFLAGS) practice.cpp -o $@ 

clean:
	@rm -vf $(TARGET)
	@rm -vf *.o

tag:
	@ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .
