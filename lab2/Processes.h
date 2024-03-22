#ifndef PROCESSES_H
#define PROCESSES_H


#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

class Processes
{
  private:
    int pid;
    int arrivalTime;
    int burstTime; 
    int remainingBurst;//remaining burst is initially given by inital burst time
    int priority;
    int waitTime =0; //initially, all processes have not waited anything yet
  public:

    //constructor
    Processes();
    Processes(int pid, int arrivalTime, int burstTime, int priority, int waitTime);

    //getters
    int getPid();

    int getArrivalTime();

    int getBurstTime();

    int getPriority();

    int getWaitTime();

    int getRemainingBurst();

    //setters
    void setPid(int pid);

    void setArrivalTime(int arrivalTime);

    void setBurstTime(int burstTime);

    void setPriority(int priority);

    void setWaitTime(int waitTime);

    void setRemainingBurst(int remBurst);
    

    //algorithms
    void printProccess();

    static bool compareArrivalTime(Processes& pro1, Processes& pro2);

    static bool compareBurstTime(Processes& pro1, Processes& pro2);

    static bool comparePriorities(Processes& pro1, Processes& pro2);




    

};
#endif