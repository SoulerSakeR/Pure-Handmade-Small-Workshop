#pragma once

#include <QWidget>

class QLabel;
class QPushButton;

class ColorPaletteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorPaletteWidget(QWidget* parent = nullptr);

private slots:
    void chooseColor();

private:
    QLabel* m_colorLabel;
    QPushButton* m_colorButton;
    QColor m_currentColor;
};

