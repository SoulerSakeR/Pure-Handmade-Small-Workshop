#include "lightsource.h"

LightSource::LightSource(QPointF position, QColor color, LightSourceType type)
    : m_position(position), m_color(color), m_type(type)
{

}

LightSource::~LightSource()
{

}

QPointF LightSource::getPosition() const
{
    return m_position;
}

void LightSource::setPosition(QPointF position)
{
    m_position = position;
}

QColor LightSource::getColor() const
{
    return m_color;
}

void LightSource::setColor(QColor color)
{
    m_color = color;
}

LightSourceType LightSource::getType() const
{
    return m_type;
}

void LightSource::setType(LightSourceType type)
{
    m_type = type;
}
