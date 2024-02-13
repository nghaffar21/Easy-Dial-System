program.exe: main.o dialog.o easy_dial.o call_registry.o phone.o
	g++ -o program.exe main.o dialog.o easy_dial.o call_registry.o phone.o -L /home/usuari/lib -lesin

main.o: main.cpp mcj_enters.hpp mcj_enters.rep
	g++ -c main.cpp $(OPCIONS)

mcj_enters.o: mcj_enters.cpp mcj_enters.hpp mcj_enters.rep
	g++ -c mcj_enters.cpp $(OPCIONS)

clean:
	rm *.o
	rm *.exe
	rm *.gch
