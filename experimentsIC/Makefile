mainIC: difusioIC.o genetico.o greedyIC.o main.o 
	g++ -o mainIC difusioIC.o genetico.o greedyIC.o main.o

difusioIC.o: difusioIC.cc difusioIC.hh main.hh
	g++ -c difusioIC.cc -o difusioIC.o

genetico.o: genetico.cc genetico.hh main.hh
	g++ -c genetico.cc -o genetico.o

greedyIC.o: greedyIC.cc greedyIC.hh main.hh
	g++ -c greedyIC.cc -o greedyIC.o


main.o: main.cc main.hh difusioIC.hh genetico.hh greedyIC.hh
	g++ -c main.cc -o main.o

clean:
	rm -f *.o
	rm main
