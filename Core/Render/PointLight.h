#include "Core/Render/LightSource.h"
#include <QPointF>
#include <QColor>

class PointLight : public LightSource {
public:
    PointLight(QPointF position, QColor color, qreal radius, qreal intensity) :
        LightSource(position, color, Point),
        m_radius(radius),
        m_intensity(intensity) {}

    qreal getRadius() const { return m_radius; }
    void setRadius(qreal radius) { m_radius = radius; }

    qreal getIntensity() const { return m_intensity; }
    void setIntensity(qreal intensity) { m_intensity = intensity; }

    virtual QColor calcIntensity(GameObject* target) override;

private:
    qreal m_radius;
    qreal m_intensity;
};
