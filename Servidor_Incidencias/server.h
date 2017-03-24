#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlDriver>
#include <iostream>

#include "sslserver.h"



class Server : public QObject
{
    Q_OBJECT
public:
    Server(QString dir, quint16 port, QObject *parent = 0);
    void start();
    ~Server();

signals:

public slots:

public:
    QSqlDatabase *db;

private:
    SslServer *servidor;

    QString dir_;
    quint16 port_;


};

#endif // SERVER_H
