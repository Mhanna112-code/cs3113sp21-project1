#include <stdio.h>

struct Process{
    int ID;
    int runTime;
    int priority;
    int lastIdxFlag;
    int volSwitch;
    int firstIdxFlag;
}processArr[40000];

int findIndex(struct Process arr[], int idx, int K)
{
    if (idx < 0)
        return -1;

    if (arr[idx].ID == K) {
        return idx;
    }

    // Recursive Call
    return findIndex(arr, idx - 1, K);
}

int countNumDistinctElements(struct Process arr[], int len)
{
    if (len <= 0) return 0;
    int unique = 1;

    for (int outer = 1; outer < len; ++outer)
    {
        int is_unique = 1;
        for (int inner = 0; is_unique && inner < outer; ++inner)
        {
            if (arr[inner].ID == arr[outer].ID) is_unique = 0;
        }
        if (is_unique) ++unique;
    }
    return unique;
}

int nProcesses, nExecutionElements, nInstructions;

int main(int argc, char **argv) {
    FILE *f = fopen(argv[1], "r");
    int contextSwitches = 0;
    fscanf(f, "%d", &nProcesses);
    fscanf(f, "%d", &nExecutionElements);
    fscanf(f, "%d", &nInstructions);
    for (int i = 0; i < nInstructions; i++) {
        processArr[i].lastIdxFlag = 0;
        processArr[i].volSwitch = 0;
        processArr[i].firstIdxFlag = 0;
    }
    float throughPut;
    float waitingTime;
    float avgResTime = 0;
    for (int i = 0; i < nInstructions; i++) {
        fscanf(f, "%d", &processArr[i].ID);
        fscanf(f, "%d", &processArr[i].runTime);
        fscanf(f, "%d", &processArr[i].priority);
    }
    for (int i = 0; i < nInstructions; i++) {
        if (processArr[i].ID != processArr[i + 1].ID)
            contextSwitches++;
    }
    int numVol = countNumDistinctElements(processArr, nInstructions);
    int numNonVol = contextSwitches - numVol;
    float cpuUtil = 100.00;
    float avgTurnAroundTime;
    int totalBurstTime = 0;
    float totalRunTime = 0;
    int lastIdx;
    int currTurnAroundTime = 0;
    for (int k = 0; k < nInstructions; k++) {
        if (processArr[k].firstIdxFlag != 1) {
            for (int j = 0; j <= k; j++) {
                currTurnAroundTime += processArr[j].runTime;
            }
            for (int u = 0; u < nInstructions; u++) {
                if (processArr[u].ID == processArr[k].ID) {
                    processArr[u].firstIdxFlag = 1;
                    totalBurstTime += processArr[u].runTime;
                }
            }
            avgResTime += currTurnAroundTime - totalBurstTime;
            totalBurstTime = 0;
            currTurnAroundTime = 0;
        }
    }

    for (int k = 0; k < nInstructions; k++) {
        totalRunTime += processArr[k].runTime;
        lastIdx = findIndex(processArr, nInstructions - 1, processArr[k].ID);
        if (processArr[k].lastIdxFlag != 1){
            for (int j = 0; j <= lastIdx; j++) {
                currTurnAroundTime += processArr[j].runTime;
                avgTurnAroundTime += processArr[j].runTime;
            }
            for (int u = 0; u <= lastIdx; u++) {
                if (processArr[u].ID == processArr[lastIdx].ID) {
                    totalBurstTime += processArr[u].runTime;
                    processArr[u].lastIdxFlag = 1;
                }
            }
            waitingTime += currTurnAroundTime - totalBurstTime;
            totalBurstTime = 0;
            currTurnAroundTime = 0;
        }
    }

    avgTurnAroundTime /= countNumDistinctElements(processArr, nInstructions);
    waitingTime /= countNumDistinctElements(processArr, nInstructions);
    avgResTime /= countNumDistinctElements(processArr, nInstructions);
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
