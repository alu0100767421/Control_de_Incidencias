#include "controlimpresoras.h"
#include "ui_controlimpresoras.h"

ControlImpresoras::ControlImpresoras(SocketTcp* socket ,QDialog *parent) :
    QDialog(parent),
    socket_(socket),
    tamPacket(0),
    ip(socket->ip_local),
    ui(new Ui::ControlImpresoras)
{
    ui->setupUi(this);
    connect(socket_->sslSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    //TODO: CONECTAR SEÑAL PARA CERRAR EL SOCKET CUANDO SE CIERRE.
    connect(this, SIGNAL(rejected()), this, SLOT(cerrar()));
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

QByteArray ControlImpresoras::serializar(QString ip, QString equipo, QString ubicacion, quint16 subred, QString marca , QString modelo ,quint16 type){
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


                busqueda_impresoras.append(d.ip());
                busqueda_impresoras.append(d.ubicacion());
                busqueda_impresoras.append(d.marcaImpresora());
                busqueda_impresoras.append(d.modeloImpresora());
                busqueda_impresoras.append(d.nombre_equipo()); //CARTUCHO

            }
            break;
            case 6:
            case 7:
            case 8:
            case 12:{

                //AÑADIR IMPRESORA TECNISUR
                //EDITAR IMPRESORA TECNISUR
                //ELIMINAR IMPRESORA TECNISUR
                //AÑADIR IMPRESORA RC

            QMessageBox msgBox;
             msgBox.setWindowTitle("Aviso");
             msgBox.setText(d.ip());
             msgBox.exec();
            }

            break;
        case 16:{
            int cantidad;
            cantidad = d.subred();

            cantStock.append(cantidad);

        }
            break;
        case 17:{
            stock = new StockImpresoras(cantStock, socket_);
            while(!cantStock.empty()){
                cantStock.pop_front();
            }
            //stock_list.append(stock);
            stock->show();
        }
            break;
        case 18:
        case 19:{
            //MOSTRAR BUSQUEDA IMPRESORAS
            QMessageBox msgBox;
            QString impresoras;

             msgBox.setWindowTitle("IMPRESORAS");

             while(!busqueda_impresoras.empty()){
                 impresoras += "\nIP: " + busqueda_impresoras.first();
                 busqueda_impresoras.pop_front();
                 impresoras += "\nDEPARTAMENTO: " +  busqueda_impresoras.first();
                 busqueda_impresoras.pop_front();
                 impresoras += "\nMARCA: " + busqueda_impresoras.first();
                 busqueda_impresoras.pop_front();
                 impresoras += "\nMODELO: " + busqueda_impresoras.first();
                 busqueda_impresoras.pop_front();
                 impresoras += "\nCARTUCHO: " + busqueda_impresoras.first();
                 busqueda_impresoras.pop_front();
                 impresoras += "\n-----------------------------";

             }

             msgBox.setText(impresoras);
             msgBox.exec();

        }
            break;
        case 20:{
            stock->actualizarStock(cantStock);
            while(!cantStock.empty()){
                cantStock.pop_front();
            }

        }
            break;
        default:
            break;
        }


    }
}

void ControlImpresoras::cerrar()
{
    QByteArray bytes = serializar(ip+"_IMPRESORAS", "", "", 0, 11);
    enviar(socket_, bytes);
}



void ControlImpresoras::on_addTecnisurButton_clicked()
{
    QString ip;
    QString dpto;
    QString marca;
    QString modelo;

    ip = ui->ipTecnisurAddLine->text();
    dpto = ui->dptoTecnisurAddBox->currentText();
    marca = ui->marcaTecnisurAddBox->currentText();
    modelo = ui->modeloTecnisurAddLine->text();

    if(ip != "" && modelo != ""){
        QByteArray bytes = serializar(ip,"",dpto,0,marca,modelo,6);
        enviar(socket_, bytes);
    }else{
        QMessageBox msgBox;
         msgBox.setWindowTitle("Error");
         msgBox.setText("Por favor introduce la ip y modelo.");
         msgBox.exec();
    }

}

