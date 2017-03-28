#ifndef DIRECCIONES_H
#define DIRECCIONES_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>


class Direcciones
{

public:
    explicit Direcciones();

    QString ip() const;
    QString nombre_equipo() const;
    QString ubicacion() const;
    quint16 type() const;
    quint16 subred() const;
    QString marcaImpresora() const;
    QString modeloImpresora() const;

    void setIp(const QString& ip);
    void setNombre(const QString& nombre);
    void setUbicacion(const QString& ubicacion);
    void setType(const quint16& type);
    void setSubred(const quint16& subred);
    void setMarcaImpresora(const QString& marca);
    void setModeloImpresora(const QString& modelo);


private:
    quint16 type_;
    QString m_ip;
    QString m_equipo;
    QString m_ubicacion;
    quint16 m_subred;

    //IMPRESORAS
    QString m_modelo;
    QString m_marca;



};

#endif // DIRECCIONES_H
