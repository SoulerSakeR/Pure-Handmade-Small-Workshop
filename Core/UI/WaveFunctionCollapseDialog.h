#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QFileDialog>
#include <QProgressDialog>
#include <QRadioButton>
#include <qgroupbox.h>
#include <qcheckbox.h>

class WaveFunctionCollapseDialog : public QDialog
{
    Q_OBJECT

public:
    WaveFunctionCollapseDialog(QWidget* parent = nullptr);

    QString getImagePath() const;
    QString getOutputPath() const;
    uint32_t getWidth() const;
    uint32_t getHeight() const;
    uint32_t getN() const;
    bool getPeriodicOutput() const;
    bool getPeriodicInput() const;
    bool getGround() const;
    uint32_t getSymmetry() const;
    uint32_t getScreenshots() const;
    QString getHeuristic() const;

private slots:
    void browseImageLocation();
    void browseOutputLocation();
    void runwfc();

private:
    QLineEdit* m_imageLocationLineEdit;
    QLineEdit* m_outputLocationLineEdit;
    QLineEdit* m_outputWidthLineEdit;
    QLineEdit* m_outputHeightLineEdit;
    QLineEdit* m_patternSizeLineEdit;
    QLineEdit* m_symmetryLineEdit;
    QLineEdit* m_screenshotsLineEdit;
    QPushButton* m_browseImageButton;
    QPushButton* m_browseOutputButton;
    QPushButton* m_runwfcButton;
    QGroupBox* m_optionsGroupBox;
    QCheckBox* m_periodicOutputCheckBox;
    QCheckBox* m_periodicInputCheckBox;
    QCheckBox* m_groundCheckBox;
    QGroupBox* m_heuristicGroupBox;
    QRadioButton* m_entropyRadioButton;
    QRadioButton* m_scanlineRadioButton;
    QRadioButton* m_MRVRadioButton;
};