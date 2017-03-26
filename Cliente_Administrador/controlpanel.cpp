#include "controlpanel.h"
#include "ui_controlpanel.h"

ControlPanel::ControlPanel(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ControlPanel)
{
    ui->setupUi(this);
}

ControlPanel::~ControlPanel()
{
    delete ui;
}

void ControlPanel::on_ipControlButton_clicked()
{
    SocketTcp* socket = new SocketTcp("192.168.1.100", 9000, this);
    ControlIp* control_ip = new ControlIp(socket);
    control_ip->show();
}

void ControlPanel::on_pushButton_clicked()
{
    SocketTcp* socket = new SocketTcp("192.168.1.100", 9000, this);
    ControlImpresoras* control_impresoras = new ControlImpresoras(socket);
    control_impresoras->show();


}
