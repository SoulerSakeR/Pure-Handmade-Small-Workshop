#include "Vector2DLineEdit.h"

Vector2DLineEdit::Vector2DLineEdit(QWidget* parent) :QLineEdit(parent)
{
	connect(this, &Vector2DLineEdit::editingFinished, this, &Vector2DLineEdit::onEditingFinished);
}

void Vector2DLineEdit::onEditingFinished()
{
	auto str = text().toStdString();
	emit Vector2DChanged(Vector2D::fromString(str));
}
