all: compile run

compile:
	g++ src/*.cpp -lodbc -o build/app.out

run:
	./build/app.out

clean:
	rm -rf build/