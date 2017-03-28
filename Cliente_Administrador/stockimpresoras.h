#ifndef STOCKIMPRESORAS_H
#define STOCKIMPRESORAS_H

//#include "controlimpresoras.h"
#include "sockettcp.h"
//#include "direcciones.h"
#include <QDialog>

namespace Ui {
class StockImpresoras;
}

class StockImpresoras : public QDialog
{
    Q_OBJECT

public:
    explicit StockImpresoras(QList<int> cant, SocketTcp* socket ,QWidget *parent = 0);
    ~StockImpresoras();
    void actualizarStock(QList<int> cant);
    QByteArray serializar(QString ip, QString equipo, QString ubicacion, quint16 subred, quint16 type);
    void enviar(SocketTcp *socket, QByteArray bytes);

private slots:
    void on_addStockButton_clicked();

    void on_delStockButton_clicked();

private:
    Ui::StockImpresoras *ui;
    QList<int> cantidadTinta;
    SocketTcp* socket_;
};

#endif // STOCKIMPRESORAS_H
