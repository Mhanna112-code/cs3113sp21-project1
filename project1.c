#include <stdio.h>

struct Process{
    int ID;
    int runTime;
    int priority;
    int flag1;
    int flag2;
    int flag3;
    int lastIdx;
}processArr[2000];

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
int nProcesses, nExecutionElements, nInstructions;

int main(int argc, char **argv) {
    FILE *f = fopen(argv[1], "r");
    int arrLength = sizeof(processArr) / sizeof(processArr[0]);
    for (int k = 0; k < arrLength; k++){
        processArr[k].flag1 = 0;
        processArr[k].flag2 = 0;
        processArr[k].flag3 = 0;
        processArr[k].lastIdx = 0;

    }
    int flag = 0;
    int nonVol = 0;
    int vol = 0;
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
        if (i != 0) {
            for (int j = 0; j < nInstructions; j++) {
                if (processArr[j].ID == processArr[i].ID && j != i && j != i - 1 && j != i + 1 && processArr->flag1 != 1) {
                    flag = 1;
                    nonVol++;
                    processArr[i].ID = 1;
                }
                if (flag == 0)
                    vol++;
                flag = 1;
            }
            flag = 0;
        }
    }
    float cpuUtil = 100.00;
    float throughPutNum = nExecutionElements;
    float throughPutDenom = 0;
    float avgResTimeNum = 0;
    float avgResTimeDenum = nExecutionElements;
    float avgTurnAroundTimeNum = 0;
    float avgTurnAroundTimeDenum  = nExecutionElements;
    float avgTurnAroundTime;
    for (int k = 0; k < nInstructions; k++) {
        throughPutDenom += processArr[k].runTime;
        if (processArr[k].priority != 1)
            waitingTime += processArr[k].priority;
        if (k == 0)
            avgResTimeNum += 1;
        else
            avgResTimeNum += processArr[k - 1].runTime + 1;
        processArr[k].lastIdx = findIndex(processArr, arrLength-1, processArr[k].ID);
    }
    for (int b = 0; b < nInstructions; b++) {
            if (processArr[processArr[b].lastIdx].flag3 != 1) {
                for (int c = 0; c <= processArr[b].lastIdx; c++)
                    avgTurnAroundTimeNum += processArr[c].runTime;
                processArr[processArr[b].lastIdx].flag3 = 1;
            }
    }
    throughPut = throughPutNum/throughPutDenom;
    waitingTime /= nExecutionElements;
    avgResTime = avgResTimeNum/avgResTimeDenum;
    avgTurnAroundTime = avgTurnAroundTimeNum/avgTurnAroundTimeDenum;
    printf("%d\n", vol);
    printf("%d\n", nonVol);
    printf("%.2f\n", cpuUtil);
    printf("%.2f\n", throughPut);
    printf("%.2f\n", avgTurnAroundTime);
    printf("%.2f\n", waitingTime);
    printf("%.2f\n", avgResTime);
    return 0;
}
                               
