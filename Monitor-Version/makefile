all: hw3_part2.cpp hw3_part2_improved.cpp dep
	g++ -o philosopher hw3_part2.cpp -pthread
	g++ -o improved hw3_part2_improved.cpp -pthread
	
dep:
	g++ -M *.cpp > dep
	
clean:
	rm dep philosopher improved
