output: main.o buffer_queue.o monitor.o
	g++ -o output main.o buffer_queue.o monitor.o -pthread

main.o: main.cpp buffer_queue.h
	g++ -c main.cpp

buffer_queue.o: buffer_queue.cpp buffer_queue.h monitor.h
	g++ -c buffer_queue.cpp

monitor.o: monitor.cpp monitor.h
	g++ -c monitor.cpp

clean:
	rm *.o output
