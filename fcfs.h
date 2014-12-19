#ifndef FCFS_H
#define FCFS_H

#include <QWidget>
#include <QCloseEvent>
#include <Sched.h>
void OpenFCFSWindow();
namespace Ui {
    class FCFS;
}

class FCFS : public QWidget
{
    Q_OBJECT

public:
    explicit FCFS(QWidget *parent = 0);
    ~FCFS();
    int iTypeOFScheduling;

private:
    Ui::FCFS *ui;
    Ui::FCFS *gUi;
    void closeEvent(QCloseEvent *);
    void ClearFCFSAll();
    int iBurstIndex;
    int iArrivalIndex;
    int iRemovalIndex;
    bool bCPUBusy;
    int iProcInRQ;
    bool bFcfsStart;
    bool bFcfsPause;
    bool bFcfsReset;
    int iProcessInSystem;
    QTimer *Timer;
    void DeleteTimer();
    void CreateTimer();
    void ClearProcessLabels();
    void ClearBTLabels();
    void ClearATLabels();
    void ClearRQLabels();
    void ClearCPULabels();
    void ClearBTTimeLabel();
    void ClearAllStructure();
    void ClearStructureForStop();
    void ResetSimulator();
    void ProcessDisplayOnHMi(ProcessDef *);
    void UpdateColourWhenInRQ();
    void UpdateColourWhenInCPU();
    void UpdateColourWhenInNormal();
    void PutIntoTheCPU();
    void Dispatcher();
    void ClearLabelColour();
    void UpdateDisplayRQ();
    void UpdateReadyQueue();
    void FillDisplayRQ(char *proc,int iProcID);
    void DisplayMessage(int);
    void DisableWidgetsWhenStart();
    void EnableWidgetWhenStop();
    void DisableWidgetsBeforeProcessCreation();
    void EnableWidgetAfterProcessCreation();

private slots:
    void on_PauseSim_clicked();
    void CallDispatcher();
    void on_ResetSim_clicked();
    void on_StepSim_clicked();
    void on_StartSim_clicked();
    void on_ArrivalTime_textChanged(QString );
    void on_ChangeAT_highlighted(int index);
    void on_ChangeBT_highlighted(int index);
    void on_BurstTime_textChanged(QString );
    void on_CreateProcess_clicked();
};

#endif // FCFS_H
