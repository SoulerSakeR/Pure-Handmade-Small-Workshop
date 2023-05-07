#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <QPointF>
#include <QColor>

#include "Core/Core/GameObject.h"


struct LightSourceData {
    QVector2D position; // 光源位置
    QVector3D color;    // 光源颜色
    int type;           // 光源类型
    float intensity;    // 光源强度（0-1）
    float radius;       // 光源半径
};

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
