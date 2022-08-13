#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mpi.h"
#include <complex.h>

#define FULL_WIDTH 1024
#define CHUNK_WIDTH 32
#define MAX_ITER 1000

void plot(int* image_arr, FILE* img);
long iterator(Complex c, double im, double re);

int main(int argc, char* argv[])
{
    int* image_arr;
    int pixel_YX[3];
    int Y_start, X_start, CUR_CHUNK, disp = 0;
    int i, j;
    Complex c;
    FILE* img;
    int NUM_CHUNKS = (FULL_WIDTH / CHUNK_WIDTH) * (FULL_WIDTH / CHUNK_WIDTH);

    /** Time correlation**/
    double start, stop;
    float elapsed_time;

    /** MPI Related variables**/
    MPI_Status status, stat_recv;
    MPI_Request request;
    MPI_Datatype CHUNKxCHUNK, CHUNKxCHUNK_RE;
    int rankID, numProcs, numSlaves;

    /** MPI Initialization of environment**/
    MPI_Init(&argc, &argv);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankID);
    MPI_Barrier(MPI_COMM_WORLD);

    /** Create type**/
    int full_sizes[2] = { FULL_WIDTH, FULL_WIDTH };
    int sub_sizes[2] = { CHUNK_WIDTH, CHUNK_WIDTH };
    int starting[2] = { 0, 0 };
    int sendcounts[numProcs];
    int displs[numProcs];

    /** Create CHUNK */
    MPI_Type_create_subarray(
        2,
        full_sizes,
        sub_sizes,
        starting,
        MPI_ORDER_C,
        MPI_INT,
        &CHUNKxCHUNK
    );
    /** Set offset**/
    MPI_Type_create_resized(
        CHUNKxCHUNK,
        0,
        CHUNK_WIDTH * sizeof(int),
        &CHUNKxCHUNK_RE
    );
    /** Submit the type to use**/
    MPI_Type_commit(&CHUNKxCHUNK_RE);

    /** Number of slave processes */
    numSlaves = numProcs - 1;

    /** Print basic information */
    if (rankID == 0)
        printf("Runtime Stats:\n\tNum Procs:\t%d\n\tNum Slaves:\t%d\n", numProcs, numSlaves);

    MPI_Barrier(MPI_COMM_WORLD);

    /** Value of parameter C */
    c.re = -0.4;
    c.im = 0.5;

    /** Main process */
    if (rankID == 0) {
        img = fopen("t6.ppm", "w");

        if (img == NULL) {
            printf("Could not open handle to image\n");
            return 1;
        }

        fprintf(img, "P6\n%d %d 255\n", FULL_WIDTH, FULL_WIDTH);

        image_arr = (int*)malloc(FULL_WIDTH * FULL_WIDTH * sizeof(int));

        /** Start timer */
        start = MPI_Wtime();

        /** X and Y are calculated and distributed to each node**/
        for (pixel_YX[2] = 0; pixel_YX[2] < numSlaves; pixel_YX[2]++) {
            pixel_YX[0] = (pixel_YX[2] / (FULL_WIDTH / CHUNK_WIDTH)) * CHUNK_WIDTH; // Y
            pixel_YX[1] = (pixel_YX[2] % (FULL_WIDTH / CHUNK_WIDTH)) * CHUNK_WIDTH; // X

            MPI_Send(
                pixel_YX,
                3,
                MPI_INT,
                pixel_YX[2] + 1,
                0,
                MPI_COMM_WORLD
            );
        }

        /** Update current chunk and pixel */
        pixel_YX[2] = numSlaves;
        pixel_YX[0] = (pixel_YX[2] / (FULL_WIDTH / CHUNK_WIDTH)) * CHUNK_WIDTH; // Y
        pixel_YX[1] = (pixel_YX[2] % (FULL_WIDTH / CHUNK_WIDTH)) * CHUNK_WIDTH; // X

        /** Receives the current block from X and sends the next block to X */
        while (pixel_YX[2] < NUM_CHUNKS) {
            /** Detect the receiving buffer, calculate the displacement in the array, and receive */
            MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat_recv);

            disp = ((stat_recv.MPI_TAG * CHUNK_WIDTH) % FULL_WIDTH) +
                (((stat_recv.MPI_TAG * CHUNK_WIDTH) / FULL_WIDTH) * CHUNK_WIDTH * FULL_WIDTH);

            MPI_Recv(
                image_arr + disp,
                1,
                CHUNKxCHUNK_RE,
                stat_recv.MPI_SOURCE,
                stat_recv.MPI_TAG,
                MPI_COMM_WORLD,
                &status
            );

#ifdef DEBUG
            printf("Proc: MA\tJob: Recieved [# %d]\n", stat_recv.MPI_TAG);
#endif

            MPI_Send(
                pixel_YX,
                3,
                MPI_INT,
                status.MPI_SOURCE,
                0,
                MPI_COMM_WORLD
            );

            /** pixel_YX To next block value */
            pixel_YX[2]++;
            pixel_YX[0] = (pixel_YX[2] / (FULL_WIDTH / CHUNK_WIDTH)) * CHUNK_WIDTH;
            pixel_YX[1] = (pixel_YX[2] % (FULL_WIDTH / CHUNK_WIDTH)) * CHUNK_WIDTH;
        }

        /** Final reception matches initial transmission */
        for (i = 0; i < numSlaves; i++) {
            MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat_recv);

            disp = ((stat_recv.MPI_TAG * CHUNK_WIDTH) % FULL_WIDTH) + (((stat_recv.MPI_TAG * CHUNK_WIDTH) / FULL_WIDTH) * CHUNK_WIDTH * FULL_WIDTH);

            MPI_Recv(
                image_arr + disp,
                1,
                CHUNKxCHUNK_RE,
                stat_recv.MPI_SOURCE,
                stat_recv.MPI_TAG,
                MPI_COMM_WORLD,
                &status
            );
