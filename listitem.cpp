#include "listitem.h"
#include <QIcon>
#include <QImage>

ListItem::ListItem(const QString &imagePath, const QString &text,QListWidget * listview)
    :QListWidgetItem(QIcon(imagePath),text,listview)
{
    _image=std::make_unique<QImage>(imagePath);
}
ListItem::~ListItem()
{
}
