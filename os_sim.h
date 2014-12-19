#ifndef OS_SIM_H
#define OS_SIM_H
#include <QWidget>
#include <QHBoxLayout>
namespace Ui {
    class OS_Sim;
}
enum Scheduling{
    FCFSS = 1,
    SJF,
    PRIORITY,
    RR
};
enum MegType{
    QVALUE_ZERO,
    QVALUE_OUT_RANGE,
    BURST_MSG,
    ARRIVAL_MSG,
    PRORITY_MSG
};

class OS_Sim : public QWidget
{
    Q_OBJECT

public:
    explicit OS_Sim(QWidget *parent = 0);
    ~OS_Sim();

private:
    Ui::OS_Sim *ui;
    int iTypeOfScheduling;
private slots:
    void on_SelectScheduling_clicked();
    void on_SchedulingType_highlighted(int index);
};

#endif // OS_SIM_H
