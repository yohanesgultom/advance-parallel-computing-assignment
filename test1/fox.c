 // mpiApp.cpp : Defines the entry point for the console application.
 // http://aetos.it.teithe.gr/~amarg/MPI/mpiCode/fox.c
#include <stdafx.h>

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ORDER 9


typedef struct GridInfo {
	int processNumber;     // total number of processed
	MPI_Comm gridComm;	   // communicator for entire grid
	MPI_Comm rowComm;	   // communicator for current row
	MPI_Comm colComm;	   // communicator for current column
	int gridOrder;		   // the order of grid
	int currentRow;		   // row of current process
	int currentCol;		   // column of current process
	int gridCommRank;	   // rank of current process in gridComm
	} GridInfo;


void PrintGridInfo (GridInfo * grid) {
	printf ("Number of Processes is %d\n", grid->processNumber);
	printf ("Grid Comm Identifier is %d\n", grid->gridComm);
	printf ("Row Comm Identifier is %d\n", grid->rowComm);
	printf ("Column Comm Identifier is %d\n", grid->colComm);
	printf ("Grid Order is %d\n", grid->gridOrder);
	printf ("Current Process Coordinates are (%d, %d)\n",
		     grid->currentRow, grid->currentCol);
	printf ("Process rank in Grid is %d\n", grid->gridCommRank); }

double ** Allocate2DMatrix (int rows, int columns) {
  int counter; double ** matrix;
  matrix = (double **) malloc (rows*sizeof(double *));
  if (!matrix) return (NULL);
  for (counter=0;counter<rows;counter++) {
    matrix[counter] = (double *) malloc (columns*sizeof(double));
    if (!matrix[counter]) return (NULL);}
  return matrix; }


void Free2DMatrix (double ** matrix, int rows) {
  int counter;
  for (counter=0;counter<rows;counter++)
    free ((double *)matrix[counter]);
  free ((double **)matrix);}


int SetupGrid (GridInfo * grid) {
	int flag, worldRank;
	// cartesian topology is 2D
	int dims[2], periods[2], gridCoords[2], subCoords[2];
	// if MPI has not been initialized, abort procedure
	MPI_Initialized(&flag);
	if (flag==false) return (-1);

	// get the process rank and the comm size
	MPI_Comm_rank (MPI_COMM_WORLD, &worldRank);
	MPI_Comm_size (MPI_COMM_WORLD, &(grid->processNumber));

	// check if process number is a perfect square
	if (sqrt((double)grid->processNumber)==
	        (int)sqrt((double)grid->processNumber))
		grid->gridOrder=(int)sqrt((double)grid->processNumber);
	else return (-2);

	dims[0]=dims[1]=grid->gridOrder;
	// a circular shift is required for the second dimension
	periods[0]=periods[1]=true;
	// create communicator for the process grid
	MPI_Cart_create (MPI_COMM_WORLD, 2, dims, periods,
		true, &(grid->gridComm));

	// retrieve the process rank in the grid Communicator
	// and the process coordinates in the cartesian topology
	MPI_Comm_rank (grid->gridComm, &(grid->gridCommRank));
	MPI_Cart_coords (grid->gridComm, grid->gridCommRank,
		2, gridCoords);
	grid->currentRow=gridCoords[0];
	grid->currentCol=gridCoords[1];

	// setup row communicators
	subCoords[0]=0;
	subCoords[1]=1;
	MPI_Cart_sub (grid->gridComm, subCoords, &(grid->rowComm));

	// setup column communicators
	subCoords[0]=1;
	subCoords[1]=0;
	MPI_Cart_sub (grid->gridComm, subCoords, &(grid->colComm));
	return (0); }


// this procedure performs the submatrix multiplication
// for each system process
void LocalMatrixProduct (double ** A, double ** B,
		  double ** C, int dim) {
	int i,j,k;
	for (i=0;i<dim;i++) {
		for (j=0;j<dim;j++) {
			 C[i][j]=0.0;
			 for (k=0;k<dim;k++) {
				  C[i][j]+=A[i][k]*B[k][j];}}}}


