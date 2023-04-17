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

    QLabel* outputWidthLabel = new QLabel(tr("Output width:"));
    m_outputWidthLineEdit = new QLineEdit;
    
    QLabel* outputHeightLabel = new QLabel(tr("Output height:"));
    m_outputHeightLineEdit = new QLineEdit;

    QLabel* patternSizeLabel = new QLabel(tr("Pattern size:"));
    m_patternSizeLineEdit = new QLineEdit;

    QLabel* symmetryLabel = new QLabel(tr("Number of symmetries:"));
    m_symmetryLineEdit = new QLineEdit;

    QLabel* screenshotsLabel = new QLabel(tr("Number of output images:"));
    m_screenshotsLineEdit = new QLineEdit;

    m_browseImageButton = new QPushButton(tr("Browse"));
    m_browseOutputButton = new QPushButton(tr("Browse"));
    m_runwfcButton = new QPushButton(tr("Run"));


    m_optionsGroupBox = new QGroupBox(tr("Options:"));
    m_periodicInputCheckBox = new QCheckBox(tr("Periodicity of Input"));
    m_periodicOutputCheckBox = new QCheckBox(tr("Periodicity of Output"));
    m_groundCheckBox = new QCheckBox(tr("Ground"));
    m_periodicInputCheckBox->setChecked(true);

    m_heuristicGroupBox = new QGroupBox(tr("Heuristic"));
    m_entropyRadioButton = new QRadioButton(tr("Entropy"));
    m_scanlineRadioButton = new QRadioButton(tr("Scanline"));
    m_MRVRadioButton = new QRadioButton(tr("MRV"));
    m_entropyRadioButton->setChecked(true); // set the default checked option

    QVBoxLayout* optionsGroupBoxLayout = new QVBoxLayout;
    optionsGroupBoxLayout->addWidget(m_periodicInputCheckBox);
    optionsGroupBoxLayout->addWidget(m_periodicOutputCheckBox);
    optionsGroupBoxLayout->addWidget(m_groundCheckBox);
    m_optionsGroupBox->setLayout(optionsGroupBoxLayout);

    QHBoxLayout* heuristicGroupBoxLayout = new QHBoxLayout;
    heuristicGroupBoxLayout->addWidget(m_entropyRadioButton);
    heuristicGroupBoxLayout->addWidget(m_scanlineRadioButton);
    heuristicGroupBoxLayout->addWidget(m_MRVRadioButton);
    m_heuristicGroupBox->setLayout(heuristicGroupBoxLayout);


    QGridLayout* layout = new QGridLayout;
    layout->addWidget(imageLocationLabel, 0, 0);
    layout->addWidget(m_imageLocationLineEdit, 0, 1, 1, 2);
    layout->addWidget(m_browseImageButton, 0, 3);

    layout->addWidget(projectLocationLabel, 1, 0);
    layout->addWidget(m_outputLocationLineEdit, 1, 1, 1, 2);
    layout->addWidget(m_browseOutputButton, 1, 3);

    layout->addWidget(outputWidthLabel, 2, 0);
    layout->addWidget(m_outputWidthLineEdit, 2, 1);
    layout->addWidget(outputHeightLabel, 2, 2);
    layout->addWidget(m_outputHeightLineEdit, 2, 3);

    layout->addWidget(patternSizeLabel, 3, 0);
    layout->addWidget(m_patternSizeLineEdit, 3, 1);
    layout->addWidget(symmetryLabel, 3, 2);
    layout->addWidget(m_symmetryLineEdit, 3, 3);

    layout->addWidget(screenshotsLabel, 4, 0);
    layout->addWidget(m_screenshotsLineEdit, 4, 1);

    layout->addWidget(m_optionsGroupBox, 5, 0, 1, 4); // add the group box to the layout

    layout->addWidget(m_heuristicGroupBox, 6, 0, 1, 4);

    layout->addWidget(m_runwfcButton, 7, 1);


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
    return m_outputWidthLineEdit->text().toUInt();
}

uint32_t WaveFunctionCollapseDialog::getHeight() const
{
    return m_outputHeightLineEdit->text().toUInt();
}

uint32_t WaveFunctionCollapseDialog::getN() const
{
    return m_patternSizeLineEdit->text().toUInt();
}

bool WaveFunctionCollapseDialog::getPeriodicOutput() const
{
    return m_periodicOutputCheckBox->isChecked();
}

bool WaveFunctionCollapseDialog::getPeriodicInput() const
{
    return m_periodicInputCheckBox->isChecked();
}

bool WaveFunctionCollapseDialog::getGround() const
{
    return m_groundCheckBox->isChecked();
}

uint32_t WaveFunctionCollapseDialog::getSymmetry() const
{
    return m_symmetryLineEdit->text().toUInt();
}

uint32_t WaveFunctionCollapseDialog::getScreenshots() const
{
    return m_screenshotsLineEdit->text().toUInt();
}

QString WaveFunctionCollapseDialog::getHeuristic() const
{
    if(m_MRVRadioButton->isChecked())
        return QString("MRV");
    else if (m_scanlineRadioButton->isChecked())
        return QString("Scanline");
    else
        return QString("Entropy");
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
    std::string heuristic = getHeuristic().toStdString();
    // 创建一个模态对话框
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Running"));
    msgBox.setText(tr("The program is running, please wait..."));
    msgBox.setStandardButtons(QMessageBox::NoButton);
    msgBox.show();
    QApplication::processEvents();

    // 调用run_overlapping函数
	WaveFunctionCollapse::run_overlapping(imagePath, outputPath + "/", getWidth(), getHeight(), getN(), getPeriodicOutput(), getPeriodicInput(), getGround(), getSymmetry(), getScreenshots(), heuristic);

    // 关闭对话框并显示一个成功消息
    msgBox.close();
    QMessageBox::information(this, tr("Success"), tr("The program has finished running successfully!"));
    close();
}

