#include "sockettcp.h"

SocketTcp::SocketTcp(QString ip_server, quint16 port_server, QObject *parent) :
    QObject(parent),
    server_ip(ip_server),
    server_port(port_server)
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
    sslSocket->connectToHostEncrypted("192.168.1.100", 9000);

    connect(sslSocket, SIGNAL(encrypted()), this, SLOT(ready()));
    connect(sslSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));
    connect(sslSocket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(error()));


}

//SE ACTIVA CUANDO EL HANDSHAKE ESTA COMPLETO
void SocketTcp::ready()
{
    //PIDE AL SERVIDOR LOS
    qDebug() << "Socket abierto";
}

void SocketTcp::error()
{

    qDebug() << sslSocket->errorString();
    sslSocket->ignoreSslErrors();
}
