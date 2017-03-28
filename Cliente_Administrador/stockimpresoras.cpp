#include "stockimpresoras.h"
#include "ui_stockimpresoras.h"


void StockImpresoras::enviar(SocketTcp *socket, QByteArray bytes)
{
    //Enviamos el tamaño del paquete
    quint32 size_packet = bytes.size();
    QByteArray envio;
    QDataStream env(&envio, QIODevice::WriteOnly);
    env.setVersion(7);
    env << (quint32)size_packet;

    socket->sslSocket->write(envio);
    socket->sslSocket->write(bytes);
    socket->sslSocket->waitForBytesWritten();
}

QByteArray StockImpresoras::serializar(QString ip, QString equipo, QString ubicacion, quint16 subred, quint16 type)
{
    QJsonObject peticion_ips_libres;
    peticion_ips_libres["type"] = type;
    peticion_ips_libres["subred"] = subred;
    peticion_ips_libres["ip"] = ip;
    peticion_ips_libres["equipo"] = equipo;
    peticion_ips_libres["ubicacion"] = ubicacion;

    QJsonDocument peticion(peticion_ips_libres);
    QByteArray bytes = peticion.toJson();

    return bytes;
}


StockImpresoras::StockImpresoras(QList<int> cant, SocketTcp* socket ,QWidget *parent) :
    QDialog(parent),
    cantidadTinta(cant),
    socket_(socket),
    ui(new Ui::StockImpresoras)
{
    ui->setupUi(this);


    ui->TINTA1->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA2->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA3->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA4->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA5->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA6->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA7->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA8->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA9->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA10->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA11->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA12->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA13->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA14->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA15->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA16->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA17->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA18->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA19->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA20->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA21->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA22->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA23->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA24->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA25->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA26->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA27->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA28->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA29->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA30->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA31->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA32->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA33->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA34->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA35->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA36->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA37->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA38->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA39->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
    ui->TINTA40->setText(QString::number(cantidadTinta.first()));
    cantidadTinta.pop_front();
}

StockImpresoras::~StockImpresoras()
{
    delete ui;
}

void StockImpresoras::actualizarStock(QList<int> cant)
{
    ui->TINTA1->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA2->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA3->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA4->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA5->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA6->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA7->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA8->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA9->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA10->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA11->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA12->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA13->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA14->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA15->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA16->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA17->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA18->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA19->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA20->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA21->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA22->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA23->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA24->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA25->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA26->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA27->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA28->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA29->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA30->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA31->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA32->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA33->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA34->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA35->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA36->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA37->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA38->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA39->setText(QString::number(cant.first()));
    cant.pop_front();
    ui->TINTA40->setText(QString::number(cant.first()));
    cant.pop_front();

    this->repaint();
}

void StockImpresoras::on_addStockButton_clicked()
{
    QString cantidad = ui->cantidadSpinBox->text();
    QString tinta = ui->tintasBox->currentText();

    QByteArray bytes = serializar(tinta,"","",cantidad.toInt(),20);
    enviar(socket_, bytes);



}

void StockImpresoras::on_delStockButton_clicked()
{
    QString cantidad = ui->cantidadSpinBox->text();
    QString tinta = ui->tintasBox->currentText();

    QByteArray bytes = serializar(tinta,"","",cantidad.toInt(),21);
    enviar(socket_, bytes);
}
