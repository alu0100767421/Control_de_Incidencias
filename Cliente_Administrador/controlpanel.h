#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QMainWindow>
#include "sockettcp.h"
#include "controlip.h"
#include "controlimpresoras.h"

namespace Ui {
class ControlPanel;
}

class ControlPanel : public QMainWindow
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget *parent = 0);
    ~ControlPanel();

private slots:
    void on_ipControlButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::ControlPanel *ui;
};

#endif // CONTROLPANEL_H
