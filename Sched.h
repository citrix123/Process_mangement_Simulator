#ifndef SCHED_H
#define SCHED_H

#define MAXIMUM_PROC 20

typedef class Process{
public:
    char cProcessName[20];
    int iBurstTime;
    int iProcessID;
    int iArrivalTime;
    int iPriority;
    bool bFill;
    bool bInReadyQ;
    bool bInCpu;
    bool bInJobQ;
}ProcessDef;
typedef class DispProcInReadyQueue{
public:
    char cProc[20];
    bool bFill;
    int iProcessID;
    int iBurst;
    int iPriority;
}DispProc;

enum Preemptiveness{
    NONPREEMPTIVE,
    PREEMPTIVE
};
void Sorting(DispProc *,int,int);
void UpdateDisplayRQAtEveryBeat(DispProc *proc,ProcessDef data);
int GenRendTime();
void FillProcessInfo(ProcessDef *ProcessInfo);
#endif // SCHED_H
