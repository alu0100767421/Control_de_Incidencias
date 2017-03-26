#include "controlip.h"
#include "ui_controlip.h"

ControlIp::ControlIp(SocketTcp* socket, QWidget *parent) :
    QWidget(parent),
    socket_(socket),
    tamPacket(0),
    ui(new Ui::ControlIp)
{
    ui->setupUi(this);
    connect(socket_->sslSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    //TODO: CONECTAR SEÑAL PARA CERRAR EL SOCKET CUANDO SE CIERRE.
    //connect(this, SIGNAL(re))
}

ControlIp::~ControlIp()
{
    delete ui;
}

Direcciones ControlIp::deserializar()
{


    /*
     * JSON:
     *  type
     *  ip
     *  ubicacion
     *  equipo
     */



    QByteArray buffer;
    Direcciones dir_ip;

    QJsonDocument doc;
    QJsonObject json;

    while(socket_->sslSocket->bytesAvailable() > 0){

        QDataStream in(socket_->sslSocket);
        in.setVersion(QDataStream::Qt_4_0);
        if(socket_->sslSocket->bytesAvailable() >= (int)(sizeof(qint32))&& (tamPacket==0)){
            in >> tamPacket;
        }
        if ((tamPacket !=0) && (socket_->sslSocket->bytesAvailable() >=tamPacket )){
             buffer=socket_->sslSocket->read(tamPacket);

             /*
              * DESERIALIZAMOS LA INFORMACION A UN JSON PARA
              * POSTERIORMENTE PROCESARLA COMO OBJETO DE LA
              * CLASE DIRECCIONES
              * */

             doc = QJsonDocument::fromJson(buffer); //PASAMOS EL JSON QUE VIENE COMO QBYTEARRAY A QJSONDOCUMENT
             json = doc.object();
             dir_ip.setIp(json.value("ip").toString());
             dir_ip.setNombre(json.value("equipo").toString());
             dir_ip.setUbicacion(json.value("ubicacion").toString());
             dir_ip.setType(json.value("type").toInt());
             dir_ip.setSubred(json.value("subred").toInt());



             //paquete.ParseFromString(buffer.toStdString());

             tamPacket =0;

             return dir_ip;

         }else{
              //paquete.set_type(10);
              dir_ip.setType(10);
              return dir_ip;
         }
        }
        //paquete.set_type(10);
        dir_ip.setType(10);
        return dir_ip;



}


void ControlIp::enviar(SocketTcp *socket, QByteArray bytes)
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

QByteArray ControlIp::serializar(QString ip, QString equipo, QString ubicacion, quint16 subred, quint16 type)
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

void ControlIp::on_pushButton_clicked()
{
    quint16 subred = ui->subredesBox->currentText().toInt();

    //TODO: MANDAR AL SERVIDOR INFORMACION CON TYPE 0 PARA RECIBIR INFORMACIÓN CON
    // LAS IPS LIBRES DE LA SUBRED SELECCIONADA

    ui->freeIpText->setText("");

    //SERIALIZAMOS LA INFORMACION Y LA ENVIAMOS
    QByteArray bytes = serializar(socket_->ip_local, "", "", subred, 0);
    enviar(socket_, bytes);


}

void ControlIp::on_cleanFreeIpButton_clicked()
{
    ui->freeIpText->setText("");
}


void ControlIp::on_searchIpButton_clicked()
{
    QString ip_a_buscar = ui->ipSearchEdit->text();

    ui->ipSearchEdit->setText("");
    ui->dirIp->setText("");
    ui->ubicacionEquipo->setText("");
    ui->nombreEquipo->setText("");

    QByteArray bytes = serializar(ip_a_buscar,"","",0,1);
    enviar(socket_,bytes);
}


void ControlIp::on_addIpButton_clicked()
{
    QString ip = ui->addIpLine->text();
    QString name = ui->addNameLine->text();
    QString ubi = ui->addUbicationLine->text();

    if(ip == "" || name == "" || ubi == ""){
        QMessageBox msgBox;
         msgBox.setText("Por favor rellena todos los campos.");
         msgBox.exec();
    }else{
        QByteArray bytes = serializar(ip,name,ubi,0,2);
        enviar(socket_,bytes);

        ui->addIpLine->setText("");
        ui->addNameLine->setText("");
        ui->addUbicationLine->setText("");
    }



}


void ControlIp::on_deleteIpButton_clicked()
{
    QString ip = ui->deleteIpLine->text();

    if(ip == ""){
        QMessageBox msgBox;
         msgBox.setText("Por favor introduce la ip.");
         msgBox.exec();
    }else{
        QByteArray bytes = serializar(ip,"","",0,3);
        enviar(socket_,bytes);


        ui->deleteIpLine->setText("");
    }
}


void ControlIp::on_editIpButton_clicked()
{
    QString ip = ui->editIpLine->text();
    QString name = ui->editNameLine->text();
    QString ubi = ui->editUbicacionLine->text();

    if(ip == "" || name == ""){
        QMessageBox msgBox;
         msgBox.setText("Por favor rellena al menos la ip y el nombre del equipo.");
         msgBox.exec();
    }else{
        QByteArray bytes = serializar(ip,name,ubi,0,4);
        enviar(socket_,bytes);

        ui->editIpLine->setText("");
        ui->editNameLine->setText("");
        ui->editUbicacionLine->setText("");
    }
}



void ControlIp::readyRead()
{

    //MIENTRAS HAYAN BYTES
    while(socket_->sslSocket->bytesAvailable()){
        Direcciones d;

        d = deserializar();


        if(d.type() == 10)
            return;

        switch (d.type()) {
            case 0:{
                //IPS LIBRES

                //MOSTRAMOS LAS IP LIBRES
                ui->freeIpText->append(d.ip());

            }
                break;
            case 1:{
                ui->nombreEquipo->setText(d.nombre_equipo());
                ui->ubicacionEquipo->setText(d.ubicacion());
                ui->dirIp->setText(d.ip());

            }
                break;
            case 2:
            case 3:
            case 4:{
                QMessageBox msgBox;
                 msgBox.setText(d.ip());
                 msgBox.exec();
            }
                break;
        default:
            break;
        }


    }
}






