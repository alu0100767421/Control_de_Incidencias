#include "sockettcp.h"

void enviar(SocketTcp *socket, QByteArray bytes)
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

QByteArray serializar(QString ip, QString equipo, QString ubicacion, quint16 subred, quint16 type)
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




SocketTcp::SocketTcp(QString ip_server, quint16 port_server, QString tipo_ ,QObject *parent) :
    QObject(parent),
    server_ip(ip_server),
    server_port(port_server),
    tipo(tipo_)
{

    QList<QHostAddress> list = QNetworkInterface::allAddresses();

    for(int nIter=0; nIter<list.count(); nIter++){
      if(!list[nIter].isLoopback())
          if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol )
                ip_local =  list[nIter].toString();

    }


    //TODO: SACAR IP_LOCAL
    sslSocket = new QSslSocket();
    sslSocket->setProtocol(QSsl::AnyProtocol);
    sslSocket->connectToHostEncrypted(ip_server, 9000);

    connect(sslSocket, SIGNAL(encrypted()), this, SLOT(ready()));
    connect(sslSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));
    connect(sslSocket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(error()));


}

//SE ACTIVA CUANDO EL HANDSHAKE ESTA COMPLETO
void SocketTcp::ready()
{
    //PIDE AL SERVIDOR LOS
    qDebug() << "Socket abierto";
    //ENVIARMOS DATOS AL SERVIDOR PARA QUE NOS AÑADA A LA LISTA
    //DE PCS CONECTADOS
    QByteArray bytes = serializar(ip_local+tipo, "", "", 0, 9);
    enviar(this, bytes);

}

void SocketTcp::error()
{

    qDebug() << sslSocket->errorString();
    sslSocket->ignoreSslErrors();
}
