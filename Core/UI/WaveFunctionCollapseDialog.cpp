#include "WaveFunctionCollapseDialog.h"
#include "Core/WaveFunctionCollapse/WaveFunctionCollapse.h"
#include <QMessageBox>
#include <qapplication.h>

  

WaveFunctionCollapseDialog::WaveFunctionCollapseDialog(QWidget* parent)
    : QDialog(parent)
{
    QLabel* imageLocationLabel = new QLabel(tr("Image Location:"));
    m_imageLocationLineEdit = new QLineEdit;

    QLabel* projectLocationLabel = new QLabel(tr("Output location:"));
    m_outputLocationLineEdit = new QLineEdit;

    m_browseImageButton = new QPushButton(tr("Browse"));
    m_browseOutputButton = new QPushButton(tr("Browse"));
    m_runwfcButton = new QPushButton(tr("Run"));

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(imageLocationLabel, 0, 0);
    layout->addWidget(m_imageLocationLineEdit, 0, 1);
    layout->addWidget(m_browseImageButton, 0, 2);
    layout->addWidget(projectLocationLabel, 1, 0);
    layout->addWidget(m_outputLocationLineEdit, 1, 1);
    layout->addWidget(m_browseOutputButton, 1, 2);
    layout->addWidget(m_runwfcButton, 2, 1);


    connect(m_browseImageButton, &QPushButton::clicked, this, &WaveFunctionCollapseDialog::browseImageLocation);
    connect(m_browseOutputButton, &QPushButton::clicked, this, &WaveFunctionCollapseDialog::browseOutputLocation);
    connect(m_runwfcButton, &QPushButton::clicked, this, &WaveFunctionCollapseDialog::runwfc);

    setLayout(layout);
}

QString WaveFunctionCollapseDialog::getImagePath() const
{
    return m_imageLocationLineEdit->text();
}

QString WaveFunctionCollapseDialog::getOutputPath() const
{
    return m_outputLocationLineEdit->text();
}

uint32_t WaveFunctionCollapseDialog::getWidth() const
{
    return uint32_t();
}

uint32_t WaveFunctionCollapseDialog::getHeight() const
{
    return uint32_t();
}

uint32_t WaveFunctionCollapseDialog::getN() const
{
    return uint32_t();
}

bool WaveFunctionCollapseDialog::getPeriodicOutput() const
{
    return false;
}

bool WaveFunctionCollapseDialog::getPeriodicInput() const
{
    return false;
}

bool WaveFunctionCollapseDialog::getGround() const
{
    return false;
}

uint32_t WaveFunctionCollapseDialog::getSymmetry() const
{
    return uint32_t();
}

uint32_t WaveFunctionCollapseDialog::getScreenshots() const
{
    return uint32_t();
}

QString WaveFunctionCollapseDialog::getHeuristic() const
{
    return QString();
}

void WaveFunctionCollapseDialog::browseImageLocation()
{
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Select directory"), QDir::homePath());
    if (!imagePath.isEmpty())
        m_imageLocationLineEdit->setText(imagePath);
}

void WaveFunctionCollapseDialog::browseOutputLocation()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select directory"), QDir::homePath());
    if (!directory.isEmpty())
        m_outputLocationLineEdit->setText(directory);
}

void WaveFunctionCollapseDialog::runwfc()
{
    std::string imagePath = getImagePath().toStdString();
    std::string outputPath = getOutputPath().toStdString();

    // 创建一个模态对话框
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Running"));
    msgBox.setText(tr("The program is running, please wait..."));
    msgBox.setStandardButtons(QMessageBox::NoButton);
    msgBox.show();
    QApplication::processEvents();

    // 调用run_overlapping函数
    WaveFunctionCollapse::run_overlapping(imagePath, outputPath + "/");

    // 关闭对话框并显示一个成功消息
    msgBox.close();
    QMessageBox::information(this, tr("Success"), tr("The program has finished running successfully!"));
    close();
}

