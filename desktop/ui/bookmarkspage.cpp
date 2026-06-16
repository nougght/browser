#include "bookmarkspage.h"
#include "ui/bookmarkdelegate.h"
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



    _bookmarksList->setContentsMargins(0, 0, 0, 0);
    _bookmarksList->setSpacing(0);
    // _bookmarksList->setFrameShape(QFrame::NoFrame);
    _bookmarksList->setModel(_bookmarksModel);
    _bookmarksList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto delegate = new BookmarkDelegate();
    connect(delegate, &BookmarkDelegate::bookmarkClicked, this, &BookmarksPage::onBookmarkClicked);
    connect(delegate, &BookmarkDelegate::deleteClicked, this, &BookmarksPage::onDeleteClicked);
    _bookmarksList->setItemDelegate(delegate);

    _vertLayout->addWidget(new QLabel("Закладки"),0 , Qt::AlignHCenter);
    _vertLayout->addWidget(_bookmarksList, 0);
}

void BookmarksPage::onBookmarkClicked(const QModelIndex &index) {
    emit bookmarkClicked(index.row());
}

void BookmarksPage::onDeleteClicked(const QModelIndex &index) {
    emit deleteClicked(index.row());
}