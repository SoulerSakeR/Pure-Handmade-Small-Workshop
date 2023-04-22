#include "LayerComboBox.h"
#include "Core/ResourceManagement/SceneMgr.h"

LayerComboBox::LayerComboBox(QWidget* parent) :QComboBox(parent)
{
	for (const auto& layer : SceneMgr::get_instance().get_render_setting()->get_render_layers())
	{
		addItem(QString::fromStdString(layer.second));
	}
}
