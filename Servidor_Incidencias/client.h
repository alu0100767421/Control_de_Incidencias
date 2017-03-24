#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QtSql>
#include <QSslSocket>
#include <QSslError>
#include <QHostAddress>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QPaintDevice>
#include <QDataStream>
#include <QImageReader>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QVector>


#include "direcciones.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QSslSocket* sslSocket, QSqlDatabase *db, QObject *parent = 0);
    Direcciones deserializar();
    ~Client();

signals:


public slots:
    void readyRead();
    void error();

private:
    QSslSocket* sslSocket_;
    quint32 tamPacket;
    QSqlDatabase* db_;
    quintptr socketDescriptor;
    Direcciones dir;


};

#endif // CLIENT_H
