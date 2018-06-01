generator_test:
	g++ --std=c++17 -o build/generator_test $(shell find src/generator -name "*.cpp")

clean:
	rm -f build/*