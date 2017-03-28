#include <QCoreApplication>
#include <QSettings>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <QSslSocket>
#include <fstream>
#include <QList>
#include <cstring>
#include <QSslCertificate>
#include <QSslKey>
#include <QFile>
#include <QDebug>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    quint16 port;
    port = 9000;
    QString ip_option = "127.0.0.1";

    Server server(ip_option, port);
    server.start();

    return a.exec();
}
