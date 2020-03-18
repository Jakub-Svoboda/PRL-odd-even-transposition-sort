/*
	Odd-even sorting algorithm 
	Author: Jakub Svoboda
	Date 17.3.2020
	Login: xsvobo0z
	Email: xsvobo0z@stud.fit.vutbr.cz
*/

#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <iostream>
#include <fstream>

using namespace std;

#define TAG 0

uint32_t receiveNumber(int32_t processID, MPI_Status mpiStat){
	uint32_t number;		
	MPI_Recv(&number, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &mpiStat); 
	return number;
}

void loadFile(){
	char fileName[] = "numbers";	//file name   
	fstream fin;
	fin.open(fileName, ios::in);
	uint32_t targetProcessNum = 0;                   

	while(fin.good()){
		uint32_t loadedNum = fin.get();			//load a single number from a file
		if	(!fin.good()){			//stop at file end
			break;
		}	
		MPI_Send(&loadedNum, 1, MPI_INT, targetProcessNum, TAG, MPI_COMM_WORLD); //send init number
		targetProcessNum++;
	}
	fin.close();  	
}	
	
int main(int argc, char *argv[]){
	int32_t numOfProcessors = 0;	//zero processors by default
	int32_t processID = 0;			//ID of this process

	MPI_Init(&argc, &argv);			//initialize MPI
	MPI_Comm_size(MPI_COMM_WORLD, &numOfProcessors); //get the number of availible processes
	MPI_Comm_rank(MPI_COMM_WORLD ,&processID); 	//get the ID of this process

	if (processID == 0) {
		loadFile();
	}
	
	MPI_Status mpiStat;
	uint32_t number = receiveNumber(processID, mpiStat);
	uint32_t neighNumber;

	//Odd even transposition sort cycle:			
	uint32_t evenMax = (2*(numOfProcessors/2)) -1;	//10->8,  11->8				                 
    uint32_t oddMax = 2 *((numOfProcessors-1)/2);	//10->8,  11->10

	for (int i = 1; i <= (numOfProcessors/2); i++){
		if ((processID % 2 == 0) && (processID < evenMax)){			//0,2,4,6,8
			MPI_Send(&number, 1, MPI_INT, processID+1, TAG, MPI_COMM_WORLD);	//send num
			MPI_Recv(&number, 1, MPI_INT, processID+1, TAG, MPI_COMM_WORLD, &mpiStat);   //receive response
		} else if ((processID % 2 == 1) && (processID <= evenMax)){ //1,3,5,7,9
			MPI_Recv(&neighNumber, 1, MPI_INT, processID-1, TAG, MPI_COMM_WORLD, &mpiStat); //if even, then receive first
			if (neighNumber > number){	//if the number on left is larger
				MPI_Send(&number, 1, MPI_INT, processID-1, TAG, MPI_COMM_WORLD);	//then send mine back
				number = neighNumber;	//and save my number
			}
			else {
				MPI_Send(&neighNumber, 1, MPI_INT, processID-1, TAG, MPI_COMM_WORLD); //or just send it back
			}	
		}

		if ((processID % 2 == 1) && (processID < oddMax)){		//1,3,5,7,9
			MPI_Send(&number, 1, MPI_INT, processID+1, TAG, MPI_COMM_WORLD);	//send num
			MPI_Recv(&number, 1, MPI_INT, processID+1, TAG, MPI_COMM_WORLD, &mpiStat);   //receive response
		}else if ((processID !=0) && (processID <= oddMax)){	 //0,2,4,6,8
			MPI_Recv(&neighNumber, 1, MPI_INT, processID-1, TAG, MPI_COMM_WORLD, &mpiStat); //if even, then receive first
			if (neighNumber > number){	//if the number on left is larger
				MPI_Send(&number, 1, MPI_INT, processID-1, TAG, MPI_COMM_WORLD);	//then send mine back
				number = neighNumber;	//and save my number
			}
			else {
				MPI_Send(&neighNumber, 1, MPI_INT, processID-1, TAG, MPI_COMM_WORLD); //or just send it back
			}	
		} 
	}

	int* final = new int [numOfProcessors];
	//final=(int*) malloc(numprocs*sizeof(int));
	for (int i=1; i<numOfProcessors; i++){
		if (processID == 0){		//if master, receive
			MPI_Recv(&neighNumber, 1, MPI_INT, i, TAG, MPI_COMM_WORLD, &mpiStat); 	
			final[i] = neighNumber;
		}
		if (processID == i) {
			MPI_Send(&number, 1, MPI_INT, 0, TAG,  MPI_COMM_WORLD);
		}
	}

	if (processID == 0){
		final[0] = number;
		for (int i=0; i<numOfProcessors; i++){
			cout << "proc: "<< i <<" num: "<< final[i] << endl;
		}
	}
	
	MPI_Finalize(); 
	return 0;

}	



























