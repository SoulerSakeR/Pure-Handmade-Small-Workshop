#pragma once
#include <qdialog.h>
#include "Core/ResourceManagement/Texture2D.h"

class TextureEditorDialog : public QDialog
{
	Q_OBJECT
public:
	TextureEditorDialog(Texture2D* texture, QWidget* parent = nullptr);

private:
	Texture2D* texture;
};

