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
        socket->setProtocol(QSsl::AnyProtocol);
        socket->setPrivateKey("C:/INCIDENCIAS/CERT_INCIDENCIAS/server.key");
        socket->setLocalCertificate("C:/INCIDENCIAS/CERT_INCIDENCIAS/server.crt");
        socket->ignoreSslErrors();

        Client *client = new Client(socket, db, this);
        socket->startServerEncryption();
    }
    else{
        delete socket;
    }

}
