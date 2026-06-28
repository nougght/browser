#include "searchbar.h"
#include <QToolButton>
#include <QStyle>
#include <QComboBox>

QIcon getEngineIcon(SearchEngine engine) {
    switch (engine) {
        case SearchEngine::Google:
            return QIcon(":/assets/google-icon.svg");
        case SearchEngine::Yandex:
            return QIcon(":/assets/yandex-icon.svg");
        case SearchEngine::DuckDuckGo:
            return QIcon(":/assets/duckduckgo-icon.svg");
        default:
            return QIcon(":/assets/circle-filled.svg");
    }
}

void SearchBar::setSearchEngine(SearchEngine engine) {
    _engineComboBox->setCurrentIndex(static_cast<int>(engine));
    _engineComboBox->setToolTip(QString::fromStdString(searchEngineToString(engine)));
}

SearchBar::SearchBar(SearchEngine defaultEngine) {
    _hLayout = new QHBoxLayout(this);
    _hLayout->setContentsMargins(0,0,0,0);

    _search = new QLineEdit();
    this->setObjectName("searchBar");
    _search->setPlaceholderText("Введите запрос или адрес сайта");
    _search->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _search->setFrame(false);
    connect(_search, &QLineEdit::returnPressed, this,
            [this] { emit searchFinished(_search->text()); });

    _engineComboBox = new QComboBox();
    _engineComboBox->setObjectName("engineComboBox");
    for (int i = 0; i < static_cast<int>(SearchEngine::Count); ++i) {
        _engineComboBox->addItem(getEngineIcon(static_cast<SearchEngine>(i)),
        "", i);
    }

    _engineComboBox->setCurrentIndex(static_cast<int>(defaultEngine));
    _engineComboBox->setToolTip(QString::fromStdString(searchEngineToString(defaultEngine)));

    connect(_engineComboBox, &QComboBox::currentIndexChanged, this, [this](int index) {
        _engineComboBox->setToolTip(QString::fromStdString(searchEngineToString(static_cast<SearchEngine>(index))));
        emit searchEngineChanged(static_cast<SearchEngine>(_engineComboBox->currentData().toInt()));
    });

    _bookmarkButton = new QToolButton();
    // _bookmarkButton->setIcon(QIcon(":assets/bookmark.svg"));
    _bookmarkButton->setObjectName("bookmarkButton");
    connect(_bookmarkButton, &QToolButton::clicked, this, [this]{
        emit bookmarkToggled();
    });

    _hLayout->addWidget(_engineComboBox, 0, Qt::AlignVCenter);
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
