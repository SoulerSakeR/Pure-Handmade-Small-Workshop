#ifndef CREATEPROJECTDIALOG_H
#define CREATEPROJECTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QFileDialog>

class CreateProjectDialog : public QDialog
{
    Q_OBJECT

public:
    CreateProjectDialog(QWidget* parent = nullptr);

    QString getProjectName() const;
    QString getProjectLocation() const;

private slots:
    void browseProjectLocation();
    void createProject();

private:
    QLineEdit* m_projectNameLineEdit;
    QLineEdit* m_projectLocationLineEdit;
    QPushButton* m_browseButton;
    QPushButton* m_createButton;
};

#endif // CREATEPROJECTDIALOG_H