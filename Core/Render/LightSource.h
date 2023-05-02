#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <QPointF>
#include <QColor>

enum LightSourceType {
    PointLight,
    DirectionalLight,
    SpotLight
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

private:
    QPointF m_position;
    QColor m_color;
    LightSourceType m_type;
};

#endif // LIGHTSOURCE_H
