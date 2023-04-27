#include "Core/UI/ExportGameDialog.h"

ExportGameDialog::ExportGameDialog(QWidget* parent)
    : QDialog(parent)
{
    QLabel* gameNameLabel = new QLabel(tr("Game name:"));
    m_gameNameLineEdit = new QLineEdit;

    QLabel* exportLocationLabel = new QLabel(tr("Export location:"));
    m_exportLocationLineEdit = new QLineEdit;
    m_browseButton = new QPushButton(tr("Browse"));
    m_exportButton = new QPushButton(tr("Export Game"));

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(gameNameLabel, 0, 0);
    layout->addWidget(m_gameNameLineEdit, 0, 1);
    layout->addWidget(exportLocationLabel, 1, 0);
    layout->addWidget(m_exportLocationLineEdit, 1, 1);
    layout->addWidget(m_browseButton, 1, 2);
    layout->addWidget(m_exportButton, 2, 1);

    connect(m_browseButton, &QPushButton::clicked, this, &ExportGameDialog::browseExportLocation);
    connect(m_exportButton, &QPushButton::clicked, this, &ExportGameDialog::exportGame);

    setLayout(layout);
}

QString ExportGameDialog::getGameName() const
{
    return m_gameNameLineEdit->text();
}

QString ExportGameDialog::getExportLocation() const
{
    return m_exportLocationLineEdit->text();
}

void ExportGameDialog::browseExportLocation()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select directory"), QDir::homePath());
    if (!directory.isEmpty())
        m_exportLocationLineEdit->setText(directory);
}

void ExportGameDialog::exportGame()
{
    int result = GameEngine::get_instance().exportGame(getGameName().toStdString(), getExportLocation().toStdString());
    if (result == 0) {
        QMessageBox::information(this, tr("Export Successful"), tr("The game was exported successfully!"));
    }
    else {
        QMessageBox::critical(this, tr("Export Failed"), tr("The game export failed. The directory already exists."));
    }
    close();
}
