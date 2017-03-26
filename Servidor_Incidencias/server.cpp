#include "server.h"


Server::Server(QString dir, quint16 port, QObject *parent) :
    QObject(parent),
    servidor(nullptr),
    dir_(dir),
    port_(port)
{

    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "SQLITE"));
    db->setDatabaseName("C:/INCIDENCIAS/BBDD/incidencias.db");


    if(!db->open()){

        qDebug() << "No se pudo acceder a la base de datos\n";
        exit(1);
    }
    servidor = new SslServer(dir_,port_,db,this);
}

void Server::start()
{
    servidor->listen(QHostAddress(dir_), port_);
    qDebug() << "Listening...";
    qDebug() << "Dir: " << dir_;
    qDebug() << "Port: " << port_;
}

Server::~Server()
{
    delete servidor;
    db->close();
    delete db;
}
