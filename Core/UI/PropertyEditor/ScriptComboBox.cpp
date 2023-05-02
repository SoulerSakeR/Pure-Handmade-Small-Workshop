#include "ScriptComboBox.h"
#include "Core/ResourceManagement/ResourceMgr.h"

ScriptComboBox::ScriptComboBox(QWidget* parent, Property* property) :AnimationComboBox(nullptr, parent), property(property)
{
	setEditable(true);
	setInsertPolicy(QComboBox::NoInsert);	
	text = property->get_data<string>();
	addItem("Countinue Editing ... ");
	if (!ResourceMgr::get_instance().isExist<ScriptData>(text))
	{
		addItem( (text+"(Not Found)").c_str());
	}
	else
	{
		addItem(text.c_str());
	}
	setCurrentIndex(1);
	lineEdit()->setCompleter(nullptr);	
	connect(lineEdit(), &QLineEdit::editingFinished, [=]() {
		if(text == currentText().toStdString())
			return;
		text = currentText().toStdString();
		updateItems();
		showPopup();
	});
	connect(this, &QComboBox::activated, [this] (int index){
		if (index == 0)
		{
			this->blockSignals(true);
			this->setCurrentIndex(1);
			this->blockSignals(false);
			return;
		}			
		if (index == 1)
		{
			string name = text;
			if (name == "None")
			{
				this->property->set_data<string>(name);
				return;
			}
			else
			{
				if (name.substr(0, 1).find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_") != string::npos)
				{
					Debug::warningBox(this, "Script name can only begin with letters and underscores");
					return;
				}
				else
				{
					this->property->set_data<string>(name);
					auto script = ResourceMgr::get_instance().loadFromName<ScriptData>(name);
					if (script != nullptr)
					{
						QDesktopServices::openUrl(QUrl::fromLocalFile(script->get_path().c_str()));
					}
				}
			}
		}
		else
			this->property->set_data<string>(itemText(index).toStdString());
	});
}

void ScriptComboBox::updateItems()
{
	QSignalBlocker(this);
	int size = count();
	setItemText(1,text.c_str());
	for(int i=size-1;i>=2;--i)
		removeItem(i);
	std::string currentText = text;
	QStringList itemList;
	for (auto& pair : ResourceMgr::get_instance().script_assets)
	{
		auto script = pair.second;
		if (script->get_name().find(currentText) != std::string::npos)
			itemList << QString::fromStdString(script->get_name());
	}
	addItems(itemList);
}
