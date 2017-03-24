#include "server.h"


Server::Server(QString dir, quint16 port, QObject *parent) :
    QObject(parent),
    servidor(nullptr),
    dir_(dir),
    port_(port)
{

    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "SQLITE"));
    db->setDatabaseName("incidencias.db");


    if(!db->open()){

        qDebug() << "No se pudo acceder a la base de datos\n";
        exit(1);
    }
    servidor = new SslServer(dir_,port_,db,this);
/*
    QSqlQuery query(*db);

    query.exec("CREATE TABLE IF NOT EXIST login "
                "(usuario VARCHAR(50) PRIMARY KEY,"
                " password VARCHAR(50) NOT NULL,"
                " ip VARCHAR(20) NOT NULL,"
                " port INTEGER NOT NULL))");
*/
}

void Server::start()
{
    servidor->listen(QHostAddress(dir_), port_);
    qDebug() << "Listening...";
    qDebug() << "Port: " << port_;
}

Server::~Server()
{
    delete servidor;
    db->close();
    delete db;
}
