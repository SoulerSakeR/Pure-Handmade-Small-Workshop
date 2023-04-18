#include "ColorPaletteWidget.h"
#include <QVBoxLayout>
#include <QColorDialog>
#include <QLabel>
#include <QPushButton>

ColorPaletteWidget::ColorPaletteWidget(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    // 创建一个标签用于显示当前选定的颜色
    m_colorLabel = new QLabel(this);
    m_colorLabel->setText("Selected color");

    // 创建一个按钮，用于打开颜色对话框
    m_colorButton = new QPushButton("Choose color", this);

    // 连接按钮的 clicked 信号到选择颜色的槽函数
    connect(m_colorButton, &QPushButton::clicked, this, &ColorPaletteWidget::chooseColor);

    layout->addWidget(m_colorLabel);
    layout->addWidget(m_colorButton);
}

void ColorPaletteWidget::chooseColor()
{
    // 打开颜色对话框，让用户选择颜色
    QColor color = QColorDialog::getColor(m_currentColor, this, "Choose color");

    // 如果用户选择了颜色，则更新当前颜色并在标签上显示
    if (color.isValid())
    {
        m_currentColor = color;
        m_colorLabel->setText("Selected color: " + m_currentColor.name());
        m_colorLabel->setStyleSheet("QLabel { background-color: " + m_currentColor.name() + "; color: white; }");
    }
}
