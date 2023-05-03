#pragma once
#include "qdockwidget.h"
#include <Core/Core/GameObject.h>
#include "HierarchyWidget.h"
#include "Core/ResourceManagement/Texture2D.h"

class QTimer;

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
    void updatePropertyData(Property* property);

    HierarchyWidget* hierarchy;
    QTimer* timer;
    bool is_refreshing;

public slots:
    void onGameObjectSelected(GameObject* gameobj);
    void onPropertyInputed(QObject* sender,void* value);
    void onTimer();

private:
    QWidget* components_widget;   
    std::map<Property*, QObject*> property_Object_map;
    std::map<QObject*, Property*> Object_Property_map;
    static ComponentsDockWidget* instance;
    GameObject* selected_gameobject;
    Property* changed_property;
    void* changed_value;
    std::vector<Property*> changed_properties_buffer1;
    std::vector<Property*> changed_properties_buffer2;

    friend class MainWindow;
};

