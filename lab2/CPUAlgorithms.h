#ifndef CPUALGORITHMS_H
#define CPUALGORITHMS_H


#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "Processes.h"

class CPUAlgorithms
{
  private:
    string currentAlgorithm;
    int timeQuantum; //optional variable in the case of round robin
  public:
    CPUAlgorithms();
    CPUAlgorithms(string alg);
    string getAlgorithm();
    void setAlgorithm(string alg);
    int getTimeQuantum();
    void setTimeQuantum(int time);
    void printResult(vector<Processes> &arr);
    void printOriginalProcess(vector<Processes> &arr);
    void printAvgWait(vector<Processes> &arr, int numOfProcesses);
    
    int sumWaitTime(vector<Processes> &arr);
    vector<Processes> executeProcess(vector<Processes> &arr);
    vector<Processes> roundRobin(vector<Processes>& arr, int quantumTime);
    vector<Processes> SJF(vector<Processes>& arr);
    vector<Processes> PR_noPREMP(vector<Processes>& arr);
    vector<Processes> PR_withPREMP(vector<Processes>& arr);
};
#endif