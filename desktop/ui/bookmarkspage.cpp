#include "bookmarkspage.h"
#include <QLabel>

BookmarksPage::BookmarksPage(QWidget *parent,
                             QAbstractListModel *bookmarksModel)
    : QFrame(parent), _bookmarksModel(bookmarksModel) {
    _setupUI();
}

void BookmarksPage::_setupUI(){
    _vertLayout = new QVBoxLayout(this);

    _bookmarksList = new QListView();
    _bookmarksList->setObjectName("bookmarksList");
    _bookmarksList->setFlow(QListView::TopToBottom);
    _bookmarksList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _bookmarksList->setModel(_bookmarksModel);

    _vertLayout->addWidget(new QLabel("Закладки"),0 , Qt::AlignHCenter);
    _vertLayout->addWidget(_bookmarksList, 0, Qt::AlignHCenter);

}
