#ifndef CONTROLIP_H
#define CONTROLIP_H

#include <QWidget>
#include <QMessageBox>
#include "sockettcp.h"
#include "direcciones.h"

namespace Ui {
class ControlIp;
}

class ControlIp : public QWidget
{
    Q_OBJECT

public:
    explicit ControlIp(SocketTcp* socket, QWidget *parent = 0);
    ~ControlIp();
    SocketTcp* socket_;
    quint32 tamPacket;
    Direcciones deserializar();
    void enviar(SocketTcp* socket, QByteArray bytes);
    QByteArray serializar(QString ip, QString equipo, QString ubicacion, quint16 subred, quint16 type);

private slots:
    void on_pushButton_clicked();

    void on_cleanFreeIpButton_clicked();

    void on_searchIpButton_clicked();

    void on_addIpButton_clicked();

    void on_deleteIpButton_clicked();

    void on_editIpButton_clicked();

public slots:
    void readyRead();

private:
    Ui::ControlIp *ui;
};

#endif // CONTROLIP_H
