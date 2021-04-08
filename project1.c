#include <stdio.h>
#include <cstdlib>

struct Process{
    int ID;
    int runTime;
    int priority;
    int lastIdxFlag = 0;
    int flag2 = 0;
    int flag3 = 0;
    int lastIdx = 0;
}processArr[40000];

int findIndex(Process arr[], int idx, int K)
{
    if (idx < 0)
        return -1;

    if (arr[idx].ID == K) {
        return idx;
    }

    // Recursive Call
    return findIndex(arr, idx - 1, K);
}

int countNumDistinctElements(struct Process arr[], int n)
{
    int numVol = 1;

    // Pick all elements one by one
    for (int i = 1; i < n; i++) {
        int j = 0;
        for (j = 0; j < i; j++)
            if (arr[i].ID == arr[j].ID)
                break;

        // If not printed earlier, then print it
        if (i == j)
            numVol++;
    }
    return numVol;
}

int nProcesses, nExecutionElements, nInstructions;

int main(int argc, char **argv) {
    FILE *f = fopen(argv[1], "r");
    int arrLength = sizeof(processArr) / sizeof(processArr[0]);
    for (int i = 0; i < arrLength; i++) {
        processArr[i].lastIdxFlag = 0;
        processArr[i].flag2 = 0;
        processArr[i].flag3 = 0;
        processArr[i].lastIdx = 0;
    }
    int contextSwitches = 0;
    fscanf(f, "%d", &nProcesses);
    fscanf(f, "%d", &nExecutionElements);
    fscanf(f, "%d", &nInstructions);
    float throughPut;
    float waitingTime;
    float avgResTime = 0;
    for (int i = 0; i < nInstructions; i++) {
        fscanf(f, "%d", &processArr[i].ID);
        fscanf(f, "%d", &processArr[i].runTime);
        fscanf(f, "%d", &processArr[i].priority);
        if (i == 0) {
            if (processArr[i].ID != processArr[i + 1].ID)
                contextSwitches++;
        }
        else if (i > 0 && i < nInstructions - 1)
            if ((processArr[i].ID != processArr[i + 1].ID) && (processArr[i].ID != processArr[i + 1].ID))
                contextSwitches++;
        else
            if (processArr[i].ID != processArr[i - 1].ID)
                contextSwitches++;
    }
    float numVol = countNumDistinctElements(processArr,nInstructions);
    int numNonVol = contextSwitches - numVol;
    float cpuUtil = 100.00;
    float avgTurnAroundTime;
    int totalBurstTime = 0;
    float totalRunTime = 0;
    int lastIdx;
    int currTurnAroundTime = 0;
    for (int k = 0; k < nInstructions; k++) {
        totalRunTime += processArr[k].runTime;
        lastIdx = findIndex(processArr, arrLength - 1, processArr[k].ID);
        if (processArr[k].lastIdxFlag != 1){
            for (int j = 0; j <= lastIdx; j++) {
                currTurnAroundTime += processArr[j].runTime;
                avgTurnAroundTime += processArr[j].runTime;
            }
            for (int u = 0; u <= lastIdx; u++) {
                if (processArr[u].ID == processArr[lastIdx].ID)
                    totalBurstTime += processArr[u].runTime;
            }
                waitingTime += currTurnAroundTime - totalBurstTime;
                totalBurstTime = 0;
                currTurnAroundTime = 0;
                processArr[lastIdx].lastIdxFlag = 1;
        }
    }
    avgTurnAroundTime /= numVol;
    waitingTime /= numVol;
    throughPut = numVol / totalRunTime;
    printf("%d\n", numVol);
    printf("%d\n", numNonVol);
    printf("%.2f\n", cpuUtil);
    printf("%.2f\n", throughPut);
    printf("%.2f\n", avgTurnAroundTime);
    printf("%.2f\n", waitingTime);
    printf("%.2f\n", avgResTime);
    return 0;
}
