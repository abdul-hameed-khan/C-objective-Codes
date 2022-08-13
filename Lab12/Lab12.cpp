
/*
#include <iostream>

#include<mpi.h>
int main(int argc, char** argv)
{
	int mynode, totalnodes; int sum, startval, endval, accum;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &totalnodes);
	MPI_Comm_rank(MPI_COMM_WORLD, &mynode); 
	sum = 0;
	startval = 1000 * mynode / totalnodes + 1; 
	endval = 1000 * (mynode + 1) / totalnodes;
	for (int i = startval;i <= endval;i = i + 1)
	{
		sum = sum + i; 
		if (mynode != 0) {
			MPI_Send(&sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
		}

		else {
			for (int j = 1;j < totalnodes;j = j + 1)
			{
				MPI_Recv(&accum, 1, MPI_INT, j, 1, MPI_COMM_WORLD,
					&status);
				sum = sum + accum;
			}
		}
	}
	if (mynode == 0) { std::cout << "The sum from 1 to 1000 is: " << sum <<std:: endl; }
	MPI_Finalize();
}
*/
#include <mpi.h>
#include<stdio.h>
#include<iostream>
using namespace std;
int main(int argc, char** argv)
{
    char reply[100];
    char buff[128];

    int numprocs;
    int myid;
    int i;

    MPI_Status stat;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);


    /* -----------------------------------------
       Master process
       ----------------------------------------- */
    if (myid == 0)
    {
        printf("WE have %d processors\n", numprocs);

        /* -----------------------------------------
           Master process: send msg with tag 1234
           ----------------------------------------- */
        for (i = 1;i < numprocs;i++)
        {
            sprintf_s(buff, "Hello %d", i);

            MPI_Send(buff, 128, MPI_CHAR, i, 1234, MPI_COMM_WORLD);
        }

        /* ---------------------------------------------
           Master process: wait for msg with tag 4444
           --------------------------------------------- */
        for (i = 1;i < numprocs;i++)
        {
            MPI_Recv(buff, 128, MPI_CHAR, i, 4444, MPI_COMM_WORLD, &stat);
            cout << buff << endl;
        }
    }
    else
        /* -----------------------------------------
           Slave process: receive msg with tag 1234
           ----------------------------------------- */
    {
        /* -----------------------------------------
           Slave process: receive msg with tag 1234
           ----------------------------------------- */
        MPI_Recv(buff, 128, MPI_CHAR, 0, 1234, MPI_COMM_WORLD, &stat);

        sprintf_s(reply,
            " |--> Hello 0, Processor %d is present and accounted for !",
            myid);
        strcat_s(buff, reply);

        /* --------------------------------------------
           Slave process: send back msg with tag 4444
           -------------------------------------------- */
        MPI_Send(buff, 128, MPI_CHAR, 0, 4444, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}