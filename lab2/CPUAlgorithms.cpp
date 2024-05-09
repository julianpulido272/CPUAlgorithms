#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Processes.h"
#include "CPUAlgorithms.h"
#include <deque>
#include <queue>

//constructors
CPUAlgorithms::CPUAlgorithms()
{
  timeQuantum = -1;
}
CPUAlgorithms::CPUAlgorithms(string alg)
{
  currentAlgorithm = alg;
  timeQuantum = -1;
}

//getters and setters
string CPUAlgorithms::getAlgorithm()
{
  return currentAlgorithm;
}


void CPUAlgorithms::setAlgorithm(string alg)
{
  this->currentAlgorithm = alg;
}
int CPUAlgorithms::getTimeQuantum()
{
  return timeQuantum;
}
void CPUAlgorithms::setTimeQuantum(int time)
{
  timeQuantum = time;
}

//printing a vector of process
//note that this is different from printing in proccess class. professor wants it print this way
void CPUAlgorithms::printResult(vector<Processes> &arr)
{

  //special case to print out a time quantum if its round robin. 
  //time quantum was automatically set to -1 if it does not use time quantum for its algorithm
  if(getTimeQuantum() ==-1)
  {
    cout <<"Printing Result From "<< getAlgorithm() << " Algorithm" << endl;
  }
  else
  {
    cout <<"Printing Result From "<< getAlgorithm() << " "<<getTimeQuantum()<<" Algorithm" << endl;

  }

  //print format will be:
  //[waitTime , ProccessID]
  cout<<"[Wait Time, ProcessID]"<<endl;
  for(int i =0; i < arr.size();i++)
  {
    cout<< arr[i].getWaitTime() << " " << arr[i].getPid() <<endl;
  } 
}

//prints the original process list in full
void CPUAlgorithms::printOriginalProcess(vector<Processes> &arr)
{
  cout <<"Printing Original Process"<<endl;
  cout<<"[Process number, Arrival Time, CPU burst time,Priority]"<<endl;
  for (int i =0; i < arr.size(); i++)
  {
    cout << arr[i].getPid() << " " << arr[i].getArrivalTime() << " " << arr[i].getBurstTime() <<" " << arr[i].getPriority() << endl;
  }
  cout <<"\n";


}

void CPUAlgorithms::printAvgWait(vector<Processes> &arr, int numOfProcesses)
{

  double output = static_cast<double>(sumWaitTime(arr))/numOfProcesses;
  cout<<"Average waiting time: " << output <<endl;
}

/*
  Function sums up wait time of processes
  Note that if a process is processes repeatedly, its first wait time is only accounted for 
  P1 , P1, P1, P1
  ^
  |
  Only counts this wait time
*/
int CPUAlgorithms::sumWaitTime(vector<Processes> &arr)
{
  int totalWaitTime =0;
  int prevPid = 0;
  
  //hashmap to see if item appeared or not. allows to get difference in wait time of same process
  unordered_map<int,int> pidCounts;

  for(int i=0; i < arr.size(); i++)
  {
    //we want to count waittime for everytime a process does not show up sequentially
    if(arr[i].getPid() != prevPid)
    {
      //if the process id is not in our hashmap, add it
      if(pidCounts.find(arr[i].getPid()) == pidCounts.end())
      {
        //add the process in our hashmap with the index its located at
        pidCounts[arr[i].getPid()] = i;
        totalWaitTime += arr[i].getWaitTime() - arr[i].getArrivalTime();
      }
      else
      {
        //the processID was previously bursted before (but not i -1). we need to account for difference it has waited specifically
        //subtract (current wait time) - (for the previous time it bursted, the following process wait time)
         int waitTimeProcessesAlrBursted = arr[i].getWaitTime() - arr[pidCounts[arr[i].getPid()] +1].getWaitTime();
         totalWaitTime += waitTimeProcessesAlrBursted;

         //incase it burst again in the future but not sequentially, reconfigure index
         pidCounts[arr[i].getPid()] = i;
      }


      //record previous process id
      prevPid = arr[i].getPid();

    }
  }

  return totalWaitTime;
}





//algorithms
/*
finds the algorithm and executes the process vector with it
*/
vector<Processes> CPUAlgorithms::executeProcess(vector<Processes> &arr)
{
  //execute the algorithm based on what it was
  if(getAlgorithm().find("RR") != string::npos)
  {

    //need to get quantum time from algorithm
    string num = getAlgorithm();
    num = num[num.length() -1];
    setTimeQuantum(stoi(num));


    setAlgorithm("RR");

    return(roundRobin(arr, getTimeQuantum()));
    

  }
  else if (getAlgorithm().find("SJF") != string::npos)
  {
    return SJF(arr);
  }
  else if (getAlgorithm().find("PR_noPREMP")!= string::npos)
  {
    return PR_noPREMP(arr);
  }
  else
  {
    return PR_withPREMP(arr);
  }
  
}

