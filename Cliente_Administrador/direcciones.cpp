#include "direcciones.h"

Direcciones::Direcciones()
{
     type_ = 100;
     m_ip = "";
     m_equipo = "";
     m_ubicacion = "";
     m_subred = 0;

    //IMPRESORAS
     m_modelo = "";
     m_marca = "";
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

/****impresoras*****/
QString Direcciones::marcaImpresora() const
{
    return m_marca;
}

QString Direcciones::modeloImpresora() const
{
    return m_modelo;
}
/***************/

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

/****impresoras **/
void Direcciones::setMarcaImpresora(const QString &marca)
{
    m_marca = marca;
}

void Direcciones::setModeloImpresora(const QString &modelo)
{
    m_modelo = modelo;
}
/***********/
