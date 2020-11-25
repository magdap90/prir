#include <stdio.h>
#include <stddef.h>
#include <mpi.h>

// Ping Pong program

int main(int argc, char** argv)
{
	MPI_Init(NULL,NULL);
	int nb_mpi_processes;
	MPI_Comm_size(MPI_COMM_WORLD, &nb_mpi_processes);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(nb_mpi_processes != 2) { printf("Ten program jest przeznaczony do uruchamiania tylko z 2 procesami"); return 0;}

	int tag = 7777;
	int pilka = 0;
	int n;
	int niter = 6;
	for (n=1;n<=niter;n++)
	{
		if(rank==0) {
			MPI_Send ( &pilka , 1 , MPI_INTEGER , 1 , tag , MPI_COMM_WORLD );
			MPI_Recv ( &pilka , 1 , MPI_INTEGER , 1 , tag , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			pilka = pilka + 2;
    	}

		if(rank==1) {
			MPI_Recv ( &pilka , 1 , MPI_INTEGER , 0 , tag , MPI_COMM_WORLD, MPI_STATUS_IGNORE );
			pilka = pilka + 1;
			MPI_Send ( &pilka, 1 , MPI_INTEGER , 0 , tag , MPI_COMM_WORLD );
		}

		printf("Process %d iter %d wartosc pilki : %d\n",rank,n, pilka);

		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();

	return 0;
}
