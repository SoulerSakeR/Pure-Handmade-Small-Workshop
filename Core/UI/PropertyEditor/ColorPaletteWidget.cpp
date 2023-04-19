#include "ColorPaletteWidget.h"
#include <QVBoxLayout>
#include <QColorDialog>
#include <QLabel>
#include <QPushButton>

ColorPaletteWidget::ColorPaletteWidget(QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);

    // ����һ����ǩ������ʾ��ǰѡ������ɫ
    m_colorLabel = new QLabel(this);
    m_colorLabel->setText("");
    m_colorLabel->setFixedWidth(23);

    // ����һ����ť�����ڴ���ɫ�Ի���
    m_colorButton = new QPushButton("Choose color", this);

    // ���Ӱ�ť�� clicked �źŵ�ѡ����ɫ�Ĳۺ���
    connect(m_colorButton, &QPushButton::clicked, this, &ColorPaletteWidget::chooseColor);

    layout->addWidget(m_colorLabel);
    layout->addWidget(m_colorButton);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(30);
}

Color32 ColorPaletteWidget::getColor()
{
    return Color32(m_currentColor.red(), m_currentColor.green(), m_currentColor.blue(), m_currentColor.alpha());
}

void ColorPaletteWidget::setColor(Color32 color)
{
    m_currentColor = QColor(color.r, color.g, color.b, color.a);
    m_colorLabel->setStyleSheet("QLabel { background-color: " + m_currentColor.name() + "; color: white; }");
}

void ColorPaletteWidget::chooseColor()
{
    // ����ɫ�Ի������û�ѡ����ɫ
    QColor color = QColorDialog::getColor(m_currentColor, this, "Choose color",QColorDialog::ShowAlphaChannel);

    // ����û�ѡ������ɫ������µ�ǰ��ɫ���ڱ�ǩ����ʾ
    if (color.isValid())
    {
        m_currentColor = color;
        //m_colorLabel->setText("Selected color: " + m_currentColor.name());
        m_colorLabel->setStyleSheet("QLabel { background-color: " + m_currentColor.name() + "; color: white; }");
        emit colorChanged(getColor());
    }
}
