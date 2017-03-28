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
    SocketTcp* socket = new SocketTcp("127.0.0.1", 9000, "_IP",this);
    ControlIp* control_ip = new ControlIp(socket);
    //control_ip->show();
    control_ip->setWindowFlags(Qt::Widget);
    control_ip->show();
    //control_ip->exec();
}

void ControlPanel::on_pushButton_clicked()
{
    SocketTcp* socket = new SocketTcp("127.0.0.1", 9000, "_IMPRESORAS" ,this);
    ControlImpresoras* control_impresoras = new ControlImpresoras(socket);
    //control_impresoras->show();
    control_impresoras->setWindowFlags(Qt::Widget);
    control_impresoras->show();
    //control_impresoras->exec();

}
