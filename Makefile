main:
	g++ --std=c++17 -o build/main src/main.cpp src/generator/generator.cpp src/generator/problem.cpp src/generator/source/vector_source.cpp -lrestclient-cpp -lpthread -lboost_system -lcurl

generator_test:
	g++ --std=c++17 -o build/generator_test $(shell find src/generator -name "*.cpp") src/generator_test.cpp

game_test:
	g++ --std=c++17 -o build/game_test $(shell find src/generator -name "*.cpp") $(shell find src/game -name "*.cpp") src/game_test.cpp

clean:
	rm -f build/*