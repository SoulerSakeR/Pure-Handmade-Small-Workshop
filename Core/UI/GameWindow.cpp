#include "GameWindow.h"

// 构造函数
GameWindow::GameWindow()
{
	// 设置窗口标题
	setWindowTitle("Game Window");
	// 设置窗口大小
	setFixedSize(800, 600);
	// 设置窗口背景色
	//setStyleSheet("background-color: black;");
	// 设置窗口为顶层窗口
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	
	cWidget = std::make_unique<QWidget>();
	setCentralWidget(cWidget.get());

	renderWidget = std::make_unique<RenderWidget>(this);
}