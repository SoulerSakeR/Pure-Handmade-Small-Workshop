#pragma once

#include <QWidget>
#include "Core/ResourceManagement/Texture2D.h"

class QLabel;
class QPushButton;


class ColorPaletteWidget : public QWidget
{
    Q_OBJECT

signals:
    void colorChanged(Color32 color);
public:
    explicit ColorPaletteWidget(QWidget* parent = nullptr);
    Color32 getColor();
    void setColor(Color32 color);

private slots:
    void chooseColor();

private:
    QLabel* m_colorLabel;
    QPushButton* m_colorButton;
    QColor m_currentColor;
    Color32 color;
};