/*
  Round robin adds processes in FIFO queue if they have arrived. Executes each process (named currentProcess).
  Timer increments based on how long they bursted for.
  If processes have arrived <= timer, add them in ready queue
  If current process last longer than time quantum, it is placed at end of queue

  Output:
  [waiting time, process]
  ...
*/
vector<Processes> CPUAlgorithms::roundRobin(vector<Processes>& arr, int quantumTime)
{
  //output of vector is dynamic
  vector<Processes> output;

  //queue for processess
  queue<Processes> readyQueue;

  //our timer
  int timer =0;

  //outputIndex for our output
  int outputIndex =0;

  //for loop to add process to our queue
  int i =0;


    //add all processes in ready queue if the arrival time is less than timer
    if(readyQueue.size() < arr.size())
    {
      while(arr[i].getArrivalTime() <= timer)
      {
        readyQueue.push(arr[i]);
        i++;
      }
    }


  while(!readyQueue.empty())
  {

    Processes currentProcess = readyQueue.front();
    readyQueue.pop();


    //set the wait time before executing each process with a timer
    currentProcess.setWaitTime(timer);

    //process finishes within time quantum
    if(currentProcess.getRemainingBurst() <= quantumTime)
    {
      //increase timer by burst it took
      timer+= currentProcess.getRemainingBurst();

      //set it to zero since its done
      currentProcess.setRemainingBurst(0);

    }
    //process took longer than time quantum
    else
    {
      //add our timer by time quantum since thats how long it took
      timer+= quantumTime;

      //subtract remaining burst time by time quantum
      currentProcess.setRemainingBurst(currentProcess.getRemainingBurst() - quantumTime);
    }

    //timer changed; need to check which processes arrived then put them into queue first
    if(readyQueue.size()< arr.size())
    {
      while(arr[i].getArrivalTime() <= timer)
      {
        readyQueue.push(arr[i]);
        i++;
      }
    }

    //if theres still remaining burst time of current process, add it to back of queue
    if(currentProcess.getRemainingBurst() >0)
    {
      readyQueue.push(currentProcess);
    }

    //save that to output
    output.insert(output.begin() + outputIndex, currentProcess);
    
    output[outputIndex].setWaitTime(currentProcess.getWaitTime());
    output[outputIndex].setArrivalTime(currentProcess.getArrivalTime());
    output[outputIndex].setPid(currentProcess.getPid());
    
    //increment our index for our output
    outputIndex++;

    }
  return output;
}

/*
Shortest Job first prioritizes the shortest job of the priorities

Input: 2D vector in format. can have many rows but only 4 columns
[Process number, arrival time, CPU burst time, priority]

Output:
0, Process number //0 becaues it doesnt wait
Wait time, Process number
Wait time, Process number
...

*/
vector<Processes> CPUAlgorithms::SJF(vector<Processes>& arr)
{

  //first figure out which job arrives first. Sort by arrival time
  std::sort(arr.begin(), arr.end(), Processes::compareArrivalTime);

  //create output vector of processes that is same size of input array
  vector<Processes> output(arr.size());

  //Priority Queue: NEED to declare it like this. if you pass in the static function in the parameter, it will give an error
  auto comp = Processes::compareBurstTime; 
  //this creates a min heap of processes by burst time
  priority_queue<Processes, vector<Processes>, decltype(comp)> priorityQueue(comp);


  //need to add all priorities that have arrived in our queue

  //for loop to add process to our queue
  int i =0;
  int timer = 0;
  int outputIndex =0;


    //add all processes in ready queue if the arrival time is less than timer
    if(priorityQueue.size() < arr.size())
    {
      while(arr[i].getArrivalTime() <= timer)
      {
        priorityQueue.push(arr[i]);
        i++;
      }
    }
    while(!priorityQueue.empty())
    {
      //pop the top of the priority queue
      Processes currentProcess = priorityQueue.top();
      priorityQueue.pop();

      //save info to our output
      output[outputIndex].setPid(currentProcess.getPid());
      output[outputIndex].setArrivalTime(currentProcess.getArrivalTime());
      output[outputIndex].setWaitTime(timer);

      //increment out timer
      timer += currentProcess.getBurstTime();

      //change resulting burst time
      currentProcess.setBurstTime(0);


      //time changed, add any processes that have arrived
      if(priorityQueue.size() < arr.size())
      {
        while(arr[i].getArrivalTime() <= timer)
        {
          priorityQueue.push(arr[i]);
          cout << "test" <<  arr[i].getPid() << endl;
          i++;
        }
      }
      outputIndex++;
    }
    
  
  return output;
}



  /*
  Priority Scheduling without Preemption:
  Higher priority processes schedules first. Processes with same priority level executed in FCFS
  NOTE: low numbers indicate high priority

  Input: 2D vector in format. can have many rows but only 4 columns
  [Process number, arrival time, CPU burst time, priority]

  Output:
  0, Process number //0 becaues it doesnt wait
  Wait time, Process number
  Wait time, Process number
  ...
  */
