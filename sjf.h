#ifndef SJF_H
#define SJF_H

#include <QWidget>
#include <Sched.h>
void OpenSjfWindow();
namespace Ui {
    class sjf;
}

class sjf : public QWidget
{
    Q_OBJECT

public:
    explicit sjf(QWidget *parent = 0);
    ~sjf();

private:
    Ui::sjf *ui;
    Ui::sjf *gUi;
    void closeEvent(QCloseEvent *);
    void ClearFCFSAll();
    int iBurstIndex;
    int iArrivalIndex;
    int iRemovalIndex;
    bool bCPUBusy;
    bool bSjfStart;
    bool bSjfPause;
    bool bSjfReset;
    int iProcInRQ;
    int iPreemptiveness;
    int iProcessInSystem;
    QTimer *Timer;
    void DeleteTimer();
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
    void FillDisplayRQ(ProcessDef);
    void CreateTimer();
    void DisplayMessage(int);
    void DisableWidgetsWhenStart();
    void EnableWidgetWhenStop();
    void DisableWidgetsBeforeProcessCreation();
    void EnableWidgetAfterProcessCreation();

private slots:
    void on_Preemptiveness_highlighted(int index);
    void CallDispatcher();
    void on_ResetSim_clicked();
    void on_StepSim_clicked();
    void on_PauseSim_clicked();
    void on_StopSim_clicked();
    void on_StartSim_clicked();
    void on_ArrivalTime_textChanged(QString );
    void on_BurstTime_textChanged(QString );
    void on_ChangeAT_highlighted(int index);
    void on_ChangeBT_highlighted(int index);
    void on_CreateProcess_clicked();
};

#endif // SJF_H
