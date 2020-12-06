#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "mpi.h"

#define ILEITERACJI 150
#define ROZMIAR 64
#define SZEROKOSC 8

int mod(int a, int b)
{
    int r = a % b;
    if(r < 0)
        return (r + b);
    else
        return r;
}

int main(int argc, char **argv)
{

    int plansza[64] =
        {1, 1, 1, 1, 0, 0, 0, 1,
         0, 1, 0, 0, 0, 1, 0, 0,
         0, 0, 0, 1, 0, 0, 0, 0,
         0, 1, 0, 1, 0, 0, 0, 1,
         0, 1, 0, 1, 1, 0, 0, 0,
         0, 1, 0, 0, 0, 0, 0, 1,
         0, 0, 1, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 1, 0,};

    int num_procs;
    int ID, j;
    int ktoraIteracja = 0;
    int liczbaIteracji;

    if (argc == 1) liczbaIteracji = ILEITERACJI;
    else if (argc == 2) liczbaIteracji = atoi(argv[1]);
    else {
        printf("error\n");
        exit(1);
    }
    MPI_Status stat;

    if(MPI_Init(&argc, &argv) != MPI_SUCCESS) printf("MPI_Init error\n");

    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &ID);

    assert(SZEROKOSC % num_procs == 0); //Ma zwrócić kod błędu i wyłączyć program jeśli jest różne od zera.

    // Setup environment
    int *arr = (int *)malloc(SZEROKOSC * ((SZEROKOSC / num_procs) + 2) * sizeof(int));
    for (ktoraIteracja = 0; ktoraIteracja < liczbaIteracji; ktoraIteracja++) {
        j = SZEROKOSC;
        for (int i = ID * (ROZMIAR / num_procs); i < (ID + 1) * (ROZMIAR / num_procs); i++) {
            arr[j] = plansza[i];
            j++;
        }
        if (num_procs != 1) {
            int incoming_1[SZEROKOSC];
            int incoming_2[SZEROKOSC];
            int send_1[SZEROKOSC];
            int send_2[SZEROKOSC];
            if (ID % 2 == 0) {
                for (int i = 0; i < SZEROKOSC; i++)
                    send_1[i] = arr[i + SZEROKOSC];

                MPI_Ssend(&send_1, SZEROKOSC, MPI_INT, mod(ID - 1, num_procs), 1, MPI_COMM_WORLD);

                for (int i = 0; i < SZEROKOSC; i++)
                    send_2[i] = arr[(SZEROKOSC * (SZEROKOSC / num_procs)) + i];

                MPI_Ssend(&send_2, SZEROKOSC, MPI_INT, mod(ID + 1, num_procs), 1, MPI_COMM_WORLD);
            } else {
                MPI_Recv(&incoming_2, SZEROKOSC, MPI_INT, mod(ID + 1, num_procs), 1, MPI_COMM_WORLD, &stat);
                MPI_Recv(&incoming_1, SZEROKOSC, MPI_INT, mod(ID - 1, num_procs), 1, MPI_COMM_WORLD, &stat);
            }
            if (ID % 2 == 0) {
                MPI_Recv(&incoming_2, SZEROKOSC, MPI_INT, mod(ID + 1, num_procs), 1, MPI_COMM_WORLD, &stat);
                MPI_Recv(&incoming_1, SZEROKOSC, MPI_INT, mod(ID - 1, num_procs), 1, MPI_COMM_WORLD, &stat);
            } else {
                for (int i = 0; i < SZEROKOSC; i++)
                    send_1[i] = arr[i + SZEROKOSC];

                MPI_Ssend(&send_1, SZEROKOSC, MPI_INT, mod(ID - 1, num_procs), 1, MPI_COMM_WORLD);

                for (int i = 0; i < SZEROKOSC; i++)
                    send_2[i] = arr[(SZEROKOSC * (SZEROKOSC / num_procs)) + i];

                MPI_Ssend(&send_2, SZEROKOSC, MPI_INT, mod(ID + 1, num_procs), 1, MPI_COMM_WORLD);
            }
            for (int i = 0; i < SZEROKOSC; i++) {
                arr[i] = incoming_1[i];
                arr[(SZEROKOSC * ((SZEROKOSC / num_procs) + 1)) + i] = incoming_2[i];
            }
        } else {
            for (int i = 0; i < SZEROKOSC; i++)
                arr[i + ROZMIAR + SZEROKOSC] = plansza[i];

            for (int i = ROZMIAR; i < ROZMIAR + SZEROKOSC; i++)
                arr[i - ROZMIAR] = plansza[i - SZEROKOSC];
        }
        //game logic neighbours
        int * final = (int *)malloc(SZEROKOSC * ((SZEROKOSC / num_procs)) * sizeof(int));

        for (int k = SZEROKOSC; k < SZEROKOSC * ((SZEROKOSC / num_procs) + 1); k++) {
            int total_rows = SZEROKOSC * (SZEROKOSC / num_procs) + 2;
            int r = k / SZEROKOSC;
            int c = k % SZEROKOSC;
            int prev_r = mod(r - 1, total_rows);
            int prev_c = mod(c - 1, SZEROKOSC);
            int next_r = mod(r + 1, total_rows);
            int next_c = mod(c + 1, SZEROKOSC);

            int count = arr[prev_r * SZEROKOSC + prev_c] + arr[prev_r * SZEROKOSC + c] + arr[prev_r * SZEROKOSC + next_c] + arr[r * SZEROKOSC + prev_c] + arr[r * SZEROKOSC + next_c] + arr[next_r * SZEROKOSC + prev_c] + arr[next_r * SZEROKOSC + c] + arr[next_r * SZEROKOSC + next_c];
            if (arr[k] == 1) {
                if (count < 2)
                    final[k - SZEROKOSC] = 0;
                else if (count > 3)
                    final[k - SZEROKOSC] = 0;
                else
                    final[k - SZEROKOSC] = 1;
            } else {
                if (count == 3)
                    final[k - SZEROKOSC] = 1;
                else
                    final[k - SZEROKOSC] = 0;
            }
        }
        j = 0;
        for (int i = ID * (ROZMIAR / num_procs); i < (ID + 1) * (ROZMIAR / num_procs); i++) {
            plansza[i] = final[j];
            j++;
        }
        MPI_Gather(final, SZEROKOSC * (SZEROKOSC / num_procs), MPI_INT, &plansza, SZEROKOSC * (SZEROKOSC / num_procs), MPI_INT, 0, MPI_COMM_WORLD);

        // Output the updated grid state
        if (ID == 0) {
            printf("\nNr iteracji: %d, Gra w życie:\n", ktoraIteracja);
            for (j = 0; j < ROZMIAR; j++) {
                if (j % SZEROKOSC == 0)
                    printf("\n");

                printf("%d  ", plansza[j]);
            }
            printf("\n");
        }
    }
    free(arr);
    MPI_Finalize();
}
