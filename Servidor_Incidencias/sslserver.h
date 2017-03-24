#ifndef SSLSERVER_H
#define SSLSERVER_H



#include <QTcpServer>
#include <QSslSocket>
#include <QSslKey>
#include <QList>
#include <QtSql>
#include <QMap>
#include "client.h"



class SslServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit SslServer(QString dir, quint16 port, QSqlDatabase *db, QObject *parent = 0);
    ~SslServer();



public slots:
public:
    void incomingConnection(qintptr socketDescriptor);
    QTcpServer *server;

private:


    QSqlDatabase* db;

    //QMap<Client*, quintptr> list_clients;
    QString dir_;
    quint16 port_;


};

#endif // SSLSERVER_H
