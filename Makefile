
build: 
	gcc main.c queue.c utils.c -o quadtree 

run:
	./quadtree

clean: rm quadtree 
