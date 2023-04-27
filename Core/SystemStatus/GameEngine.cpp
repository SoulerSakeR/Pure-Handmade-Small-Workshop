#include <filesystem>

#include "GameEngine.h"
#include "../Core/Scene.h"
#include "../Core/Debug.h"
#include "../FileIO/IO.h"
#include "Core/Render/renderwidget.h"
#include <Core/ResourceManagement/SceneMgr.h>
#include <Core/Utils/PHPath.h>
#include <Core/Core/Image.h>
#include <Core/UI/ComponentsDockWidget.h>
#include "Core/ResourceManagement/ResourceMgr.h"
#include "Core/GameLogic/GameLoop.h"

using namespace std;

GameEngine* GameEngine::instance = nullptr;

GameEngine::GameEngine():pool(4)
{
	rootPath = std::filesystem::current_path().string();
	gameLoop = nullptr;
	gameProject = nullptr;
}

/// @brief get singleton instance, if it is not exists, create one 
/// @return the pointer of instance
GameEngine& GameEngine::get_instance()
{
	if(instance==nullptr)
		instance = new GameEngine();
    return *instance;
}

/// @brief initialize game engine and subsystem.
/// @return value that indicates the process was completed or not.
bool GameEngine::initialize(MainWindow* window)
{
	this->inEditor = true;
	Debug::logLevel = Debug::LogLevel::INFO;
	Debug::logInfo()<< "Engine initializing\n";
	srand((unsigned)time(NULL));
	gameProject = nullptr;
	this->window = window;
	Debug::logInfo() << "Engine initialized\n";
    return true;
}

// add by jz
bool GameEngine::initializeGame (GameWindow* window)
{
	// 
	this->inEditor = false;
	Debug::logLevel = Debug::LogLevel::WARNING;
	Debug::logInfo() << "Game initializing\n";
	srand((unsigned)time(NULL));
	gameProject = nullptr;
	this->gameWindow = window;	
	window->show();
	Debug::logInfo() << "Game initialized\n";
	openGameProject(this->gamePathForGameExport);
	return true;
}

GameObject* GameEngine::getSelectedGameObject()
{
	if (ComponentsDockWidget::get_instance() != nullptr)
		return ComponentsDockWidget::get_instance()->get_selected_gameobject();
	return nullptr;
}

void GameEngine::onPropertyChange(Property* property)
{
	if (ComponentsDockWidget::get_instance() != nullptr)
		ComponentsDockWidget::get_instance()->onPropertyChanged(property);
}

void GameEngine::renderLoop()
{
	RenderWidget::getCurrentWidget().update();
}

bool GameEngine::readyToClose()
{
	if(gameLoop->isClosed)
		return true;
	return false;
}

Scene* const  GameEngine::getCurrentScene()
{
	return SceneMgr::get_instance().get_current_scene();
}

GameProject* GameEngine::getCurrentGameProject()
{
	return gameProject;
}

