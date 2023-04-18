#include "ColorPaletteWidget.h"
#include <QVBoxLayout>
#include <QColorDialog>
#include <QLabel>
#include <QPushButton>

ColorPaletteWidget::ColorPaletteWidget(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    // ����һ����ǩ������ʾ��ǰѡ������ɫ
    m_colorLabel = new QLabel(this);
    m_colorLabel->setText("Selected color");

    // ����һ����ť�����ڴ���ɫ�Ի���
    m_colorButton = new QPushButton("Choose color", this);

    // ���Ӱ�ť�� clicked �źŵ�ѡ����ɫ�Ĳۺ���
    connect(m_colorButton, &QPushButton::clicked, this, &ColorPaletteWidget::chooseColor);

    layout->addWidget(m_colorLabel);
    layout->addWidget(m_colorButton);
}

void ColorPaletteWidget::chooseColor()
{
    // ����ɫ�Ի������û�ѡ����ɫ
    QColor color = QColorDialog::getColor(m_currentColor, this, "Choose color");

    // ����û�ѡ������ɫ������µ�ǰ��ɫ���ڱ�ǩ����ʾ
    if (color.isValid())
    {
        m_currentColor = color;
        m_colorLabel->setText("Selected color: " + m_currentColor.name());
        m_colorLabel->setStyleSheet("QLabel { background-color: " + m_currentColor.name() + "; color: white; }");
    }
}
