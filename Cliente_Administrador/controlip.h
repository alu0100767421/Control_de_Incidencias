#ifndef CONTROLIP_H
#define CONTROLIP_H

#include <QWidget>
#include <QDialog>
#include <QMessageBox>
#include "sockettcp.h"
#include "direcciones.h"

namespace Ui {
class ControlIp;
}

class ControlIp : public QDialog
{
    Q_OBJECT

public:
    explicit ControlIp(SocketTcp* socket ,QDialog *parent = 0);
    ~ControlIp();
    SocketTcp* socket_;
    quint32 tamPacket;
    QString ip;
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
    void cerrar();

private:
    Ui::ControlIp *ui;
};

#endif // CONTROLIP_H
