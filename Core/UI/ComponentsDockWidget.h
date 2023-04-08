#pragma once
#include "qdockwidget.h"
#include <Core/Core/GameObject.h>

class ComponentsDockWidget :
    public QDockWidget
{
    Q_OBJECT
public:
    ComponentsDockWidget(QWidget* parent);

    static ComponentsDockWidget* get_instance();
    void set_components_widget(QWidget* widget);
    void clear(); 
    GameObject* get_selected_gameobject();
    void onPropertyChanged(Property* property);

public slots:
    void onGameObjectSelected(GameObject* gameobj);
    void onPropertyInputed(QObject* sender,void* value);
    void onIntChanged(int value);
    void onFloatChanged(double value);
    void onStringChanged();
    void onBoolChanged(bool value);
    void onVector2DChanged(QString value);

private:
    QWidget* components_widget;
    std::map<Property*, QObject*> property_Object_map;
    std::map<QObject*, Property*> Object_Property_map;
    static ComponentsDockWidget* instance;
    GameObject* selected_gameobject;
    Property* changed_property;
    void* changed_value;
};

