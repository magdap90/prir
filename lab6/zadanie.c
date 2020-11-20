#include <stdio.h>
#include <math.h>
#include "mpi.h"
int main(int argc, char **argv){
	int p; // p-numer procesu
	int n; //liczba procecow
	int tag=50;

	float liczba =0, zmienna =0, wynik =0;


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &p);
	MPI_Comm_size(MPI_COMM_WORLD, &n);
	MPI_Status status;

	if(p==0)
	{
		liczba=(pow((-1.0),p))/(2*p+1);
        zmienna +=liczba;
        wynik = 4*zmienna;

		printf("\nNumer procesu: %d ,  wynik= %f\n", p, wynik);
		MPI_Send(&liczba, 1, MPI_INT ,p+1 ,tag, MPI_COMM_WORLD);
		MPI_Send(&zmienna, 1, MPI_INT ,p+1 ,tag, MPI_COMM_WORLD);
		MPI_Send(&wynik, 1, MPI_INT ,p+1 ,tag, MPI_COMM_WORLD);
	}

	if((p>0)&&(p<n-1)){

		MPI_Recv(&liczba, 1, MPI_INT, p-1, tag, MPI_COMM_WORLD, &status);
		MPI_Recv(&zmienna,1, MPI_INT, p-1, tag, MPI_COMM_WORLD, &status);
		MPI_Recv(&wynik, 1, MPI_INT, p-1, tag, MPI_COMM_WORLD, &status);

        liczba=(pow((-1.0),p))/(2* p +1);
        zmienna +=liczba;
        wynik = 4*zmienna;

		printf("Numer procesu: %d ,  wynik= %f\n", p, wynik);

		if(p!=n-1){
			MPI_Send(&liczba, 1, MPI_INT,p+1,tag,MPI_COMM_WORLD);
			MPI_Send(&zmienna, 1, MPI_INT,p+1,tag,MPI_COMM_WORLD);
			MPI_Send(&wynik, 1, MPI_INT,p+1,tag,MPI_COMM_WORLD);
		}
	}

MPI_Finalize();
return 0;
}
