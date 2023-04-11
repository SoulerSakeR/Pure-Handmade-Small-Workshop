#pragma once
#include <qpushbutton.h>
#include "Core/Core/Component.h"
class DeleteComponentButton : public QPushButton
{
    Q_OBJECT
public:
    DeleteComponentButton(QWidget* parent = Q_NULLPTR,Component* component = nullptr);
    Component* component;

public slots:
    void deleteComponent();
};

