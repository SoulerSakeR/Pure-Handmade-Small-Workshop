#pragma once
#include "qdockwidget.h"
#include <Core/Core/GameObject.h>
#include "HierarchyWidget.h"

class ComponentsDockWidget :
    public QDockWidget
{
    Q_OBJECT
    friend class ComponentGroupBox;
public:
    ComponentsDockWidget(QWidget* parent);

    static ComponentsDockWidget* get_instance();
    void set_components_widget(QWidget* widget);
    void clear(); 
    GameObject* get_selected_gameobject();
    void onPropertyChanged(Property* property);
    void refresh();

public slots:
    void onGameObjectSelected(GameObject* gameobj);
    void onPropertyInputed(QObject* sender,void* value);
    void onIntChanged(int value);
    void onFloatChanged(double value);
    void onStringChanged();
    void onBoolChanged(bool value);
    void onVector2DChanged(Vector2D value);

private:
    QWidget* components_widget;
    std::map<Property*, QObject*> property_Object_map;
    std::map<QObject*, Property*> Object_Property_map;
    static ComponentsDockWidget* instance;
    GameObject* selected_gameobject;
    Property* changed_property;
    void* changed_value;
};

