#include "sslserver.h"


#include <QSslSocket>
#include <QSslCertificate>
#include <QSslKey>
#include <iostream>
//#include <syslog.h>

SslServer::SslServer(QString dir, quint16 port, QSqlDatabase *db, QObject *parent):
    QTcpServer(parent),
    db(db),
    dir_(dir),
    port_(port)
{

}

SslServer::~SslServer()
{
    delete server;
    db->close();
    delete db;
}


void SslServer::incomingConnection(qintptr socketDescriptor){

    QSslSocket* socket = new QSslSocket();

    if(socket->setSocketDescriptor(socketDescriptor)){
        connect(socket, SIGNAL(sslErrors(QList<QSslError>)), socket, SLOT(ignoreSslErrors()));
        socket->setProtocol(QSsl::TlsV1_1);
        socket->setPrivateKey("Cert/server.key");
        socket->setLocalCertificate("Cert/server.crt");
        socket->ignoreSslErrors();

        std::cout << "New conecction" << std::endl;

        Client *client = new Client(socket, db, this);
        socket->startServerEncryption();
    }
    else{
        delete socket;
    }

}
