#include "GameWindow.h"

// 构造函数
GameWindow::GameWindow()
{
	// 设置窗口标题
	setWindowTitle("Game Window");
	// 设置窗口大小
	//setFixedSize(1920, 1080);
	// 设置窗口背景色
	//setStyleSheet("background-color: black;");
	
	
	cWidget = std::make_unique<QWidget>();
	setCentralWidget(cWidget.get());

	auto layout = new QVBoxLayout(cWidget.get());

	renderWidget = std::make_unique<RenderWidget>(this);
	layout->addWidget(renderWidget.get());
	renderWidget->currentWidget = renderWidget.get();
}