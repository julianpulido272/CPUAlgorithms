#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "Processes.h"
#include "CPUAlgorithms.h"
#include <deque>

using namespace std;

/*
Julian Pulido
CSC-139 03
Lab2: CPU Scheduling

Algorithm
integer - number of processes
one line per process
*/


bool fileExists(string& fileName);
vector<Processes> getProcessesList(string& fileName , CPUAlgorithms& alg);

//main function
int main(int argc, char*argv[]) 
{    
    //error check: make sure it accepts at least one file
    if (argc != 2)
    {
        cout<< "Needs to accept only one input argument" << endl;
    }



    //accept command line args
    string filename = argv[1];

    //create Algorithm object to call algorithm on processes
    CPUAlgorithms algs;

    //create vector of processes from file
    vector<Processes> processes = getProcessesList(filename, algs);

    algs.printOriginalProcess(processes);

    vector<Processes> output = algs.executeProcess(processes);

    algs.printResult(output);
    algs.printAvgWait(output);

    return 0;
}

//boolean to check if file exists
bool fileExists(string& fileName)
{
  std::ifstream file (fileName);
  return file.is_open();
}

//create vector of processes from file
vector<Processes> getProcessesList(string& fileName, CPUAlgorithms& alg)
{
    //check if file exists
    if(!fileExists(fileName))
    {
        cout<< "File does not exists";
        exit(1);
    }

    //ifstream is the class to read from files
    //create file from the filename given
    ifstream file (fileName);

    //read file contents
    int size = 4; 
    string line;
    int counter =0;

    //loop through the first two lines first to establish arraysize and algorithm type before searching through rest of file
    for (int i =0; i < 2; i++)
    {
        getline(file, line);        
        if(i ==0)
        {
            //first line will be algorithm
            alg.setAlgorithm(line);
            counter +=1;
        }
        else
        {
            //second line will be number of processes

            //converts the string to int
            size = stoi(line);
            counter +=1;
        }

    }
    //create array of size*4. Hardcoded because typically our input is 4 numbers in one line
    //vector<vector<int>> processes(size, vector<int>(4));
    vector<Processes> processes(size);



     for (int i =0; i < processes.size(); i++)
     {   
        //third line and beyond will be processes list     
        getline(file, line);    
            
        //use stringstream to split the line based on spaces
        stringstream ss(line);

        int num;
        for (int j =0; j < 4; j ++)
        {
            //parse the strings from the stream into the num
            ss >> num;

            if(j==0)
            {
                processes[i].setPid(num);
            }
            if(j==1)
            {
                processes[i].setArrivalTime(num);
            }
            if(j==2)
            {
                processes[i].setBurstTime(num);
                processes[i].setRemainingBurst(num);

            }
            if(j==3)
            {
                processes[i].setPriority(num);
            }
        }
     }

    return processes;
}