#ifdef DEBUG
            printf("Proc: MA\tJob: Recieved [# %d]\n", stat_recv.MPI_TAG);
#endif
        }

        /** End current process**/
        for (i = 0; i < numSlaves; i++)
            MPI_Send(
                0,
                0,
                MPI_INT,
                i + 1,
                0xFFFF,
                MPI_COMM_WORLD
            );

        /** Stop the timer and calculate elapsed_time */
        stop = MPI_Wtime();
        elapsed_time = stop - start;

#ifdef DEBUG
        printf("Proc: Ma\tJob: Plotting image\n");
#endif
        plot(image_arr, img);

        printf("Algorithm completed for,\n\t%d * %d pixels\n\t%d maximum iterations\n\t\tin %f seconds.\n", \
            FULL_WIDTH, FULL_WIDTH, \
            MAX_ITER, \
            elapsed_time);

        fclose(img);
    }
    else {
        /** Everyone assigns their image_arr part */
        image_arr = (int*)malloc(CHUNK_WIDTH * CHUNK_WIDTH * sizeof(int));

        /** Infinite loop until 'break;' */
        while (1) {
            MPI_Recv(
                pixel_YX,
                3,
                MPI_INT,
                0,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                &status
            );

            /** Check whether the call is terminated */
            if (status.MPI_TAG == 0xFFFF) {
                printf("Proc: %d \tJob: Exiting\n", rankID);
                break;
            }

            CUR_CHUNK = pixel_YX[2];

#ifdef DEBUG
            printf("Proc: %d \tChunk %d \tJob: Algorithm\n", rankID, CUR_CHUNK);
#endif

            /** For each Y value */
            for (i = 0; i < CHUNK_WIDTH; i++) {
                for (j = 0; j < CHUNK_WIDTH; j++) {
                    image_arr[(i * CHUNK_WIDTH) + j] = iterator(
                        c,
                        -(((pixel_YX[0] + i) - (FULL_WIDTH / 2)) / (double)FULL_WIDTH) * 2,
                        (((pixel_YX[1] + j) - (FULL_WIDTH / 2)) / (double)FULL_WIDTH) * 2
                    );
                }
            }

#ifdef DEBUG
            printf("Proc: %d \tJob: Returning [# %d]\n", rankID, CUR_CHUNK);
#endif

            /** Send part calculation imaging to MASTER */
            MPI_Send(
                image_arr,
                CHUNK_WIDTH * CHUNK_WIDTH,
                MPI_INT,
                0,
                CUR_CHUNK,
                MPI_COMM_WORLD
            );
        }
    }

    free(image_arr);

    /** End the running environment of MPI */
    MPI_Type_free(&CHUNKxCHUNK_RE);
    MPI_Finalize();
    fflush(stdout);

    return 0;
}

/**
Main iterative function of program
*/
long iterator(Complex c, double im, double re)
{
    Complex z;
    long itCount = 0;

    z.re = re;
    z.im = im;

    for (; itCount < MAX_ITER; itCount++) {
        z = cmplx_add(cmplx_squared(z), c);

        if (cmplx_magnitude(z) > 4)
            break;
    }

    return itCount + 1;
}

/**
    Drawing graphics
*/
void plot(int* image_arr, FILE* img)
{
    int i, j;
    unsigned char line[3 * FULL_WIDTH];

    for (i = 0; i < FULL_WIDTH; i++) {
        for (j = 0; j < FULL_WIDTH; j++) {
            if (*(image_arr + j + (i * FULL_WIDTH)) <= 63) {
                line[3 * j] = 255;
                line[3 * j + 1] = line[3 * j + 2] =
                    255 - 4 * *(image_arr + j + (i * FULL_WIDTH));
            }
            else {
                line[3 * j] = 255;
                line[3 * j + 1] = *(image_arr + j + (i * FULL_WIDTH)) - 63;
                line[3 * j + 2] = 0;
            }

            if (*(image_arr + j + (i * FULL_WIDTH)) == 320)
                line[3 * j] = line[3 * j + 1] = line[3 * j + 2] = 255;
        }

        fwrite(line, 1, 3 * FULL_WIDTH, img);
    }
}