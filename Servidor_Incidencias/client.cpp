#include "client.h"

Client::Client(QSslSocket* sslSocket, QSqlDatabase* db, QObject *parent) :
    QObject(parent),
    db_(db),
    sslSocket_(sslSocket)
{

    tamPacket = 0;
    connect(sslSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(sslSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));
    connect(sslSocket, SIGNAL(sslErrors(QList<QSslError>)), sslSocket, SLOT(ignoreSslErrors()));
    connect(sslSocket, SIGNAL(disconnected()), sslSocket, SLOT(deleteLater()));

    qDebug() << "Client";

    //CHECK IF DATABASE IS NOT OPEN
    if(!db_->open()){
        exit(-1);
    }

}

Direcciones Client::deserializar()
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

    while(sslSocket_->bytesAvailable() > 0){


        QDataStream in(sslSocket_);
        in.setVersion(QDataStream::Qt_4_0);
        if(sslSocket_->bytesAvailable() >= (int)(sizeof(qint32))&& (tamPacket==0)){
            in >> tamPacket;
        }
        if ((tamPacket !=0) && (sslSocket_->bytesAvailable() >=tamPacket )){
             buffer=sslSocket_->read(tamPacket);

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

Client::~Client()
{

}

void Client::readyRead()
{

    //MIENTRAS HAYAN BYTES
    while(sslSocket_->bytesAvailable()){
        Direcciones d;

        d = deserializar();



        if(d.type() == 10)
            return;

        QSqlQuery query(*db_);

        //CASE QUE PROCESA LAS PETICIONES
        switch (d.type()) {
        /*
         * CASOS
         *  0 - BUSCAR IP LIBRE
         *  1 - BUSCAR POR IP
         *  2 - BUSCAR POR NOMBRE EQUIPO
         */

        case 0:
            //Algortimo busqueda ip libre

            break;
        default:
            break;
        }

    }
}

void Client::error()
{

}


