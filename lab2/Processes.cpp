#include "Processes.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <queue>

  //constructor
  Processes::Processes()
  {
    this->setRemainingBurst(getBurstTime());
  }
  Processes::Processes(int pid, int arrivalTime, int burstTime, int priority, int waitTime)
  {
    this->pid = pid;
    this->arrivalTime = arrivalTime;
    this->burstTime = burstTime;
    this->priority = priority;
    this->waitTime = 0; //all processes automatically have no wait until, we found depending on algorithm executed
  }

  //getters
  int Processes::getPid()
  {
    return pid;
  }
  int Processes::getArrivalTime()
  {
    return arrivalTime;
  }
  int Processes::getBurstTime()
  {
    return burstTime;
  }
  int Processes::getPriority()
  {
    return priority;
  }

  int Processes::getWaitTime()
  {
    return waitTime;
  }

  int Processes::getRemainingBurst()
  {
    return remainingBurst;
  }


  //setters
  void Processes::setPid(int pid)
  {
    this->pid = pid;
  }
  void Processes::setArrivalTime(int arrivalTime)
  {
    this->arrivalTime = arrivalTime;
  }
  void Processes::setBurstTime(int burstTime)
  {
    this->burstTime = burstTime;
  }
  void Processes::setPriority(int priority)
  {
    this->priority = priority;
  }
  void Processes::setWaitTime(int waitTime)
  {
    this->waitTime = waitTime;
  }
  void Processes::setRemainingBurst(int remBurst)
  {
    this->remainingBurst = remBurst;
  }



// Function to compare two rows based on the third column. Sorts by assensding order
bool Processes::compareArrivalTime(Processes& pro1, Processes& pro2)

{

  return pro1.getArrivalTime() < pro2.getArrivalTime();
  /*
    std::sort(arr.begin(), arr.end(), [col](const std::vector<int>& a, const std::vector<int>& b) 
        {
            // Check if the specified column exists in both rows
            if (col < a.size() && col < b.size()) {
                return a[col] < b[col]; // Sort based on the specified column
            }
            return false; // In case of column index out of bounds, do not swap
        }
    );
  */
}

bool Processes::compareBurstTime(Processes& pro1, Processes& pro2)
{
  return pro1.getBurstTime() > pro2.getBurstTime(); //gets descending order
}

//returns true if p1 has higher priority(lower number in UNIX)
bool Processes::comparePriorities(Processes& pro1, Processes& pro2)
{
  return pro1.getPriority() > pro2.getPriority();
}

void Processes::printProccess()
{

  cout << getPid() << " " << getArrivalTime() << " " << getBurstTime() <<" " << getPriority() << endl;

}


