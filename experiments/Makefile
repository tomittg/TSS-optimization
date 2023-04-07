main: difusioLT.o genetico.o greedyLT.o main.o 
	g++ -o main difusioLT.o genetico.o greedyLT.o main.o

difusioLT.o: difusioLT.cc difusioLT.hh main.hh
	g++ -c difusioLT.cc -o difusioLT.o

genetico.o: genetico.cc genetico.hh main.hh
	g++ -c genetico.cc -o genetico.o

greedyLT.o: greedyLT.cc greedyLT.hh main.hh
	g++ -c greedyLT.cc -o greedyLT.o


main.o: main.cc main.hh difusioLT.hh genetico.hh greedyLT.hh
	g++ -c main.cc -o main.o

clean:
	rm -f *.o
	rm main
