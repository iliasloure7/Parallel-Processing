/*

Το κώδικα τον τρέχω στο visual studio, γι αυτό έχω χρησιμοποιήσει την scanf_s αντί για scanf,
σε περίπτωση που δεν λειτουργεί χρησιμοποιήστε την scanf.
Επίσης, στον κώδικα παρατηρούμε όταν μία διεργασία, τρέχει τις 10000000 επαναλήψεις το execution time είναι 2-3 seconds, 
ένω όταν το έτρεξα με 10 διεργασίες έτρεξε ο χρόνος μειώθηκε κατά 2 με 3 δεύτερα.

*/

#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

double getCoordinate()
{
	return (rand() * 1.0 / RAND_MAX) * 2 - 1;
}


int main(int argc, char** argv)
{
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);
	int rank, processSize;
	long  in, processLoop, start, end, processItr, iterations, step = 1;
	double x, y, rd;
	time_t s, e;
	MPI_Status status;

	in = 0;

	srand(time(NULL));

	// Number of processes.
	MPI_Comm_size(MPI_COMM_WORLD, &processSize);

	// Unique id for every process.
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if ( !rank ) 
	{
		// number of iterations
		scanf_s("%ld", &iterations);
		// send the number of iterations for each process.
		for (long i = 1; i < processSize; i++)
			MPI_Send(&iterations, 1, MPI_LONG, i, 0, MPI_COMM_WORLD);

	}
	else
		MPI_Recv(&iterations, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD, &status);

	// iterations for each process 
	processItr = iterations / processSize; 
	start = rank * processItr; 
	end = (rank + 1) * processItr;

	s = time(NULL);

	for (long i = start + step; i < end + step; i++) 
	{
		x = getCoordinate();
		y = getCoordinate();
		rd = sqrt(x * x + y * y);
		if (rd <= 1) in++;
	
	}

	e = time(NULL);

	if ( !rank )
	{
		long final_in;
		
		// process 0 computes final sum.
		for (long i = 1; i < processSize; i++)
		{
			MPI_Recv(&final_in, 1, MPI_LONG, i, 0, MPI_COMM_WORLD, &status);
			in += final_in;

		}
	
		printf("P = %f\n", 4.0 * in / iterations);
		printf("Execution time: %d\n", e - s);

	}
	else
		MPI_Send(&in, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD);
		
	MPI_Finalize();

	return 0;
}