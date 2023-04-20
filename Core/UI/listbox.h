#ifndef LISTBOX_H
#define LISTBOX_H
#include <QtGui>
#include <QListWidget>

class ListBox: public QListWidget
{
    Q_OBJECT

public:
    explicit ListBox(QWidget *parent = nullptr);
    ~ListBox();


protected:

    void dragMoveEvent(QDragMoveEvent* e);
    void dropEvent(QDropEvent* event);
    void startDrag(Qt::DropActions supportedActions);
    void dragEnterEvent(QDragEnterEvent* event);
    Qt::DropAction supportedDropActions();

private:
    friend class mainwindow;

signals:

   void itemDroped();


};

#endif // LISTBOX_H
