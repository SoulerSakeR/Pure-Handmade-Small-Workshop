#include "Core/UI/CreateProjectDialog.h"

CreateProjectDialog::CreateProjectDialog(QWidget* parent)
    : QDialog(parent)
{
    QLabel* projectNameLabel = new QLabel(tr("Project name:"));
    m_projectNameLineEdit = new QLineEdit;

    QLabel* projectLocationLabel = new QLabel(tr("Project location:"));
    m_projectLocationLineEdit = new QLineEdit;
    m_browseButton = new QPushButton(tr("Browse"));
    m_createButton = new QPushButton(tr("Create Project"));

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(projectNameLabel, 0, 0);
    layout->addWidget(m_projectNameLineEdit, 0, 1);
    layout->addWidget(projectLocationLabel, 1, 0);
    layout->addWidget(m_projectLocationLineEdit, 1, 1);
    layout->addWidget(m_browseButton, 1, 2);
    layout->addWidget(m_createButton, 2, 1);

    connect(m_browseButton, &QPushButton::clicked, this, &CreateProjectDialog::browseProjectLocation);
    connect(m_createButton, &QPushButton::clicked, this, &CreateProjectDialog::createProject);

    setLayout(layout);
}


QString CreateProjectDialog::getProjectName() const
{
    return m_projectNameLineEdit->text();
}

QString CreateProjectDialog::getProjectLocation() const
{
    return m_projectLocationLineEdit->text();
}

void CreateProjectDialog::browseProjectLocation()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select directory"), QDir::homePath());
    if (!directory.isEmpty())
        m_projectLocationLineEdit->setText(directory);
}

void CreateProjectDialog::createProject()
{

}