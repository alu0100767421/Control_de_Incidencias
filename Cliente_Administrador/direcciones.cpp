#include "direcciones.h"

Direcciones::Direcciones()
{

}

QString Direcciones::ip() const
{
    return m_ip;
}

QString Direcciones::nombre_equipo() const
{
    return m_equipo;
}

QString Direcciones::ubicacion() const
{
    return m_ubicacion;
}

quint16 Direcciones::type() const
{
    return type_;
}

quint16 Direcciones::subred() const
{
    return m_subred;
}

void Direcciones::setIp(const QString &ip)
{
    m_ip = ip;
}

void Direcciones::setNombre(const QString &nombre)
{
    m_equipo = nombre;
}

void Direcciones::setUbicacion(const QString &ubicacion)
{
    m_ubicacion = ubicacion;
}

void Direcciones::setType(const quint16 &type)
{
    type_ = type;
}

void Direcciones::setSubred(const quint16 &subred)
{
    m_subred = subred;
}
