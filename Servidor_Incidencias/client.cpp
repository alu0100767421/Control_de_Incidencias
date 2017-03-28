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

             /*Impresoras*/
             dir_ip.setMarcaImpresora(json.value("marca").toString());
             dir_ip.setModeloImpresora(json.value("modelo").toString());



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

QByteArray Client::serializar(QString ip, QString equipo, QString ubicacion, quint16 subred, QString marca, QString modelo, quint16 type)
{
    QJsonObject peticion_ips_libres;
    peticion_ips_libres["type"] = type;
    peticion_ips_libres["subred"] = subred;
    peticion_ips_libres["ip"] = ip;
    peticion_ips_libres["equipo"] = equipo;
    peticion_ips_libres["ubicacion"] = ubicacion;
    peticion_ips_libres["marca"] = marca;
    peticion_ips_libres["modelo"] = modelo;

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




        if(d.type() == 10)
            return;

        QSqlQuery query(*db_);
        QByteArray bytes;

        //CASE QUE PROCESA LAS PETICIONES
        switch (d.type()) {
        /*
         * CASOS
         *  0 - BUSCAR IP LIBRE +
         *  1 - BUSCAR POR IP +
         *  2 - AÑADIR NUEVA IP +
         *  3 - BORRA IP EXISTENTE +
         *  4 - EDITAR IP EXISTENTE +
         *  5 - BUSCAR IMPRESORAS TECNISUR  +
         *  6 - AÑADIR IMPRESORA TECNISUR +
         *  7 - EDITAR IMPRESORA TECNISUR +
         *  8 - ELIMINAR IMPRESORA TECNISUR +
         *  12 - AÑADIR IMPRESORA RC +
         *  13 - EDITAR IMPRESORA RC +
         *  14 - ELIMINAR IMPRESORA RC +
         *  15 - BUSCAR IMPRESORA RC +
         *  16 - STOCK IMPRESORAS TINTA +
         *  17 - FIN MANDAR STOCK +
         *  18 - FIN BUSQUEDA TECNISUR +
         *  19 - FIN BUSQUEDA RC +
         *  20 - AÑADIR CANTIDAD TINTA STOCK
         *
         *
         *  9 - NUEVA CONEXION SOCKET
         *  11 - DESCONEXION DEL USUARIO Y ELIMINACION DE LA LISTA
         *
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

            QString ip = d.ip();
            QString dpto = d.ubicacion();

            if(ip != "" && dpto != ""){
               //BUSQUEDA POR IP Y DEPARTAMENTO
                query.exec("SELECT IP FROM IPS WHERE IP = '"+d.ip()+"'");
                if(query.first()){
                    query.exec("SELECT IP, DEPARTAMENTO, MARCA, MODELO, CARTUCHO FROM TECNISUR WHERE IP = '"+ip+"' AND DEPARTAMENTO = '"+dpto+"';");
                    while(query.next()){
                        QString ip_printer = query.value("IP").toString();
                        QString dpto_printer = query.value("DEPARTAMENTO").toString();
                        QString marca_printer = query.value("MARCA").toString();
                        QString modelo_printer = query.value("MODELO").toString();
                        QString cartucho_printer = query.value("CARTUCHO").toString();

                        QByteArray bytes = serializar(ip_printer, cartucho_printer, dpto_printer,0,marca_printer, modelo_printer, 5);
                        enviar(sslSocket_, bytes);
                    }

                    QByteArray bytes = serializar("","","",0,18);
                    enviar(sslSocket_, bytes);
                }else{
                    bytes = serializar("Dirección libre","","",0,5);
                }

            }else if(ip != ""){
                //BUSQUEDA SOLO POR IP

                query.exec("SELECT IP FROM IPS WHERE IP = '"+d.ip()+"'");
                if(query.first()){
                    query.exec("SELECT IP, DEPARTAMENTO, MARCA, MODELO, CARTUCHO FROM TECNISUR WHERE IP = '"+ip+"';");
                    while(query.next()){
                        QString ip_printer = query.value("IP").toString();
                        QString dpto_printer = query.value("DEPARTAMENTO").toString();
                        QString marca_printer = query.value("MARCA").toString();
                        QString modelo_printer = query.value("MODELO").toString();
                        QString cartucho_printer = query.value("CARTUCHO").toString();

                        QByteArray bytes = serializar(ip_printer, cartucho_printer, dpto_printer,0,marca_printer, modelo_printer, 5);
                        enviar(sslSocket_, bytes);
                    }

                    QByteArray bytes = serializar("","","",0,18);
                    enviar(sslSocket_, bytes);
                }else{
                    bytes = serializar("Dirección libre","","",0,5);
                }


            }else if(dpto != ""){
                //BUSQUEDA POR DEPARTAMENTO
                QString dpto = d.ubicacion();

                query.exec("SELECT IP, DEPARTAMENTO, MARCA, MODELO, CARTUCHO FROM TECNISUR WHERE DEPARTAMENTO = '"+dpto+"';");
                while(query.next()){
                    QString ip_printer = query.value("IP").toString();
                    QString dpto_printer = query.value("DEPARTAMENTO").toString();
                    QString marca_printer = query.value("MARCA").toString();
                    QString modelo_printer = query.value("MODELO").toString();
                    QString cartucho_printer = query.value("CARTUCHO").toString();

                    QByteArray bytes = serializar(ip_printer, cartucho_printer, dpto_printer,0,marca_printer, modelo_printer, 5);
                    enviar(sslSocket_, bytes);


                }
                QByteArray bytes = serializar("","","",0,18);
                enviar(sslSocket_, bytes);
            }

        }
            break;
        case 6:{
            query.exec("SELECT IP FROM IPS INNER JOIN TECNISUR WHERE IP = '"+ d.ip() +"';");
            if(!query.first()){
               if(query.exec("INSERT INTO IPS (IP, NOMBRE_EQUIPO, UBICACION) VALUES ('" + d.ip() + "','" + d.marcaImpresora() + "','" + d.ubicacion() + "');")){
                   //bytes = serializar("","","",0,6);

               }

               query.exec("SELECT CARTUCHO FROM TECNISUR WHERE MODELO = '"+d.modeloImpresora()+"';");


               if(query.exec("INSERT INTO TECNISUR (IP, DEPARTAMENTO, MARCA, MODELO, CARTUCHO) VALUES ('" + d.ip() + "', '"+d.ubicacion()+"' ,'" + d.marcaImpresora() + "','" + d.modeloImpresora() + "', '"+query.value("CARTUCHO").toString()+"');")){
                   bytes = serializar("Se añadió la impresora correctamente","","",0,6);

               }else{
                   bytes = serializar("Fallo al añadir la impresora","","",0,6);
               }

            }else
            {
                bytes = serializar("Dirección en uso","","",0,6);
            }
           enviar(sslSocket_, bytes);

        }
            break;
        case 7:{

            query.exec("SELECT IP FROM TECNISUR WHERE IP = '"+ d.ip() +"';");
            if(query.first()){
               if(query.exec("UPDATE TECNISUR SET DEPARTAMENTO = '"+d.ubicacion()+"', MARCA ='"+d.marcaImpresora()+"' , MODELO = '"+d.modeloImpresora()+"' WHERE IP = '"+d.ip()+"';")){
                   bytes = serializar("Se actualizó la impresora correctamente","","",0,7);


               }else{
                   bytes = serializar("Fallo al actualizar la impresora","","",0,7);
               }
            }else
            {
                bytes = serializar("Dirección libre","","",0,7);
            }
           enviar(sslSocket_, bytes);

        }
            break;
        case 8:{

            query.exec("SELECT IP FROM TECNISUR WHERE IP = '"+ d.ip() +"';");
            if(query.first()){
               query.exec("DELETE FROM IPS WHERE IP = '" + d.ip() + "';");

               if(query.exec("DELETE FROM TECNISUR WHERE IP = '"+d.ip()+"'")){
                   bytes = serializar("Impresora borrada.","","",0,8);
               }else{
                   bytes = serializar("Fallo al borrar la impresora.","","",0,8);
               }

            }else
            {
                bytes = serializar("Dirección libre","","",0,8);
            }
           enviar(sslSocket_, bytes);

        }
            break;
        case 9:{
            //INTRODUCIMOS EL SOCKET EN LA LISTA
            listPC.insert(d.ip(),sslSocket_);

            qDebug() << listPC;
        }
            break;
        case 11:{
            sslSocket_->disconnectFromHost();
            sslSocket_->close();
            listPC.remove(d.ip());

            qDebug() << listPC;
        }
            break;
        case 12:{
            query.exec("SELECT IP FROM IPS INNER JOIN LEXMARK WHERE IP = '"+ d.ip() +"';");
            if(!query.first()){
               if(query.exec("INSERT INTO IPS (IP, NOMBRE_EQUIPO, UBICACION) VALUES ('" + d.ip() + "','" + d.marcaImpresora() + "','" + d.ubicacion() + "');")){
                   //bytes = serializar("","","",0,6);

               }

               query.exec("SELECT CARTUCHO FROM LEXMARK WHERE MODELO = '"+d.modeloImpresora()+"';");

               if(query.exec("INSERT INTO LEXMARK (IP, DEPARTAMENTO, MARCA, MODELO, CARTUCHO) VALUES ('" + d.ip() + "', '"+d.ubicacion()+"' ,'" + d.marcaImpresora() + "','" + d.modeloImpresora() + "', '"+query.value("CARTUCHO").toString()+"');")){
                   bytes = serializar("Se añadió la impresora correctamente","","",0,6);

               }else{
                   bytes = serializar("Fallo al añadir la impresora","","",0,12);
               }

            }else
            {
                bytes = serializar("Dirección en uso","","",0,12);
            }
           enviar(sslSocket_, bytes);
        }
            break;
        case 13:{

            query.exec("SELECT IP FROM LEXMARK WHERE IP = '"+ d.ip() +"';");
            if(query.first()){
               if(query.exec("UPDATE LEXMARK SET DEPARTAMENTO = '"+d.ubicacion()+"', MARCA ='"+d.marcaImpresora()+"' , MODELO = '"+d.modeloImpresora()+"' WHERE IP = '"+d.ip()+"';")){
                   bytes = serializar("Se actualizó la impresora correctamente","","",0,13);

               }else{
                   bytes = serializar("Fallo al actualizar la impresora","","",0,13);
               }
            }else
            {
                bytes = serializar("Dirección libre","","",0,13);
            }
           enviar(sslSocket_, bytes);

        }
            break;
        case 14:{

            query.exec("SELECT IP FROM LEXMARK WHERE IP = '"+ d.ip() +"';");
            if(query.first()){
               query.exec("DELETE FROM IPS WHERE IP = '" + d.ip() + "';");

               if(query.exec("DELETE FROM LEXMARK WHERE IP = '"+d.ip()+"'")){
                   bytes = serializar("Impresora borrada.","","",0,14);
               }else{
                   bytes = serializar("Fallo al borrar la impresora.","","",0,14);
               }

            }else
            {
                bytes = serializar("Dirección libre","","",0,14);
            }
           enviar(sslSocket_, bytes);

        }
            break;
        case 15:{
            QString ip = d.ip();
            QString dpto = d.ubicacion();

            if(ip != "" && dpto != ""){
               //BUSQUEDA POR IP Y DEPARTAMENTO
                query.exec("SELECT IP FROM IPS WHERE IP = '"+d.ip()+"'");
                if(query.first()){
                    query.exec("SELECT IP, DEPARTAMENTO, MARCA, MODELO, CARTUCHO FROM LEXMARK WHERE IP = '"+ip+"' AND DEPARTAMENTO = '"+dpto+"';");
                    while(query.next()){
                        QString ip_printer = query.value("IP").toString();
                        QString dpto_printer = query.value("DEPARTAMENTO").toString();
                        QString marca_printer = query.value("MARCA").toString();
                        QString modelo_printer = query.value("MODELO").toString();
                        QString cartucho_printer = query.value("CARTUCHO").toString();

                        QByteArray bytes = serializar(ip_printer, cartucho_printer, dpto_printer,0,marca_printer, modelo_printer, 5);
                        enviar(sslSocket_, bytes);
                    }

                    QByteArray bytes = serializar("","","",0,19);
                    enviar(sslSocket_, bytes);
                }else{
                    bytes = serializar("Dirección libre","","",0,5);
                }

            }else if(ip != ""){
                //BUSQUEDA SOLO POR IP

                query.exec("SELECT IP FROM IPS WHERE IP = '"+d.ip()+"'");
                if(query.first()){
                    query.exec("SELECT IP, DEPARTAMENTO, MARCA, MODELO, CARTUCHO FROM LEXMARK WHERE IP = '"+ip+"';");
                    while(query.next()){
                        QString ip_printer = query.value("IP").toString();
                        QString dpto_printer = query.value("DEPARTAMENTO").toString();
                        QString marca_printer = query.value("MARCA").toString();
                        QString modelo_printer = query.value("MODELO").toString();
                        QString cartucho_printer = query.value("CARTUCHO").toString();

                        QByteArray bytes = serializar(ip_printer, cartucho_printer, dpto_printer,0,marca_printer, modelo_printer, 5);
                        enviar(sslSocket_, bytes);
                    }

                    QByteArray bytes = serializar("","","",0,19);
                    enviar(sslSocket_, bytes);
                }else{
                    bytes = serializar("Dirección libre","","",0,5);
                }


            }else if(dpto != ""){
                //BUSQUEDA POR DEPARTAMENTO
                QString dpto = d.ubicacion();

                query.exec("SELECT IP, DEPARTAMENTO, MARCA, MODELO, CARTUCHO FROM LEXMARK WHERE DEPARTAMENTO = '"+dpto+"';");
                while(query.next()){
                    QString ip_printer = query.value("IP").toString();
                    QString dpto_printer = query.value("DEPARTAMENTO").toString();
                    QString marca_printer = query.value("MARCA").toString();
                    QString modelo_printer = query.value("MODELO").toString();
                    QString cartucho_printer = query.value("CARTUCHO").toString();

                    QByteArray bytes = serializar(ip_printer, cartucho_printer, dpto_printer,0,marca_printer, modelo_printer, 5);
                    enviar(sslSocket_, bytes);


                }
                QByteArray bytes = serializar("","","",0,19);
                enviar(sslSocket_, bytes);
            }

        }
            break;
        case 16:{
            query.exec("SELECT TINTA, CANTIDAD FROM TINTA ORDER BY TINTA;");
            while(query.next()){
                int cantidad = query.value("CANTIDAD").toInt();

                QByteArray bytes = serializar("","","",cantidad,"","",16);
                enviar(sslSocket_, bytes);

            }

            QByteArray bytes = serializar("","","",0,"","",17);
            enviar(sslSocket_, bytes);
        }
            break;
        case 20:{
            if(query.exec("UPDATE TINTA SET CANTIDAD = CANTIDAD + "+QString::number(d.subred())+" WHERE TINTA = '"+d.ip()+"';")){
                query.exec("SELECT TINTA, CANTIDAD FROM TINTA ORDER BY TINTA;");
                while(query.next()){
                    int cantidad = query.value("CANTIDAD").toInt();

                    QByteArray bytes = serializar("","","",cantidad,"","",16);
                    enviar(sslSocket_, bytes);

                }

                QByteArray bytes = serializar("","","",0,"","",20);
                enviar(sslSocket_, bytes);
            }

        }
            break;
        case 21:{
            if(query.exec("UPDATE TINTA SET CANTIDAD = CANTIDAD - "+QString::number(d.subred())+" WHERE TINTA = '"+d.ip()+"';")){
                query.exec("SELECT TINTA, CANTIDAD FROM TINTA ORDER BY TINTA;");
                while(query.next()){
                    int cantidad = query.value("CANTIDAD").toInt();

                    QByteArray bytes = serializar("","","",cantidad,"","",16);
                    enviar(sslSocket_, bytes);

                }

                QByteArray bytes = serializar("","","",0,"","",20);
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


