#ifndef RR_H
#define RR_H

#include <QWidget>
#include <Sched.h>
void OpenRRWindow();

namespace Ui {
    class rr;
}

class rr : public QWidget
{
    Q_OBJECT

public:
    explicit rr(QWidget *parent = 0);
    ~rr();

private:
    Ui::rr *ui;
    Ui::rr *gUi;
    void closeEvent(QCloseEvent *);
    void ClearFCFSAll();
    int iBurstIndex;
    int iArrivalIndex;
    int iTimeQuantum;
    int iTempTimeQuantum;
    bool bCPUBusy;
    int iProcInRQ;
    bool bRRStart;
    bool bRRPause;
    bool bRRReset;
    int iProcessInSystem;
    QTimer *Timer;
    QList<int> l;
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
    void FillDisplayRQ(ProcessDef);
    void DisplayMessage(int);
    void BurstTimeDisplay(int);
    bool CheckQValueBeforeStart();
    void DisableWidgetsWhenStart();
    void EnableWidgetWhenStop();
    void DisableWidgetsBeforeProcessCreation();
    void EnableWidgetAfterProcessCreation();

private slots:
    void CallDispatcher();
    void on_ResetSim_clicked();
    void on_StepSim_clicked();
    void on_PauseSim_clicked();
    void on_StartSim_clicked();
    void on_ArrivalTime_textChanged(QString );
    void on_BurstTime_textChanged(QString );
    void on_ChangeAT_highlighted(int index);
    void on_ChangeBT_highlighted(int index);
    void on_CreateProcess_clicked();
    void on_TimeQuantum_textChanged(QString );
};

#endif // RR_H
