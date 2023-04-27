#include "GameWindow.h"

GameWindow* GameWindow::instance = nullptr;

// 构造函数
GameWindow::GameWindow()
{
	
	// 设置窗口标题
	setWindowTitle("Game Window");
	renderWidget = std::make_unique<RenderWidget>(this);
	cWidget = std::make_unique<QWidget>();
	setCentralWidget(cWidget.get());
	resize(1280, 720);
	
	auto layout = new QVBoxLayout(cWidget.get());
	layout->addWidget(renderWidget.get());
	
	renderWidget->currentWidget = renderWidget.get();

	// 连接信号和槽函数
	connect(renderWidget.get(), &RenderWidget::resizeGameWindow, this, &GameWindow::onResizeRenderWidget);
}

GameWindow& GameWindow::get_instance()
{
	if (instance == nullptr)
		instance = new GameWindow();
	return *instance;
}

void GameWindow::onResizeRenderWidget(bool isFullScreen) {
	// 设置 RenderWidget 的大小

	if (isFullScreen)
	{
		setWindowState(windowState() & ~Qt::WindowFullScreen);
	}	
	else
	{
		setWindowState(windowState() | Qt::WindowFullScreen);
	}
		
}


void GameWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Return && event->modifiers() == Qt::AltModifier) {
		
		std::cout << "full" << std::endl;
		renderWidget->setFullScreen(renderWidget->isFullScreen);
	}
}

