#ifndef PRORITY_H
#define PRORITY_H

#include <QWidget>
#include <Sched.h>
void OpenPriorityWIndow();
namespace Ui {
    class prority;
}

class prority : public QWidget
{
    Q_OBJECT

public:
    explicit prority(QWidget *parent = 0);
    ~prority();

private:
    Ui::prority *ui;
    Ui::prority *gUi;
    void closeEvent(QCloseEvent *);
    void ClearFCFSAll();
    int iBurstIndex;
    int iArrivalIndex;
    int iRemovalIndex;
    int iPriority;
    bool bCPUBusy;
    bool bProStart;
    bool bProPause;
    bool bProReset;
    int iProcInRQ;
    int iPreemptiveness;
    bool bPreempt;
    int iProcessInSystem;
    QTimer *Timer;
    void DeleteTimer();
    void ClearProcessLabels();
    void ClearBTLabels();
    void ClearATLabels();
    void ClearProLabels();
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
    void DisableWidgetsWhenStart();
    void EnableWidgetWhenStop();
    void DisplayMessage(int);
    void DisableWidgetsBeforeProcessCreation();
    void EnableWidgetAfterProcessCreation();

private slots:
    void on_Prority_textChanged(QString );
    void on_ChangePro_highlighted(int index);
    void CallDispatcher();
    void on_ResetSim_clicked();
    void on_StepSim_clicked();
    void on_PauseSim_clicked();
    void on_StartSim_clicked();
    void on_ArrivalTime_textChanged(QString );
    void on_BurstTime_textChanged(QString );
    void on_Preemptiveness_highlighted(int index);
    void on_ChangeAT_highlighted(int index);
    void on_ChangeBT_highlighted(int index);
    void on_CreateProcess_clicked();
};

#endif // PRORITY_H
