#ifndef EXPORTGAMEDIALOG_H
#define EXPORTGAMEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QFileDialog>
#include "Core/SystemStatus/GameEngine.h"

class ExportGameDialog : public QDialog
{
    Q_OBJECT

public:
    ExportGameDialog(QWidget* parent = nullptr);

    QString getGameName() const;
    QString getExportLocation() const;

private slots:
    void browseExportLocation();
    void exportGame();

private:
    QLineEdit* m_gameNameLineEdit;
    QLineEdit* m_exportLocationLineEdit;
    QPushButton* m_browseButton;
    QPushButton* m_exportButton;
};
#endif // EXPORTGAMEDIALOG_H