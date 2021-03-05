#CotoiVictoritaMarinela_grupa312CA
build: image_editor	

image_editor : image_editor.c
	gcc -Wall -Wextra image_editor.c -o image_editor -lm

run: image_editor
	./image_editor

clean:
	-rm -rf image_editor