// the implementation of the Fox algorithm
int Fox (int matrixOrder, GridInfo * grid, double ** localA,
		 double ** localB, double ** localC) {
	int i,sourceRank, x,y, destRank, root;
	int dim=matrixOrder/grid->gridOrder;
	double ** tempMatrix; MPI_Status status;

	// these are the source and destination ranks
	// for circular shift of B elements
	sourceRank=(grid->currentRow+1)%grid->gridOrder;
	destRank=(grid->currentRow+grid->gridOrder-1)%grid->gridOrder;

	// source and dstination ranks can also be specified by a call to
	// MPI_Cart_shift with arguments

	// MPI_Cart_shift (grid->colComm, 0, 1, &destRank, &sourceRank);

	tempMatrix=Allocate2DMatrix(dim,dim);

//	printf ("Grid order is %d\n", grid->gridOrder);

	for (i=0;i<grid->gridOrder;i++) {



		 printf ("FOX ALGORITHM->STAGE %d\n", i);
		 root=(grid->currentRow+i)%grid->gridOrder;

		 printf ("Root process is %d, Grid Column is %d, Current Process is %d\n",
			 root, grid->currentCol, grid->gridCommRank);

		 if (root==grid->currentCol) {
			 // if the current process belongs to the grid main diagocal
			 // broadcasts its copy of localA to the row processes


		    printf ("Process %d (diagonal) broadcasting localA...\n", grid->gridCommRank);
			for (x=0;x<dim;x++)
				for (y=0;y<dim;y++)
					printf ("localA[%d][%d](-%d-)=%.3f\n",
						x,y,grid->gridCommRank,localA[x][y]);


			 MPI_Bcast (localA, dim*dim, MPI_DOUBLE, root, grid->rowComm);
			 LocalMatrixProduct (localA, localB, localC, dim); }
		 else {
			 // otherewise the tempMatrix is broadcasted


		    printf ("Process %d broadcasting tempA...\n", grid->gridCommRank);
			for (x=0;x<dim;x++)
				for (y=0;y<dim;y++)
					printf ("tempA[%d][%d](-%d-)=%.3f\n",
						x,y,grid->gridCommRank, tempMatrix[x][y]);

			 MPI_Bcast (tempMatrix, dim*dim, MPI_DOUBLE, root, grid->rowComm);
			 LocalMatrixProduct (tempMatrix, localB, localC, dim); }
		 // the next function performs the circular shift and replacement
		 // of localB values
		 MPI_Sendrecv_replace (localB, 1, MPI_DOUBLE, destRank, 0,
			 sourceRank, 0, grid->colComm, &status); }

//	Free2DMatrix (tempMatrix, dim);


//	for (i=0;i<dim;i++)
//		for (int j=0;j<dim;j++)
//			printf ("localC[%d][%d](-%d-)=%.3f\n",
//			    i,j,grid->gridCommRank,localC[i][j]);


	return (0); }



// this function can be used instead of ReadMatrix to scatter data
// to system processes according to their rank in process mesh

double * ScatterProcessData (double ** matrix, GridInfo * grid, int N) {
	int i,j,x,y, samples = (int) pow (N,2), procId;
	int row, col, temp;
	int pNum=grid->processNumber, dim=N/grid->gridOrder;
	double * buffer = (double *) malloc (samples*sizeof(double));
	if (!buffer) return (NULL);
	for (i=0;i<grid->gridOrder;i++) {
		for (j=0;j<grid->gridOrder;j++) {
			procId = i*grid->gridOrder+j;
			for (x=0;x<dim;x++) {
			   row = i*dim+x;
				  for (y=0;y<dim;y++) {
					  temp = procId*dim*dim+x*dim+y;
					  col = j*dim+y;
					  buffer[temp]=matrix[row][col];
				  }}}}
	return buffer; }


// this function assigns to each process the correct submatrix

double ** ReadMatrix (double ** matrix, GridInfo *  grid, int N) {
    int x,y, dim, pRank, coords[2];
	double ** subMatrix;
	dim = N/grid->gridOrder;
	subMatrix=Allocate2DMatrix (dim, dim);
	MPI_Comm_rank (grid->gridComm, &pRank);
	MPI_Cart_coords (grid->gridComm, pRank, 2, coords);
	for (x=0;x<dim;x++)
		for (y=0;y<dim;y++)
			 subMatrix[x][y]=
				 matrix[dim*coords[0]+x][dim*coords[1]+y];
	return subMatrix;}



// WriteMatrix is called by each process to return the calculated localC submatrix.
// In this way the final cMatrix of order N is formed

