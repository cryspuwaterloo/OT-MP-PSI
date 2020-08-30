benchmark: benchmark.o Elementholder.o Keyholder.o Recon.o client.o psi_utils.o
	g++ -fopenmp -L../libhcs/lib -std=c++11 benchmark.o Elementholder.o Keyholder.o Recon.o client.o psi_utils.o -o benchmark -lntl -lgmp -lhcs  

benchmark.o: benchmark.cpp 
	g++ -I../libhcs/include -O6 -c -std=c++11 benchmark.cpp

Elementholder.o: ../client/Elementholder.h ../client/Elementholder.cpp ../server/Keyholder.h ../server/Keyholder.cpp ../global/psi_utils.cpp ../global/psi_utils.h 
	g++ -I../libhcs/include -O6 -c -std=c++11 ../client/Elementholder.cpp 

Keyholder.o: ../server/Keyholder.h ../server/Keyholder.cpp
	g++ -I../libhcs/include -O6 -c -std=c++11 ../server/Keyholder.cpp 

Recon.o: ../client/Recon.cpp ../client/Recon.h 
	g++ -fopenmp -I../libhcs/include -O6 -c -std=c++11 ../client/Recon.cpp

client.o: ../client/client.cpp ../client/client.h 
	g++ -O6 -c -std=c++11 ../client/client.cpp 

psi_utils.o : ../global/psi_utils.cpp ../global/psi_utils.h
	g++ -fopenmp -I../libhcs/include -O6 -c -std=c++11 ../global/psi_utils.cpp

clean:
	rm *.o benchmark