vector<Processes> CPUAlgorithms::PR_noPREMP(vector<Processes>& arr)
{
    //execute processes who arrive earliest. keep cpu until finish. if two arrive at same time, execute whoever has higher priority

    //first figure out which job arrives first. Sort by arrival time
    std::sort(arr.begin(), arr.end(), Processes::compareArrivalTime);
    vector<Processes> output(arr.size());

    //priority queue for processes (minimum heap is used since lower number is higher priority)

    //NEED to declare it like this. if you pass in the static function in the parameter, it will give an error
    auto comp = Processes::comparePriorities; 
    priority_queue<Processes, vector<Processes>, decltype(comp)> priorityQueue(comp);

    
    //timer to keep track of waiting time
    int timer =0;
    int i =0; //for loop traveral
    int outputIndex =0;

    //add all processes in ready queue if the arrival time is less than timer
    for (; i < arr.size(); i++) 
    {
      if (arr[i].getArrivalTime() <= timer) 
      {
        priorityQueue.push(arr[i]);
      } 
      else 
      {
        // As soon as you encounter an element that doesn't meet the condition,
        break;
      }
    }

    while(!priorityQueue.empty())
    {
      Processes currentProcess = priorityQueue.top();
      priorityQueue.pop();

      //set the wait time before executing each process with a timer
      currentProcess.setWaitTime(timer);

      //set wait time for our output
      output[outputIndex].setWaitTime(currentProcess.getWaitTime());
      output[outputIndex].setPid(currentProcess.getPid());
      output[outputIndex].setArrivalTime(currentProcess.getArrivalTime());
      //increment timer by burst time
      timer += currentProcess.getBurstTime();

      //set the remaining burst time to zero
      currentProcess.setRemainingBurst(0);

      //fill up our priority queue since time changed
      for (; i < arr.size(); i++) 
      {
        if (arr[i].getArrivalTime() <= timer) 
        {
            priorityQueue.push(arr[i]);

        } 
        else 
        {
          // As soon as you encounter an element that doesn't meet the condition,
          // you break out of the loop, no need to increment i or print "test2".
          break;
        }

      }
      outputIndex++;
    }
    return output;

    
}


/*
Priority scheduling with preemption
Process with highest priority gets executed.
It will interrupt whatever is currently running to get executed
*/
vector<Processes> CPUAlgorithms::PR_withPREMP(vector<Processes>& arr)
{
    //first figure out which job arrives first. Sort by arrival time
    std::sort(arr.begin(), arr.end(), Processes::compareArrivalTime);
    vector<Processes> output;

    //priority queue for processes (minimum heap is used since lower number is higher priority)

    //NEED to declare it like this. if you pass in the static function in the parameter, it will give an error
    auto comp = Processes::comparePriorities; 
    priority_queue<Processes, vector<Processes>, decltype(comp)> priorityQueue(comp);

    
    //timer to keep track of waiting time
    int timer =0;
    int processed =0; //for loop traveral
    int outputIndex =0;
    int prevPid = -1;

    //add each process into our priority queue if it has arrived
    for(processed; processed < arr.size();processed++)
    {
      if(arr[processed].getArrivalTime() <= timer)
      {
        priorityQueue.push(arr[processed]);
      }
      else{
        break;
      }
    }

    while(!priorityQueue.empty())
    {
      Processes currentProcess = priorityQueue.top();
      priorityQueue.pop();

      //our index only changes if it differs from the previous pid
      if(currentProcess.getPid()!=prevPid)
      {
        prevPid = currentProcess.getPid();
        
        //insert the process to our output
        output.insert(output.begin() + outputIndex,currentProcess);
        //set the wait time and process ID to output
        currentProcess.setWaitTime(timer);
        output[outputIndex].setWaitTime(currentProcess.getWaitTime());
        output[outputIndex].setPid(currentProcess.getPid());
        output[outputIndex].setArrivalTime(currentProcess.getArrivalTime());

        //increment outputIndex
        outputIndex++;
      }


      //increment time
      timer+=1;

      //remaining burst decrements by one
      currentProcess.setRemainingBurst(currentProcess.getRemainingBurst()-1);

      //if our current process is not done executing, add it back to queue
      if(currentProcess.getRemainingBurst() !=0)
      {
        priorityQueue.push(currentProcess);
      }

      //since time increase, we need to check if theres more processes that arrived
      for(; processed < arr.size();processed++)
      {
        if(arr[processed].getArrivalTime() <= timer)
        {
          priorityQueue.push(arr[processed]);

        }
        else{
          break;
        }
      }
    }

    return output;
}