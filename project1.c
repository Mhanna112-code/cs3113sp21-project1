#include <stdio.h>

//struct to store all processes
struct Process{
    int ID;
    int runTime;
    int priority;
    int lastIdxFlag;
    int firstIdxFlag;
}processArr[40000];


//finds last index of given process ID
int findIndex(struct Process arr[], int idx, int K)
{
    if (idx < 0)
        return -1;

    //compares Process ID at index with given process ID argument
    if (arr[idx].ID == K) {
        //return last index
        return idx;
    }

    //recursively call to find last index
    return findIndex(arr, idx - 1, K);
}

//counts number of unique processes
int countNumDistinctElements(struct Process arr[], int len)
{
    //invalid array length argument
    if (len <= 0) return 0;
    int unique = 1;

    //picks test cases
    for (int outer = 1; outer < len; ++outer)
    {
        //checks if test cases are unique
        int is_unique = 1;
        for (int inner = 0; is_unique && inner < outer; ++inner)
        {
            if (arr[inner].ID == arr[outer].ID) is_unique = 0;
        }
        if (is_unique) ++unique;
    }
    //return number of unique processes
    return unique;
}

int nProcesses, nExecutionElements, nInstructions;

int main(int argc, char **argv) {
    //opens file from command line argument
    FILE *f = fopen(argv[1], "r");
    int contextSwitches = 0;
    //scans command line arguments to defined int variables
    fscanf(f, "%d", &nProcesses);
    fscanf(f, "%d", &nExecutionElements);
    fscanf(f, "%d", &nInstructions);
    //set process flags to 0
    for (int i = 0; i < nInstructions; i++) {
        processArr[i].lastIdxFlag = 0;
        processArr[i].firstIdxFlag = 0;
    }
    float throughPut;
    float waitingTime;
    float avgResTime = 0;
    //scans in processes and stores process info in array
    for (int i = 0; i < nInstructions; i++) {
        fscanf(f, "%d", &processArr[i].ID);
        fscanf(f, "%d", &processArr[i].runTime);
        fscanf(f, "%d", &processArr[i].priority);
    }
    //calculates number of context switches
    for (int i = 0; i < nInstructions; i++) {
        if (processArr[i].ID != processArr[i + 1].ID)
            contextSwitches++;
    }
    //calculates number of voluntary context switches
    int numVol = countNumDistinctElements(processArr, nInstructions);
    //calculates number of non-voluntary context switches
    int numNonVol = contextSwitches - numVol;
    //CPU utilization is always 100
    float cpuUtil = 100.00;
    float avgTurnAroundTime;
    int totalBurstTime = 0;
    float totalRunTime = 0;
    int lastIdx;
    float currTurnAroundTime = 0;

    //calculates numerator of average response time
    for (int k = 0; k < nInstructions; k++) {
        //checks if process flag is turned off
        if (processArr[k].firstIdxFlag != 1) {
            //calculates Turnaround and total burst time
            for (int j = 0; j <= k; j++) {
                currTurnAroundTime += processArr[j].runTime;
                if (processArr[j].ID == processArr[k].ID)
                    totalBurstTime += processArr[j].runTime;
            }
            //turns on process flags for the same processes
            for (int u = k; u < nInstructions; u++) {
                if (processArr[u].ID == processArr[k].ID)
                    processArr[u].firstIdxFlag = 1;
            }
            //adds the difference of Turnaround time and total burst time for each process
            avgResTime += currTurnAroundTime - totalBurstTime;
            totalBurstTime = 0;
            currTurnAroundTime = 0;
        }
        continue;
    }

    float prevRunTime;
    //calculates numerator of average waiting time
    for (int k = 0; k < nInstructions; k++) {
        //calculates total run time for all processes
        totalRunTime += processArr[k].runTime;
        //calculates last index of each process
        lastIdx = findIndex(processArr, nInstructions - 1, processArr[k].ID);
        //checks if process flag is turned off
        if (processArr[k].lastIdxFlag != 1){
            for (int j = 0; j <= lastIdx; j++) {
                //calculates numerator of current and average turnaround time
                currTurnAroundTime += processArr[j].runTime;
            }
            for (int j = 0; j < lastIdx; j++) {
                prevRunTime += processArr[j].runTime;
            }
            //calculates total burst time for each process
            for (int u = 0; u <= lastIdx; u++) {
                if (processArr[u].ID == processArr[lastIdx].ID) {
                    totalBurstTime += processArr[u].runTime;
                    //turns process flag on
                    processArr[u].lastIdxFlag = 1;
                }
            }
            //adds the difference of Turnaround time and total burst time for each process
            avgTurnAroundTime += currTurnAroundTime - totalBurstTime;
            totalBurstTime = 0;
            currTurnAroundTime = 0;
        }
        continue;
    }

    //divides numerator of turnaround time by number of unique processes to determine the average turnaround time
    avgTurnAroundTime /= countNumDistinctElements(processArr, nInstructions);
    //divides numerator of waiting time by number of unique processes to determine the average turnaround time
    waitingTime = prevRunTime / countNumDistinctElements(processArr, nInstructions);
    //divides numerator of response time by number of unique processes to determine the average turnaround time
    avgResTime /= countNumDistinctElements(processArr, nInstructions);
    //divides number of unique processes by total run time to calculate throughput
    throughPut = countNumDistinctElements(processArr, nInstructions) / totalRunTime;
    printf("%d\n", numVol);
    printf("%d\n", numNonVol);
    printf("%.2f\n", cpuUtil);
    printf("%.2f\n", throughPut);
    printf("%.2f\n", avgTurnAroundTime);
    printf("%.2f\n", waitingTime);
    printf("%.2f\n", avgResTime);
    return 0;
}
