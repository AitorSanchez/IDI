all: 
	g++ -c model.cpp
	 g++ ej2.cpp -o prueba.x model.o -lglut -lGL -lGLU
	

clean: 
	rm -f *.o prueba
