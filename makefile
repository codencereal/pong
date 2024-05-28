build:
	gcc src/pong.c -lSDL2 -o bin/pong

clean:
	rm bin/*

run:
	./bin/pong
