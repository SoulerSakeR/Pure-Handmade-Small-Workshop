#include "PointLight.h"

QColor PointLight::calcIntensity(GameObject* target)
{

	qreal ambient = 0.1; // 环境光系数
	qreal diffuse = 0.8; // 漫反射系数
	
	Vector2D pos = target->transform->getWorldPosition();
	QPointF position(pos.x, pos.y);

	QPointF lightPos = getPosition();
	QColor lightColor = getColor();

	// 计算物体和光源之间的距离和光源衰减因子
	QVector2D distanceVec(getPosition() - position);
	qreal distance = distanceVec.length();
	qreal attenuation = 1.0 / (1.0 + 0.1 * distance + 0.01 * distance * distance);
	qreal radius = getRadius();
	qreal intensity = getIntensity();

	if (distance <= radius) {
		qreal factor = 1.0 - (distance / radius);
		attenuation *= factor * factor;
		intensity *= factor;
	}

	// 计算物体的漫反射
	QVector2D normal(0, -1);
	QVector2D lightDir = (QVector2D(getPosition()) - QVector2D(position)).normalized();
	qreal diffuseFactor = qMax(normal.dotProduct(normal, lightDir), 0.0);

	// 计算物体的光照强度
	qreal intensityOBJ = ambient + diffuse * diffuseFactor * attenuation;
	
	QColor finalColor(intensityOBJ, intensityOBJ, intensityOBJ);

	
	return finalColor;
}
