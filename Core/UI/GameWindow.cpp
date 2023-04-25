#include "GameWindow.h"

// ���캯��
GameWindow::GameWindow()
{
	// ���ô��ڱ���
	setWindowTitle("Game Window");
	// ���ô��ڴ�С
	setFixedSize(800, 600);
	// ���ô��ڱ���ɫ
	setStyleSheet("background-color: black;");
	// ���ô���Ϊ���㴰��
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	
	cWidget = std::make_unique<QWidget>();
	setCentralWidget(cWidget.get());

	renderWidget = std::make_unique<RenderWidget>(this);
}