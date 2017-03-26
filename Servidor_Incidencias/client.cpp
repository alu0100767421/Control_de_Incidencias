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

void Client::enviar(QSslSocket *socket, QByteArray bytes)
{
    //Enviamos el tamaño del paquete
    quint32 size_packet = bytes.size();
    QByteArray envio;
    QDataStream env(&envio, QIODevice::WriteOnly);
    env.setVersion(7);
    env << (quint32)size_packet;

    socket->write(envio);
    socket->write(bytes);
    socket->waitForBytesWritten();
}

QByteArray Client::serializar(QString ip, QString equipo, QString ubicacion, quint16 subred, quint16 type)
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

Client::~Client()
{

}

void Client::readyRead()
{


    //MIENTRAS HAYAN BYTES
    while(sslSocket_->bytesAvailable()){
        Direcciones d;

        d = deserializar();

        qDebug() << d.type();



        if(d.type() == 10)
            return;

        QSqlQuery query(*db_);

        //CASE QUE PROCESA LAS PETICIONES
        switch (d.type()) {
        /*
         * CASOS
         *  0 - BUSCAR IP LIBRE
         *  1 - BUSCAR POR IP
         *  2 - AÑADIR NUEVA IP
         *  3 - BORRA IP EXISTENTE
         *  4 - EDITAR IP EXISTENTE
         *  5 - BUSCAR IMPRESORAS GENERAL
         *  6 - BUSCAR IMPRESORAS LEXMARK
         */

        case 0:{
            //Algortimo busqueda ip libre



            QList<QString> ips_libres;
            quint16 i = 1;

            for(i=1; i<255; i++){
                QString ip_actual ="192.168.";
                ip_actual += QString::number(d.subred());
                ip_actual += ".";
                ip_actual += QString::number(i);

                query.exec("SELECT IP FROM IPS WHERE IP = '" + ip_actual + "';");

                if(!query.first())
                    ips_libres.append(ip_actual);
            }


            for(int i=0; i<ips_libres.size(); i++){
                QByteArray bytes = serializar(ips_libres.value(i), "", "", 0, 0);
                enviar(sslSocket_, bytes);
            }



        }
            break;
        case 1:{
            QString ip_a_buscar = d.ip();

            query.exec("SELECT IP, NOMBRE_EQUIPO, UBICACION FROM IPS WHERE IP = '" + ip_a_buscar + "';");
            while(query.next()){
                QString ubi = query.value("UBICACION").toString();
                QString name = query.value("NOMBRE_EQUIPO").toString();
                QString ip = query.value("IP").toString();

                QByteArray bytes = serializar(ip, name, ubi, 0, 1);
                enviar(sslSocket_, bytes);
            }
        }
            break;
        case 2:{

            QByteArray bytes;

            query.exec("SELECT IP FROM IPS WHERE IP = '"+ d.ip() +"';");
            if(!query.first()){
               if(query.exec("INSERT INTO IPS (IP, NOMBRE_EQUIPO, UBICACION) VALUES ('" + d.ip() + "','" + d.nombre_equipo() + "','" + d.ubicacion() + "');")){
                   bytes = serializar("Se añadió la dirección correctamente","","",0,2);

               }else{
                   bytes = serializar("Fallo al añadir la dirección","","",0,2);
               }
            }else
            {
                bytes = serializar("Dirección en uso","","",0,2);
            }
           enviar(sslSocket_, bytes);


        }
            break;
        case 3:{
            QByteArray bytes;

            qDebug() << d.ip();

            query.exec("SELECT IP FROM IPS WHERE IP = '"+ d.ip() +"';");
            if(query.first()){
               if(query.exec("DELETE FROM IPS WHERE IP = '" + d.ip() + "';")){
                   bytes = serializar("Se borró la dirección correctamente","","",0,3);

               }else{
                   bytes = serializar("Fallo al borrar la dirección","","",0,3);
               }
            }else
            {
                bytes = serializar("Dirección libre","","",0,3);
            }
           enviar(sslSocket_, bytes);
        }
            break;
        case 4:{
            QByteArray bytes;

            query.exec("SELECT IP FROM IPS WHERE IP = '"+ d.ip() +"';");
            if(query.first()){
               if(query.exec("UPDATE IPS SET NOMBRE_EQUIPO = '"+d.nombre_equipo()+"', UBICACION ='"+d.ubicacion()+"' WHERE IP = '"+d.ip()+"';")){
                   bytes = serializar("Se actualizó la dirección correctamente","","",0,4);

               }else{
                   bytes = serializar("Fallo al actualizar la dirección","","",0,4);
               }
            }else
            {
                bytes = serializar("Dirección libre","","",0,4);
            }
           enviar(sslSocket_, bytes);

        }
            break;
        case 5:{
            query.exec("SELECT IP, DPTO, TIPO FROM IMPRESORAS WHERE 1;");
            while (query.next()) {
                QByteArray bytes = serializar(query.value("IP").toString(), query.value("DPTO").toString(), query.value("TIPO").toString(),0,5);
                enviar(sslSocket_, bytes);
            }
        }
            break;
        default:
            break;
        }

    }
}

void Client::error()
{
    qDebug() << sslSocket_->errorString();
}


