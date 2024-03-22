/*
CSC139 
Spring 2023
First Assignment
Pulido, Julian
Section #03
OSs Tested on: Linux
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>

/*Buffer - main data storage area
*Header- control center for shared memory block
*/
// Size of shared memory block
// Pass this to ftruncate and mmap
#define SHM_SIZE 4096

// Global pointer to the shared memory block
// This should receive the return value of mmap
// Don't change this pointer in any function
void* gShmPtr;

// You won't necessarily need all the functions below
void Producer(int, int, int);
void InitShm(int, int);
void SetBufSize(int);
void SetItemCnt(int);
void SetIn(int);
void SetOut(int);
void SetHeaderVal(int, int);
int GetBufSize();
int GetItemCnt();
int GetIn();
int GetOut();
int GetHeaderVal(int);
void WriteAtBufIndex(int, int);
int ReadAtBufIndex(int);
int GetRand(int, int);


int main(int argc, char* argv[])
{
        pid_t pid;
        int bufSize; // Bounded buffer size
        int itemCnt; // Number of items to be produced
        int randSeed; // Seed for the random number generator 

        if(argc != 4){
		printf("Invalid number of command-line arguments\n");
		exit(1);
        }
	bufSize = atoi(argv[1]);
	itemCnt = atoi(argv[2]);
	randSeed = atoi(argv[3]);
	// Write code to check the validity of the command-line arguments
        //exit program if buffsize not within (2,600)
        if(bufSize <2 || bufSize > 600 )
        {
            printf("Buffer size not within (2,600)");
                exit(1);
        }
        if(itemCnt <= 0)
        {
            printf("Item count needs to be > 0");
                exit(1);
        }

        // Function that creates a shared memory segment and initializes its header
    InitShm(bufSize, itemCnt); 
	/* fork a child process */ 
	pid = fork();

	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed\n");
		exit(1);
	}
	else if (pid == 0) { /* child process */
		printf("Launching Consumer \n");
		execlp("./consumer","consumer",NULL);
	}
	else { /* parent process */
		/* parent will wait for the child to complete */
		printf("Starting Producer\n");
		
               // The function that actually implements the production
               Producer(bufSize, itemCnt, randSeed);
		
	       printf("Producer done and waiting for consumer\n");
	       wait(NULL);		
	       printf("Consumer Completed\n");
        }
    
        return 0;
}


// Function that creates a shared memory segment and initializes its header
void InitShm(int bufSize, int itemCnt)
{

    //points to next free position in the buffer
    int in = 0;
    //points to first full position in the buffer
    int out = 0;
    const char *name = "OS_HW1_julianPulido"; // Name of shared memory object to be passed to shm_open

     // Write code here to create a shared memory block and map it to gShmPtr  
     //create shared memory block with shm_open method. returns -1 if fails
     // **Extremely Important: map the shared memory block for both reading and writing 
     // Use PROT_READ | PROT_WRITE
     int fd = shm_open(name, O_RDWR | O_CREAT,0666);


    //check fi any error in creating a shared memory block
    //shm_open will return -1 if errors occured
     if (fd <0 )
     {
        printf("Error creating shared memory block");

        exit(1);
        
     }

     //truncate file to set size
     ftruncate(fd, SHM_SIZE);

     //map to gShmPtr
     //addr, size, ptro, flags, fd, offset
     gShmPtr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE , MAP_SHARED, fd, 0);

     //check for errors
     if(gShmPtr == MAP_FAILED)
     {
         printf("mmap error. Exited");
        exit(1);
     }

    // Write code here to set the values of the four integers in the header
    // Just call the functions provided below, like this
    SetBufSize(bufSize); 
    SetItemCnt(itemCnt);	
    SetIn(0);
    SetOut(0);
	   
}

void Producer(int bufSize, int itemCnt, int randSeed)
{
        
    srand(randSeed);


    // Use the functions provided below to get/set the values of shared variables "in" and "out"
    int in = GetIn();
    int out = GetOut();


    //counter var 
    int i = 0;
    int randNum = 0;

//only producing itemCnt of items
    while(i < itemCnt)
    {
        // Write code here to produce itemCnt integer values in the range [0-3000]
        randNum  = GetRand(0,3000);

        //buffer is full
        while(((in+1) % bufSize) == GetOut())
        {
                //do nothing
        }

        // Use the provided function WriteAtBufIndex() to write into the bounded buffer 	
        WriteAtBufIndex(in, randNum);

        
        // Use the following print statement to report the production of an item:
        printf("Producing Item %d with value %d at Index %d\n", i, randNum, in);
        
        //increment variables. set it on shared buffer 
        in = (in +1 ) % bufSize;
        SetIn(in);

        i +=1;
        
    }


    
     printf("Producer Completed\n");
}

// Set the value of shared variable "bufSize"
void SetBufSize(int val)
{
        SetHeaderVal(0, val);
}

// Set the value of shared variable "itemCnt"
void SetItemCnt(int val)
{
        SetHeaderVal(1, val);
}

// Set the value of shared variable "in"
void SetIn(int val)
{
        SetHeaderVal(2, val);
}

// Set the value of shared variable "out"
void SetOut(int val)
{
        SetHeaderVal(3, val);
}

// Get the ith value in the header
int GetHeaderVal(int i)
{
        int val;
        void* ptr = gShmPtr + i*sizeof(int);
        memcpy(&val, ptr, sizeof(int));
        return val;
}

// Set the ith value in the header
void SetHeaderVal(int i, int val)
{
       // Write the implementation
       // (int*) dereferences it. its caseted to integer pointer
       if(gShmPtr == NULL)
       {
           printf("Shared memory has not been created yet. Cannot set header value");

        }
       else
       {

           int *int_array = (int*) gShmPtr;
           int_array[i] = val;
       }
       

}

// Get the value of shared variable "bufSize"
int GetBufSize()
{       
        return GetHeaderVal(0);
}

// Get the value of shared variable "itemCnt"
int GetItemCnt()
{
        return GetHeaderVal(1);
}

// Get the value of shared variable "in"
int GetIn()
{
        return GetHeaderVal(2);
}

// Get the value of shared variable "out"
int GetOut()
{             
        return GetHeaderVal(3);
}


// Write the given val at the given index in the bounded buffer 
void WriteAtBufIndex(int indx, int val)
{
        // Skip the four-integer header and go to the given index 
        void* ptr = gShmPtr + 4*sizeof(int) + indx*sizeof(int);
	memcpy(ptr, &val, sizeof(int));
}

// Read the val at the given index in the bounded buffer
int ReadAtBufIndex(int indx)
{
    //array of pointers of type int
        int* int_array = (int*) gShmPtr; //+ 4*sizeof(int) + indx*sizeof(int);
        return (int_array[indx + 4]);
 
}

// Get a random number in the range [x, y]
int GetRand(int x, int y)
{
	int r = rand();
	r = x + r % (y-x+1);
        return r;
}
