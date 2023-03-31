#include "listbox.h"
#include "listitem.h"
#include "renderwindow.h"

ListBox::ListBox(QWidget* parent): QListWidget(parent)
{
    this->setFixedSize(150,720);
    this->setViewMode(QListView::IconMode);
    this->setIconSize(QSize(55, 55));
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setDragEnabled(true);
    this->setDefaultDropAction(Qt::MoveAction);
    this->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
    // 添加右键菜单
    this->setContextMenuPolicy(Qt::CustomContextMenu); 
  //////////////

}
ListBox::~ListBox()
{

}
void ListBox::dragMoveEvent(QDragMoveEvent* e) {

    if (e->mimeData()->hasFormat("application/x-item") && e->source() != this) {
        e->setDropAction(Qt::MoveAction);
        e->accept();
    } else
        e->ignore();
}


void ListBox::dropEvent(QDropEvent* event) {

    if (event->mimeData()->hasFormat("application/x-item")) {
        event->accept();
        event->setDropAction(Qt::MoveAction);
        QListWidgetItem *item = new QListWidgetItem;
        QString name = event->mimeData()->data("application/x-item");
        item->setText(name);
        item->setIcon(QIcon(":/images/iString")); //set path to image
        addItem(item);
    } else
        event->ignore();
}

void ListBox::startDrag(Qt::DropActions supportedActions) {

    ListItem* item = static_cast<ListItem*>(currentItem());
    QMimeData* mimeData = new QMimeData;
    QByteArray ba;
//    item->icon();
    ba = item->text().toLatin1().data();
    //    mimeData->setData("application/x-item", ba);
    mimeData->setImageData(*item->getImage());
    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);
    if (drag->exec(Qt::MoveAction) == Qt::MoveAction) {
        delete takeItem(row(item));
        emit itemDroped();
    }
}

void ListBox::dragEnterEvent(QDragEnterEvent* event) {

    if (event->mimeData()->hasFormat("application/x-item"))
        event->accept();
    else
        event->ignore();
}

Qt::DropAction ListBox::supportedDropActions() {

    return Qt::MoveAction;
}
