#include "controlimpresoras.h"
#include "ui_controlimpresoras.h"

ControlImpresoras::ControlImpresoras(SocketTcp* socket, QWidget *parent) :
    QWidget(parent),
    socket_(socket),
    tamPacket(0),
    ui(new Ui::ControlImpresoras)
{
    ui->setupUi(this);
    connect(socket_->sslSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    //TODO: CONECTAR SEÑAL PARA CERRAR EL SOCKET CUANDO SE CIERRE.
    //connect(this, SIGNAL(re)
}

ControlImpresoras::~ControlImpresoras()
{
    delete ui;
}

Direcciones ControlImpresoras::deserializar()
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


void ControlImpresoras::enviar(SocketTcp *socket, QByteArray bytes)
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

QByteArray ControlImpresoras::serializar(QString ip, QString equipo, QString ubicacion, quint16 subred, quint16 type)
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


void ControlImpresoras::on_buscarGeneralButton_clicked()
{
    QByteArray bytes = serializar("","","",0,5);
    enviar(socket_,bytes);
}


void ControlImpresoras::readyRead()
{
    while(socket_->sslSocket->bytesAvailable()){
        Direcciones d;

        d = deserializar();


        if(d.type() == 10)
            return;

        switch (d.type()) {
            case 5:{
                QString impresora = d.ip();
                impresora += " ------- ";
                impresora += d.nombre_equipo();
                impresora += " ------- ";
                impresora += d.ubicacion();
                ui->generalPlain->append(impresora);
            }
            break;
        default:
            break;
        }


    }
}


