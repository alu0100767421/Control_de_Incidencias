#ifndef CONTROLIMPRESORAS_H
#define CONTROLIMPRESORAS_H
#include "sockettcp.h"
#include "direcciones.h"

#include <QWidget>

namespace Ui {
class ControlImpresoras;
}

class ControlImpresoras : public QWidget
{
    Q_OBJECT

public:
    explicit ControlImpresoras(SocketTcp* socket, QWidget *parent = 0);
    ~ControlImpresoras();
    SocketTcp* socket_;
    quint32 tamPacket;
    Direcciones deserializar();
    void enviar(SocketTcp* socket, QByteArray bytes);
    QByteArray serializar(QString ip, QString equipo, QString ubicacion, quint16 subred, quint16 type);

public slots:
    void readyRead();

private slots:
    void on_buscarGeneralButton_clicked();

private:
    Ui::ControlImpresoras *ui;
};

#endif // CONTROLIMPRESORAS_H