void WriteMatrix (double ** subMatrix, double ** matrix, GridInfo * grid, int N) {
   int x,y, dim, pRank, coords[2], i, size;
   MPI_Status status;
	dim = N/grid->gridOrder;
	MPI_Comm_rank (grid->gridComm, &pRank);
	MPI_Cart_coords (grid->gridComm, pRank, 2, coords);


	if (pRank) {
		MPI_Send (&subMatrix[0][0], dim*dim, MPI_DOUBLE, 0, 0, grid->gridComm);
		MPI_Send (coords, 2, MPI_INT, 0, 0, grid->gridComm); }



	else {


		printf ("My own submatrix is \n");
		for (x=0;x<dim;x++) {
			for (y=0;y<dim;y++)
				printf ("%.3f ", subMatrix[x][y]);
			printf ("\n");}

		for (x=0;x<dim;x++)
			for (y=0;y<dim;y++)
				 matrix[dim*coords[0]+x][dim*coords[1]+y]=
					subMatrix[x][y];

		MPI_Comm_size (grid->gridComm, &size);
		for (i=1;i<size;i++) {


		MPI_Recv (&subMatrix[0][0], dim*dim, MPI_DOUBLE, i, 0, grid->gridComm, &status);
		MPI_Recv (coords, 2, MPI_INT, i, 0, grid->gridComm, &status);

		printf ("Process %d received from process %d the coordinates (%d,%d)\n", pRank, status.MPI_SOURCE,
			coords[0], coords[1]);

		printf ("Process %d received from process %d the following submatrix\n", pRank, status.MPI_SOURCE);
		for (x=0;x<dim;x++) {
			for (y=0;y<dim;y++)
				printf ("%.3f ", subMatrix[x][y]);
			printf ("\n");


		}


		for (x=0;x<dim;x++)
			for (y=0;y<dim;y++)
				 matrix[dim*coords[0]+x][dim*coords[1]+y]=
				 subMatrix[x][y]; }



	}




	}
















int main (int argc, char ** argv) {
	int i,j,rank, N=ORDER, dim;
	GridInfo grid;
	double ** localA, ** localB, ** localC;

	int k;

	double ** aMatrix;
	double ** bMatrix;
	double ** cMatrix;


	// the square matrices A, B and C of order N
	// are allocated

	aMatrix=Allocate2DMatrix(N,N);
	bMatrix=Allocate2DMatrix(N,N);
	cMatrix=Allocate2DMatrix(N,N);
	if ((!aMatrix)||(!bMatrix)||(!cMatrix))
		return (-1);

	// the mpi environment is initialized and the
	// available process number is retrieved

	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);

	// the gridInfo structure is initialized, too and
	// the order of the local matrices is calculated

	SetupGrid (&grid);
	dim = N/grid.gridOrder;

	// the localC submatrix is allocated

	localC=Allocate2DMatrix(dim,dim);
	if (!localC) return (-1);

	for (i=0;i<ORDER;i++) {
		for (j=0;j<ORDER;j++) {
			aMatrix[i][j]=(double)rand()/RAND_MAX;
			bMatrix[i][j]=(double)rand()/RAND_MAX;
			cMatrix[i][j]=0.0; }}


	// serial matric multiplication

/*	for (i=0;i<N;i++) {
		for (j=0;j<N;j++) {
			cMatrix[i][j]=0.0;
			for (k=0;k<N;k++) {
				cMatrix[i][j]+=aMatrix[i][k]*bMatrix[k][j]; }}}

	if (rank==0) {
	for (i=0;i<N;i++) {
		for (j=0;j<N;j++)
			printf ("%.3f ", cMatrix[i][j]);
		printf ("\n"); }}*/





	// all process are synchronized at this point
	MPI_Barrier (MPI_COMM_WORLD);

	// each process reads its own local submatrices
	localA = ReadMatrix (aMatrix, &grid, N);
	localB = ReadMatrix (bMatrix, &grid, N);





	// the fox algorithm is performed to multiply the
	// local submatrices

	Fox (N, &grid, localA, localB, localC);

	// after calculation the processes are synchronized again

	MPI_Barrier (MPI_COMM_WORLD);

	WriteMatrix (localC, cMatrix, &grid, N);

	printf ("\n\n");

	if (rank==0) {
		for (i=0;i<N;i++) {
			for (j=0;j<N;j++)
				printf ("%.3f ", cMatrix[i][j]);
			printf ("\n"); }}





	// the allocated memory buffers are freed before termination

/*	Free2DMatrix (localA, dim);
	Free2DMatrix (localB, dim);
	Free2DMatrix (localC, dim);

	Free2DMatrix (aMatrix, N);
	Free2DMatrix (bMatrix, N);
	Free2DMatrix (cMatrix, N); */

	// MPI is finalized
	MPI_Finalize();

	return (0); }
