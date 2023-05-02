#pragma once
#include <qlineedit.h>
#include <qstringlistmodel.h>

class Property;

class ScriptNameLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	ScriptNameLineEdit(QWidget* parent,Property* property);

private:
	Property* property;
	QStringList word_list;
};

