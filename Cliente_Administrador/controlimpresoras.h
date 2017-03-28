#ifndef CONTROLIMPRESORAS_H
#define CONTROLIMPRESORAS_H
#include "sockettcp.h"
#include "direcciones.h"
#include "stockimpresoras.h"

#include <QWidget>
#include <QDialog>
#include <QMessageBox>
#include <QObject>

static QList<QString> busqueda_impresoras;
static QList<int> cantStock;
static StockImpresoras* stock;
//static QList<StockImpresoras*> stock_list;

namespace Ui {
class ControlImpresoras;
}

class ControlImpresoras : public QDialog
{
    Q_OBJECT

public:
    explicit ControlImpresoras(SocketTcp* socket, QDialog *parent = 0);
    ~ControlImpresoras();
    SocketTcp* socket_;
    quint32 tamPacket;
    QString ip;

    QList<QString> marcamodelStock;
    Direcciones deserializar();
    void enviar(SocketTcp* socket, QByteArray bytes);
    QByteArray serializar(QString ip, QString equipo, QString ubicacion, quint16 subred, quint16 type);
    QByteArray serializar(QString ip, QString equipo, QString ubicacion, quint16 subred, QString marca , QString modelo ,quint16 type);

public slots:
    void readyRead();
    void cerrar();

private slots:
    void on_buscarGeneralButton_clicked();

    void on_addTecnisurButton_clicked();

    void on_editTecnisurButton_clicked();

    void on_delTecnisurButton_clicked();

    void on_addRCButton_clicked();

    void on_editRCButton_clicked();

    void on_delRCButton_clicked();

    void on_stockButton_clicked();

    void on_searchTecnisurButton_clicked();

    void on_searchRCButton_clicked();

private:
    Ui::ControlImpresoras *ui;
};

#endif // CONTROLIMPRESORAS_H
