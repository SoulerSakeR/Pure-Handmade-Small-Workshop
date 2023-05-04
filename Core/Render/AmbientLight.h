#include "LightSource.h"
#include <QPointF>
#include <QColor>

class AmbientLight : public LightSource
{
public:
    AmbientLight(QColor color) : LightSource(QPointF(0, 0), color, LightSourceType::Ambient) {}

    QColor calcIntensity(GameObject* target) override
    {
        return getColor();
    }
};
