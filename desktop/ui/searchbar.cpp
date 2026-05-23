#include "searchbar.h"
#include <QToolButton>
#include <QStyle>

SearchBar::SearchBar() {
    _hLayout = new QHBoxLayout(this);
    _hLayout->setContentsMargins(0,0,0,0);

    _search = new QLineEdit();
    this->setObjectName("searchBar");
    _search->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _search->setFrame(false);
    connect(_search, &QLineEdit::returnPressed, this,
            [this] { emit searchFinished(_search->text()); });

    _bookmarkButton = new QToolButton();
    // _bookmarkButton->setIcon(QIcon(":assets/bookmark.svg"));
    _bookmarkButton->setObjectName("bookmarkButton");
    connect(_bookmarkButton, &QToolButton::clicked, this, [this]{
        emit bookmarkToggled();
        switchBookmark(true);
    });

    _hLayout->addWidget(_search, 0, Qt::AlignVCenter);
    _hLayout->addWidget(_bookmarkButton, 0, Qt::AlignVCenter);
    switchBookmark(false);

}


QString SearchBar::text(){
    return _search->text();
}

void SearchBar::switchBookmark(bool toggled){
    _bookmarkButton->setProperty("toggled", toggled);
    qDebug() << "\nbookmark toggled: " <<toggled << "\n";
    _bookmarkButton->style()->unpolish(_bookmarkButton);
    _bookmarkButton->style()->polish(_bookmarkButton);
}

void SearchBar::setText(QString text){
    _search->setText(text);
}


void SearchBar::setCursorPosition(int pos) {
    _search->setCursorPosition(pos);
}