/// @brief creat game project with default scene.
/// @param name project name
/// @param path project absolute path
/// @return the pointer of the project created by name and path
GameProject& GameEngine::creatGameProject(const string& name,const string& path)
{
	// TODO: 创建前检查当前是否已保存项目，如否，弹出窗口询问是否保存当前项目
	// fix path if it contains unnecessary directory
	PHPath new_path(path);
	if (new_path.getFileName() != name)
	{
		new_path =  new_path.combinePath(name);
	}		
	GameProject* game = new GameProject(name, new_path.getNewPath());
	gameProject = game;
	gameProject->render_setting = RenderSetting::getDefaultSetting();
	SceneMgr::get_instance().render_setting = gameProject->render_setting;
	game->openScene(gameProject->creatNewScene());
	return *game;
}
int GameEngine::exportGame(const string& name, const string& path)
{
	QString fullPath = QString::fromStdString(path) + "/" + QString::fromStdString(name);

	// 创建游戏路径
	QDir destDir(fullPath);
	if (!destDir.exists()) {
		destDir.mkpath(".");
	}
	else {
		return 1;
	}

	QDir shadersDir("shaders");
	if (!destDir.exists("shaders")) {
		destDir.mkdir("shaders");
	}
	// 获取shaders目录下所有文件
	QStringList shadersFiles = shadersDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);

	// 复制文件到目标目录
	for (const auto& file : shadersFiles)
	{
		// 获取源文件路径
		QString srcPath = shadersDir.absoluteFilePath(file);

		// 获取目标文件路径
		QString destPath = destDir.absoluteFilePath("shaders/" + file);

		// 复制文件
		QFile::copy(srcPath, destPath);
	}
	
	// 复制iconengines
	QDir iconenginesDir("Engine/iconengines");
	if (!destDir.exists("iconengines")) {
		destDir.mkdir("iconengines");
	}
	QStringList iconenginesFiles = iconenginesDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);

	for (const auto& file : iconenginesFiles)
	{
		QString srcPath = iconenginesDir.absoluteFilePath(file);
		QString destPath = destDir.absoluteFilePath("iconengines/" + file);
		QFile::copy(srcPath, destPath);
	}

	//复制imageformats
	QDir imageformatsDir("Engine/imageformats");
	if (!destDir.exists("imageformats")) {
		destDir.mkdir("imageformats");
	}
	QStringList imageformatsFiles = imageformatsDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);

	for (const auto& file : imageformatsFiles)
	{
		QString srcPath = imageformatsDir.absoluteFilePath(file);
		QString destPath = destDir.absoluteFilePath("imageformats/" + file);
		QFile::copy(srcPath, destPath);
	}

	//复制platforms
	QDir platformsDir("Engine/platforms");
	if (!destDir.exists("platforms")) {
		destDir.mkdir("platforms");
	}
	QStringList platformsFiles = platformsDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);

	for (const auto& file : platformsFiles)
	{
		QString srcPath = platformsDir.absoluteFilePath(file);
		QString destPath = destDir.absoluteFilePath("platforms/" + file);
		QFile::copy(srcPath, destPath);
	}

	//复制styles
	QDir stylesDir("Engine/styles");
	if (!destDir.exists("styles")) {
		destDir.mkdir("styles");
	}
	QStringList stylesFiles = stylesDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);

	for (const auto& file : stylesFiles)
	{
		QString srcPath = stylesDir.absoluteFilePath(file);
		QString destPath = destDir.absoluteFilePath("styles/" + file);
		QFile::copy(srcPath, destPath);
	}

	//复制translations
	QDir translationsDir("Engine/translations");
	if (!destDir.exists("translations")) {
		destDir.mkdir("translations");
	}
	QStringList translationsFiles = translationsDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);

	for (const auto& file : translationsFiles)
	{
		QString srcPath = translationsDir.absoluteFilePath(file);
		QString destPath = destDir.absoluteFilePath("translations/" + file);
		QFile::copy(srcPath, destPath);
	}

	//复制Engine
	QDir engineDir("Engine");

	QStringList engineFiles = engineDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);

	for (const auto& file : engineFiles)
	{
		QString srcPath = engineDir.absoluteFilePath(file);
		QString destPath = destDir.absoluteFilePath(file);
		QFile::copy(srcPath, destPath);
	}
	std::rename((fullPath.toStdString() + "/PHEngine.exe").c_str(), (fullPath.toStdString() +"/"+ name + ".exe").c_str());
	// 创建 config.txt
	if (QFile::exists(fullPath + "/config.txt")) {
		QFile::remove(fullPath + "/config.txt");
	}
	QFile file(fullPath + "/config.txt");
	if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QTextStream out(&file);
		out << "1\n";
		std::string wholePath = GameEngine::get_instance().getGamePath() + "/" + GameEngine::get_instance().getCurrentGameProject()->name+".gameProject";
		out << wholePath.c_str();
		file.close();
	}
	return 0;
}
Vector2D GameEngine::get_resolution()
{
	return SceneMgr::get_instance().get_render_setting()->getCurrentResolution();
}
void GameEngine::set_resolution(const Vector2D& resolution)
{
	SceneMgr::get_instance().render_setting->setCurrentResolution(resolution);
}
void GameEngine::refreshHierarchy()
{
	if(GameEngine::get_instance().getInEditor())
		window->refreshHierachy();
}
bool GameEngine::needToRefeshUI(GameObject* gameobj)
{
	if (getSelectedGameObject() !=gameobj)
		return false;
	if(RenderWidget::getGameWidget().frameCount%5==0)
		return true;
	return false;
}
#ifdef TEST
bool GameEngine::openGameProjectTest(const std::string& project, const std::string** scenes)
{
	delete gameProject;
	stringstream ss(project);
	GameProject* gp = new GameProject("", "", false);
	gp->deserializeTest(ss,scenes);	
	gameProject = gp;
	gameProject->open(0);
	return true;
}
#endif // TEST

