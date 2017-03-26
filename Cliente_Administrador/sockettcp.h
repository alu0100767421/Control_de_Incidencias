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

class SocketTcp : public QObject
{
    Q_OBJECT
public:
    explicit SocketTcp(QString ip_server, quint16 port_server, QObject *parent = 0);
    QSslSocket* sslSocket;
    QString server_ip;
    QString ip_local;
    quint16 server_port;


signals:

public slots:
    void ready();
    void error();
};

#endif // SOCKETTCP_H
