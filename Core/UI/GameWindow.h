#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include "Core/Render/renderwidget.h"


class GameWindow : public QMainWindow
{
	friend class RenderWidget;
	std::unique_ptr<QWidget> cWidget;
	std::unique_ptr<RenderWidget> renderWidget;
	std::unique_ptr<QGridLayout> windowLayout;
public:
	GameWindow();
	static GameWindow& get_instance();


public slots:
	void onResizeRenderWidget(bool isFullScreen);

private:
	static GameWindow* instance;
	
};



#endif // GAMEWINDOW_H

