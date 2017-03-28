#ifndef SOCKETTCP_H
#define SOCKETTCP_H

#include <QObject>
#include <QSslSocket>
#include <QString>
#include <QBuffer>
#include <QHostAddress>
#include <QHostInfo>
#include <QList>
#include <QNetworkInterface>
#include <QAbstractSocket>
#include <QJsonObject>
#include <QJsonDocument>



class SocketTcp : public QObject
{
    Q_OBJECT
public:
    explicit SocketTcp(QString ip_server, quint16 port_server, QString tipo_, QObject *parent = 0);
    QSslSocket* sslSocket;
    QString server_ip;
    QString ip_local;
    quint16 server_port;
    QString tipo;


signals:

public slots:
    void ready();
    void error();
};

#endif // SOCKETTCP_H
