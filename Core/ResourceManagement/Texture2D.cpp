#include "Texture2D.h"
#include <Core/Utils/PHPath.h>
#include <Core/SystemStatus/GameEngine.h>

using namespace std;
Texture2D::Texture2D(const std::string& path):path(path)
{
	PHPath p(GameEngine::get_instance().getRootPath());
	QString fullPath = QString::fromStdString(p.combinePath(path).getNewPath());
	texture = new QOpenGLTexture(QImage(fullPath).mirrored());
}

void Texture2D::serialize(PHString& str)
{
}

void Texture2D::deserialize(std::stringstream& ss)
{
}
