appName := macro.exe
libs := -lSDL3 -lSDL3_image -lm
files := src/*.c src/textRenderLib/*.c

all: 
	@cls
	gcc -o bin/${appName} ${files} -I include/ -L libs ${libs}

run:
	@cls
	@bin/${appName}