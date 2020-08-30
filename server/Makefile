server: server.o Keyholder.o psi_utils.o
	g++ -L../libhcs/lib -std=c++11 -pthread  server.o Keyholder.o psi_utils.o -o server -lntl -lgmp -lhcs

server.o: server.h server.cpp
	g++ -I../libhcs/include -c -std=c++11 -pthread server.cpp  -lntl -lgmp -lhcs

Keyholder.o: Keyholder.h Keyholder.cpp
	g++ -I../libhcs/include -c -std=c++11 -pthread Keyholder.cpp  -lntl -lgmp -lhcs

psi_utils.o : ../global/psi_utils.cpp ../global/psi_utils.h
	g++ -I../libhcs/include -c -std=c++11 -pthread ../global/psi_utils.cpp -lntl -lgmp -lhcs

clean:
	rm *.o server
