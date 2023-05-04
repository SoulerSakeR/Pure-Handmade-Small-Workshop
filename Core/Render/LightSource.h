#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <QPointF>
#include <QColor>

#include "Core/Core/GameObject.h"

enum LightSourceType {
    Point,
    DirectionalLight,
    SpotLight,
    Ambient
};

class LightSource
{
public:
    LightSource(QPointF position, QColor color, LightSourceType type);
    ~LightSource();

    QPointF getPosition() const;
    void setPosition(QPointF position);

    QColor getColor() const;
    void setColor(QColor color);

    LightSourceType getType() const;
    void setType(LightSourceType type);

    virtual QColor calcIntensity(GameObject* target) = 0;

private:
    QPointF m_position;
    QColor m_color;
    LightSourceType m_type;
};





#endif // LIGHTSOURCE_H
