release: main.cpp wordLadder.hpp wordLadder.cpp linkedList.hpp
		g++ -std=c++14 -O3 -o./release main.cpp wordLadder.hpp wordLadder.cpp linkedList.hpp

debug: main.cpp wordLadder.hpp wordLadder.cpp linkedList.hpp
		g++ -std=c++14 -fsanitize=address -O0 -g3 -o./debug main.cpp wordLadder.hpp wordLadder.cpp linkedList.hpp

