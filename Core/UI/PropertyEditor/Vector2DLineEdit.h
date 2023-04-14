#pragma once
#include <qlineedit.h>
#include <Core/Utils/Vector2D.h>
class Vector2DLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	Vector2DLineEdit(QWidget* parent = nullptr);
signals:
	void Vector2DChanged(Vector2D vec);
public slots:
	void onEditingFinished();
};

