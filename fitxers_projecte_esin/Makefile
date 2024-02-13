OPCIONS = -O0 -g -Wall -Wextra -Wno-sign-compare -std=c++14 -Wno-deprecated -I /home/nima42/incl #-Werror

program.exe: driver_easydial.o call_registry.o phone.o dialog.o easy_dial.o
	g++ -o program.exe driver_easydial.o call_registry.o phone.o dialog.o easy_dial.o -L /home/nima42/lib -lesin

driver_easydial.o: driver_easydial.cpp call_registry.hpp phone.hpp
	g++ -c driver_easydial.cpp $(OPCIONS)
    
call_registry.o: call_registry.cpp call_registry.hpp call_registry.rep phone.hpp
	g++ -c call_registry.cpp $(OPCIONS)
	
phone.o: phone.cpp phone.hpp phone.rep
	g++ -c phone.cpp $(OPCIONS)

dialog.o: dialog.cpp dialog.hpp easy_dial.hpp
	g++ -c dialog.cpp $(OPCIONS)

easy_dial.o: easy_dial.cpp easy_dial.hpp easy_dial.rep call_registry.hpp
	g++ -c easy_dial.cpp $(OPCIONS)
    
clean:
	rm *.o
	rm *.exe
	rm *.gch

test:
	./program.exe < jp_public.in | diff -u - jp_public.res
