#ifndef DIRECCIONES_H
#define DIRECCIONES_H

#include <QObject>

class Direcciones
{

public:
    explicit Direcciones();

    QString ip() const;
    QString nombre_equipo() const;
    QString ubicacion() const;
    quint16 type() const;

    void setIp(const QString& ip);
    void setNombre(const QString& nombre);
    void setUbicacion(const QString& ubicacion);
    void setType(const quint16& type);

private:
    quint16 type_;
    QString m_ip;
    QString m_equipo;
    QString m_ubicacion;


};

#endif // DIRECCIONES_H
