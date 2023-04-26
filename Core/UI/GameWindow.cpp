#include "GameWindow.h"

GameWindow* GameWindow::instance = nullptr;

// 构造函数
GameWindow::GameWindow()
{
	// 设置窗口标题
	setWindowTitle("Game Window");
	
	cWidget = std::make_unique<QWidget>();
	setCentralWidget(cWidget.get());

	auto layout = new QVBoxLayout(cWidget.get());


	// 获取当前屏幕的大小
	QScreen* screen = QGuiApplication::primaryScreen();
	QRect screenGeometry = screen->geometry();
	int screenWidth = screenGeometry.width();
	int screenHeight = screenGeometry.height();

	renderWidget = std::make_unique<RenderWidget>(this);
	layout->addWidget(renderWidget.get());
	renderWidget->currentWidget = renderWidget.get();
	//renderWidget->setFixedSize(screenWidth, screenHeight);

	// 设置renderWidget全屏显示
	renderWidget->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	renderWidget->showFullScreen();

	


}

GameWindow& GameWindow::get_instance()
{
	if (instance == nullptr)
		instance = new GameWindow();
	return *instance;
}
