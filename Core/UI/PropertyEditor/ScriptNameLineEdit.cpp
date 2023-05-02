#include "ScriptNameLineEdit.h"
#include "Core/Core/Property.h"
#include "Core/Core/Debug.h"
#include <qcompleter.h>
#include <Core/ResourceManagement/ResourceMgr.h>

using namespace std;

ScriptNameLineEdit::ScriptNameLineEdit(QWidget* parent, Property* property): QLineEdit(parent),property(property)
{
	setText(property->get_data<string>().c_str());
	word_list << "None";
	for (auto& pair : ResourceMgr::get_instance().script_assets)
	{
		auto script = pair.second;
		word_list<< script->get_name().c_str();
	}
	QCompleter* completer = new QCompleter(word_list, this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	setCompleter(completer);
	connect(this, &QLineEdit::editingFinished, [=]() {
		string name = this->text().toStdString();
		if (name == "None")
		{
			property->set_data<string>(name);
			return;
		}
		else
		{
			if (name.substr(0,1).find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_") != string::npos)
			{
				Debug::warningBox(this, "Script name can only begin with letters and underscores");
				return;
			}
			else if (ResourceMgr::get_instance().isExist<ScriptData>(name))
			{
				property->set_data<string>(name);
				return;
			}
			else
			{
				property->set_data<string>(name);
				auto script = ResourceMgr::get_instance().loadFromName<ScriptData>(name);
				if (script != nullptr)
				{
					word_list << name.c_str();
					((QStringListModel*)this->completer()->model())->setStringList(word_list);
					QDesktopServices::openUrl(QUrl::fromLocalFile(script->get_path().c_str()));
				}
			}
		}
	});
}
