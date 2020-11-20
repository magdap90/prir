#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mpi.h"
main(int argc, char **argv)
{
	int liczba_procesu,numer_procesu;
	int tag=20;

	float xp = 1.5;
    float xk=2.8;

    float dx, calka = 0, zmienna =0, t=0;


	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &numer_procesu); // p
	MPI_Comm_size(MPI_COMM_WORLD, &liczba_procesu); // n

	dx= (xk - xp)/liczba_procesu;
	if (numer_procesu == liczba_procesu-1)
	{
		printf("\nLiczba procesow = %d\n",numer_procesu);
		printf("Poczatek przedzialu= %2f\n",xp);
		printf("Koniec przedziału= %f\n\n",xk);

		//wysylamy zmiennych do procesu ostatniego
		MPI_Send(&xp, 1, MPI_DOUBLE, numer_procesu-1,tag, MPI_COMM_WORLD);
		MPI_Send(&xk, 1, MPI_DOUBLE, numer_procesu-1,tag, MPI_COMM_WORLD);
		MPI_Send(&dx, 1, MPI_DOUBLE, numer_procesu-1,tag, MPI_COMM_WORLD);
		MPI_Send(&calka, 1, MPI_DOUBLE, numer_procesu-1,tag, MPI_COMM_WORLD);
		MPI_Send(&zmienna, 1, MPI_DOUBLE, numer_procesu-1,tag, MPI_COMM_WORLD);
		MPI_Send(&t, 1, MPI_DOUBLE, numer_procesu-1,tag, MPI_COMM_WORLD);
	}

	//gdy proces ma numer wiekszy niz 0 i mniejszy niz proces ostatni
	if (numer_procesu>0 && numer_procesu<liczba_procesu-1)
	{
		MPI_Recv(&xp, 1, MPI_DOUBLE, numer_procesu+1, tag,MPI_COMM_WORLD, &status);
		MPI_Recv(&xk, 1, MPI_DOUBLE, numer_procesu+1, tag,MPI_COMM_WORLD, &status);
		MPI_Recv(&dx, 1, MPI_DOUBLE, numer_procesu+1, tag,MPI_COMM_WORLD, &status);
		MPI_Recv(&calka, 1, MPI_DOUBLE, numer_procesu+1, tag,MPI_COMM_WORLD, &status);
		MPI_Recv(&zmienna, 1, MPI_DOUBLE, numer_procesu+1, tag,MPI_COMM_WORLD, &status);
		MPI_Recv(&t, 1, MPI_DOUBLE, numer_procesu+1, tag,MPI_COMM_WORLD, &status);

		zmienna += pow((xp+numer_procesu*dx),2);
        t =zmienna+(((pow((xp),2))+(pow((xk),2)))/2);
        calka =t*dx;

        printf("Numer procesu: %d, wynik= %f \n", numer_procesu,calka);

		//przeslanie zmiennych do poprzedniego procesu
		MPI_Send(&xp, 1, MPI_DOUBLE, numer_procesu-1,tag, MPI_COMM_WORLD);
		MPI_Send(&xk, 1, MPI_DOUBLE, numer_procesu-1,tag, MPI_COMM_WORLD);
		MPI_Send(&dx, 1, MPI_DOUBLE, numer_procesu-1,tag, MPI_COMM_WORLD);
		MPI_Send(&calka, 1, MPI_DOUBLE, numer_procesu-1,tag, MPI_COMM_WORLD);
		MPI_Send(&zmienna, 1, MPI_DOUBLE, numer_procesu-1,tag, MPI_COMM_WORLD);
		MPI_Send(&t, 1, MPI_DOUBLE, numer_procesu-1,tag, MPI_COMM_WORLD);
	}

	if(numer_procesu == 0)
	{
		MPI_Recv(&xp, 1, MPI_DOUBLE, numer_procesu+1, tag,MPI_COMM_WORLD, &status);
		MPI_Recv(&xk, 1, MPI_DOUBLE, numer_procesu+1, tag,MPI_COMM_WORLD, &status);
		MPI_Recv(&dx, 1, MPI_DOUBLE, numer_procesu+1, tag,MPI_COMM_WORLD, &status);
		MPI_Recv(&calka, 1, MPI_DOUBLE, numer_procesu+1, tag,MPI_COMM_WORLD, &status);
		MPI_Recv(&zmienna, 1, MPI_DOUBLE, numer_procesu+1, tag,MPI_COMM_WORLD, &status);
		MPI_Recv(&t, 1, MPI_DOUBLE, numer_procesu+1, tag,MPI_COMM_WORLD, &status);

		zmienna += pow((xp+numer_procesu*dx),2);
        t =zmienna+(((pow((xp),2))+(pow((xk),2)))/2);
        calka =t*dx;
		printf("Numer procesu: %d, wynik= %f \n",numer_procesu, calka);
	}

MPI_Finalize();
}