void ControlImpresoras::on_editTecnisurButton_clicked()
{
    QString ip;
    QString dpto;
    QString marca;
    QString modelo;

    ip = ui->ipTecnisurEditLine->text();
    dpto = ui->dptoTecnisurEditBox->currentText();
    marca = ui->marcaTecnisurEditBox->currentText();
    modelo = ui->modeloTecnisurEditLine->text();

    if(ip != "" || dpto != "" || marca != "" || modelo != ""){
        QByteArray bytes = serializar(ip,"",dpto,0,marca,modelo,7);
        enviar(socket_, bytes);
    }else{
        QMessageBox msgBox;
         msgBox.setWindowTitle("Error");
         msgBox.setText("Por favor introduce todos los parámetros.");
         msgBox.exec();
    }

    ui->ipTecnisurAddLine->setText("");
    ui->modeloTecnisurAddLine->setText("");

}

void ControlImpresoras::on_delTecnisurButton_clicked()
{
   QString ip = ui->ipTecnisurAddLine->text();

   if(ip != ""){
        QByteArray bytes = serializar(ip,"","",0,8);
        enviar(socket_, bytes);
   }else{
       QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText("Por favor introduce la ip para eliminar.");
        msgBox.exec();
   }

   ui->ipTecnisurAddLine->setText("");
   ui->modeloTecnisurAddLine->setText("");

}

void ControlImpresoras::on_addRCButton_clicked()
{
    QString ip;
    QString dpto;
    QString marca;
    QString modelo;

    ip = ui->ipRCAddLine->text();
    dpto = ui->dptoRCAddBox->currentText();
    marca = ui->marcaRCAddBox->currentText();
    modelo = ui->modeloRCAddLine->text();

    if(ip != "" && modelo != ""){
        QByteArray bytes = serializar(ip,"",dpto,0,marca,modelo,12);
        enviar(socket_, bytes);
    }else{
        QMessageBox msgBox;
         msgBox.setWindowTitle("Error");
         msgBox.setText("Por favor introduce la ip y modelo.");
         msgBox.exec();
    }

    ui->ipRCAddLine->setText("");
    ui->modeloRCAddLine->setText("");

}

void ControlImpresoras::on_editRCButton_clicked()
{
    QString ip;
    QString dpto;
    QString marca;
    QString modelo;

    ip = ui->ipRCEditLine->text();
    dpto = ui->dptoRCEditBox->currentText();
    marca = ui->marcaRCEditBox->currentText();
    modelo = ui->modeloRCEditLine->text();

    if(ip != "" || dpto != "" || marca != "" || modelo != ""){
        QByteArray bytes = serializar(ip,"",dpto,0,marca,modelo,13);
        enviar(socket_, bytes);
    }else{
        QMessageBox msgBox;
         msgBox.setWindowTitle("Error");
         msgBox.setText("Por favor introduce todos los parámetros.");
         msgBox.exec();
    }

    ui->ipRCEditLine->setText("");
    ui->modeloRCEditLine->setText("");
}

void ControlImpresoras::on_delRCButton_clicked()
{
    QString ip = ui->ipRCAddLine->text();

    if(ip != ""){
         QByteArray bytes = serializar(ip,"","",0,14);
         enviar(socket_, bytes);
    }else{
        QMessageBox msgBox;
         msgBox.setWindowTitle("Error");
         msgBox.setText("Por favor introduce la ip para eliminar.");
         msgBox.exec();
    }

    ui->ipRCAddLine->setText("");
    ui->modeloRCAddLine->setText("");
}

void ControlImpresoras::on_stockButton_clicked()
{

    QByteArray bytes = serializar("","","",0,16);
    enviar(socket_, bytes);

}

void ControlImpresoras::on_searchTecnisurButton_clicked()
{
    QString ip = ui->ipTecnisurSearchLine->text();
    QString dpto = ui->dptoTecnisurSearchBox->currentText();

    if(ip != "" && dpto != ""){
       QByteArray bytes = serializar(ip,"",dpto,0,5);
       enviar(socket_, bytes);
    }else if(ip != ""){
        QByteArray bytes = serializar(ip,"","",0,5);
        enviar(socket_, bytes);
    }else if(dpto != ""){
        QByteArray bytes = serializar("","",dpto,0,5);
        enviar(socket_, bytes);
    }


}

void ControlImpresoras::on_searchRCButton_clicked()
{
    QString ip = ui->ipRCSearchLine->text();
    QString dpto = ui->dptoRCSearchBox->currentText();

    if(ip != "" && dpto != ""){
       QByteArray bytes = serializar(ip,"",dpto,0,15);
       enviar(socket_, bytes);
    }else if(ip != ""){
        QByteArray bytes = serializar(ip,"","",0,15);
        enviar(socket_, bytes);
    }else if(dpto != ""){
        QByteArray bytes = serializar("","",dpto,0,15);
        enviar(socket_, bytes);
    }
}
