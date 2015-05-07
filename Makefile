GCC=g++
ARGS=-Wall

build: GPS

GPS: GPS.cpp List.hpp PriorityQueue.hpp
	$(GCC) $^ -o navigator $(ARGS)

clean:
	rm -rf *.o navigator
