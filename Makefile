appName := macro.exe
libs := -lSDL3 -lSDL3_image -lm
files := src/textRenderLib/*.c src/keyInteractionLib/*.c src/*.c

all: 
	@cls
	gcc -o bin/${appName} ${files} -I include/ -L libs ${libs}

run:
	@cls
	@bin/${appName}