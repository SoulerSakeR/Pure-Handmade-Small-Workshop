#ifndef LISTITEM_H
#define LISTITEM_H
#include <QListWidgetItem>

class ListItem:public QListWidgetItem
{
    std::unique_ptr<QImage> _image;

public:
    ListItem(const QString &imagePath, const QString &text,QListWidget * listview=nullptr);
    ~ListItem();

    QImage* getImage()const{return _image.get();}
};

#endif // LISTITEM_H