MainWindow* GameEngine::getWindow()
{
	return window;
}

GameWindow* GameEngine::getGameWindow()
{
	return gameWindow;
}
/// @brief open exist gameProject
/// @param path the obsolute path of .gameProject file
/// @return value that indicates whether the process was completedf
bool GameEngine::openGameProject(const string& path)
{
	Debug::logInfo()<< "openGameProject:" << path << "\n";
	const string& gameProject = IO::readText(path);
	stringstream ss(gameProject);
	GameProject* gp = new GameProject("","",false);
	gp->deserialize(ss);
	this->gameProject = gp;
	SceneMgr::get_instance().render_setting = this->gameProject->render_setting;
	ResourceMgr::get_instance().loadAllAssets();
	gp->openScene(0);
	return true;
}

/// @brief save game project to hard disk
/// @return value that indicates the process was completed or not
bool GameEngine::saveGameProject()
{
	if(gameProject!=nullptr)
	{		
		return gameProject->save();
	}
    return false;
}

/// @brief add new game object to scene or parent game object 
/// @param parent the pointer of parent game object or null for scene
/// @param name name of the new game object
/// @return the pointer of new game object
GameObject& GameEngine::addGameObject(const string& name, GameObject* const parent,Component::ComponentType type,InsertMode insertMode)
{
	Debug::logInfo()<< "addGameObject:" << name << "\n";
	GameObject* gameObject = new GameObject(name);
    if(parent == nullptr)
		GameEngine::get_instance().gameProject->currentScene->insertGameObject(*gameObject);
	else
		GameEngine::get_instance().gameProject->currentScene->insertGameObject(*gameObject,parent,insertMode);
	switch (type)
	{
	case Component::UNKNOWN:
		break;
	case Component::TRANSFORM:
		break;
	case Component::IMAGE:
		gameObject->addComponent<Image>();
		gameObject->addComponent<Renderer>();
		break;
	case Component::CAMERA:
		gameObject->addComponent<Camera>();
		gameObject->addComponent<Renderer>();
		break;
	case Component::SCRIPT:
		break;
	case Component::BOX_COLLIDER:
		gameObject->addComponent<BoxCollider>();
		gameObject -> addComponent<Renderer>();
		break;
	default:
		break;
	}
	if (inEditor)
	{
		hierarchy->addGameObject(gameObject, parent, insertMode);
	}
	return *gameObject;
}

void GameEngine::deleteGameObject(GameObject* obj)
{
	getCurrentScene()->removeGameObject(obj);
}

const std::string& GameEngine::getRootPath()
{
	return rootPath;
}

std::string GameEngine::getGamePath()
{
	if (gameProject != nullptr)
		return gameProject->path.getNewPath();
	return "";
}

bool GameEngine::getInEditor()
{
	return inEditor;
}

void GameEngine::initGamePathForExport(const std::string& path)
{
	this->gamePathForGameExport = path;
}

bool GameEngine::needToSave()
{
	if(gameProject==nullptr)
		return false;
	return gameProject->isChanged();
}
