#include "tabbarwithcontrol.h"
#include "tabdelegate.h"
#include <QHBoxLayout>
#include <QScrollBar>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QMouseEvent>


TabBarWithControl::TabBarWithControl(QWidget *parent, QAbstractListModel *model)
    : QFrame(parent), _tabsModel(model) {
    setupUI();
}

void TabBarWithControl::setupUI() {
    this->setObjectName("tabBar");
    _layout = new QHBoxLayout(this);
    _layout->setSpacing(0);
    _layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(_layout);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    _tabCountLabel = new QPushButton();
    _tabCountLabel->setObjectName("tabCountLabel");
    _tabCountLabel->setCursor(Qt::PointingHandCursor);
    connect(_tabCountLabel, &QPushButton::clicked, this, [this] {
        qDebug() << "tab count label clicked";
        _tabListWidthLimit = -1;
        _updateTabsListWidth();
    });
    
    _tabsList = new QListView();
    _tabsList->setObjectName("tabsList");
    // _tabsList->setViewMode(QListView::IconMode);
    _tabsList->setFlow(QListView::LeftToRight);
    _tabsList->setWrapping(false);
    _tabsList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _tabsList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _tabsList->setContentsMargins(0, 0, 0, 0);
    _tabsList->setSpacing(0);
    _tabsList->setResizeMode(QListView::ResizeMode::Fixed);
    // _tabsList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    _tabsList->setGridSize(QSize(100, 30));
    _tabsList->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    _tabsList->setModel(_tabsModel);
    
    connect(_tabsModel, &TabsModel::dataChanged, this, [this](const QModelIndex &topLeft,
        const QModelIndex &, const QList<int> &roles) {
        if (!roles.contains(TabsModel::IsActiveRole))
            return;
        if (topLeft.data(TabsModel::IsActiveRole).toBool())
            _tabsList->setCurrentIndex(topLeft);
        _tabCountLabel->setText(QString::number(_tabsModel->rowCount()));
    });

    _tabsList->setItemAlignment(Qt::AlignmentFlag::AlignCenter);
    // _tabsList->setUniformItemSizes(true);
    _tabsList->setWordWrap(false);
    _tabsList->setMouseTracking(true);
    auto delegate = new TabDelegate();

    connect(delegate, &TabDelegate::closeClicked, this,
            &TabBarWithControl::onTabCloseClicked);
    connect(delegate, &TabDelegate::tabClicked, this,
            &TabBarWithControl::onTabClicked);

    _tabsList->setItemDelegate(delegate);

    _tabsList->setSelectionRectVisible(false);
    _tabsList->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    int initialHeight =
        _tabsList->gridSize().height() + (_tabsList->spacing() * 2);
    _tabsList->setFixedHeight(initialHeight);

    connect(_tabsList->horizontalScrollBar(), &QScrollBar::rangeChanged, this,
            [this](int min, int max) {
        int itemHeight = _tabsList->gridSize().height(); // 30
        int frameWidth =
            _tabsList->frameWidth() * 2; // Рамки виджета 
        int spacing = 0;                 

        // высота без скроллбара
        int targetHeight = itemHeight + spacing; 

        
        if (max > 0) {
            targetHeight +=
                _tabsList->horizontalScrollBar()->sizeHint().height();
        }

        // динамически меняем высоту списка
        _tabsList->setFixedHeight(targetHeight);
    });

    // connect(_tabsList, &QListView::clicked, this,
    //         &TabBarWithControl::onTabClicked);
    // _tabsList->setMaximumHeight(35);


    qDebug() << "\nTAB LIST SIZE = " << _tabsList->size();


    _controlPanel = new QFrame();
    _controlPanel->setObjectName("controlPanel");
    auto _controlLayout = new QHBoxLayout(_controlPanel);

    _addNewTabButton = new QPushButton("");
    _addNewTabButton->setObjectName("newTabButton");
    connect(_addNewTabButton, &QPushButton::clicked, this,
            [this] { emit this->newTabClicked(); });

    _minimiseButton = new QPushButton("");
    _minimiseButton->setObjectName("minimiseButton");
    connect(_minimiseButton, &QPushButton::clicked, this,
            [this] { emit this->minimiseClicked(); });
    _maximizeButton = new QPushButton("");
    _maximizeButton->setObjectName("maximizeButton");
    _maximizeButton->setProperty("maximized", false);
    connect(_maximizeButton, &QPushButton::clicked, this,
            &TabBarWithControl::_onMaximizeClicked);
    _setMaximized(false);

    _closeButton = new QPushButton("");
    _closeButton->setObjectName("closeButton");
    connect(_closeButton, &QPushButton::clicked, this,
            [this] { emit this->closeClicked(); });

    connect(_tabsModel, &QAbstractItemModel::rowsInserted, this, [this] { _updateTabsListWidth(); });
    connect(_tabsModel, &QAbstractItemModel::rowsRemoved, this, [this] { _updateTabsListWidth(); });
    connect(_tabsModel, &QAbstractItemModel::modelReset, this, [this] { _updateTabsListWidth(); });

    _layout->addWidget(_tabCountLabel, 0, Qt::AlignCenter);
    _layout->addWidget(_tabsList, 0, Qt::AlignLeft);

    _separator = new QPushButton("");
    _separator->setObjectName("separator");
    _separator->setCursor(Qt::SplitHCursor);
    connect(_separator, &QPushButton::pressed, this, [this] { 
        qDebug() << "separator pressed";
        _isSeparatorPressed = true; 
        qApp->installEventFilter(this);
        // _separator->grabMouse();
    });

    this->setMouseTracking(true);
    _layout->addWidget(_separator, 0, Qt::AlignCenter);
    
    _layout->addItem(
        new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed));
    _controlLayout->setContentsMargins(0, 0, 0, 0);
    _controlLayout->setSpacing(0);
    _controlLayout->addWidget(_addNewTabButton, 0, Qt::AlignmentFlag::AlignLeft);
    _controlLayout->addItem(
        new QSpacerItem(50, 20, QSizePolicy::Expanding, QSizePolicy::Fixed));
    _controlLayout->addWidget(_minimiseButton);
    _controlLayout->addWidget(_maximizeButton);
    _controlLayout->addWidget(_closeButton);
    _layout->addWidget(_controlPanel, 0);
    // qDebug() << _layout->count();
}


bool TabBarWithControl::eventFilter(QObject *obj, QEvent *event) {
    qDebug() << "event filter separator pressed: " << _isSeparatorPressed;
    if (obj == _separator && _isSeparatorPressed && event->type() == QEvent::MouseMove) {
        auto *me = static_cast<QMouseEvent *>(event);
        _tabListWidthLimit = qMax(mapFromGlobal(me->globalPosition().toPoint()).x(), _minTabListWidth);
        _updateTabsListWidth();
        return true;
    }
    if (event->type() == QEvent::MouseButtonRelease) {
        qDebug() << "separator released";
        _isSeparatorPressed = false;
        qApp->removeEventFilter(this);
    }
    return QFrame::eventFilter(obj, event);
}
void TabBarWithControl::_updateTabsListWidth() {
    const int tabWidth = 100; 
    const int count = _tabsModel->rowCount();
    const int contentWidth = count * tabWidth + _tabsList->frameWidth() * 2;

    const int available = width()
        - _tabCountLabel->sizeHint().width()
        - _separator->sizeHint().width()
        - _controlPanel->sizeHint().width()
        - _layout->contentsMargins().left()
        - _layout->contentsMargins().right()
        - _layout->spacing();

    int finalWidth = _tabListWidthLimit == -1 ? contentWidth : qMin(contentWidth, _tabListWidthLimit);
    int listWidth = qMin(finalWidth, qMax(_minTabListWidth, available));
    _tabsList->setFixedWidth(listWidth);

    
    // _tabsList->setHorizontalScrollBarPolicy(
    //     contentWidth > available ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);
}
void TabBarWithControl::resizeEvent(QResizeEvent *e) {
    QFrame::resizeEvent(e);
    _updateTabsListWidth();
}

void TabBarWithControl::mouseDoubleClickEvent(QMouseEvent *event) {
    qDebug() << "\nTab bar double clicked\n";
    emit barDoubleClicked();
}

void TabBarWithControl::onTabCloseClicked(const QModelIndex &index) {
    qDebug() << "selected:" << _tabsList->selectionModel()->selectedIndexes();

    emit tabCloseClicked(index.row());
}

void TabBarWithControl::onTabClicked(const QModelIndex &index) {
    qDebug() << "tab clicked";
    emit tabClicked(index.row());
}

void TabBarWithControl::_setMaximized(bool maximized) {
    _isMaximized = maximized;
    _maximizeButton->setProperty("maximised", _isMaximized);
    _maximizeButton->style()->unpolish(_maximizeButton);
    _maximizeButton->style()->polish(_maximizeButton);
}
void TabBarWithControl::_onMaximizeClicked() {
    _setMaximized(!_isMaximized);
    emit maximizedChanged(_isMaximized);
}

// void TabBarWithControl::setTabSelected(int ind) {
//     if (ind >= _tabsModel->rowCount()) {
//         qDebug() << "invalid tab index to select: " << ind
//                  << " tabs count = " << _tabsModel->rowCount() << "\n";
//         return;
//     }
//     QModelIndex index = _tabsModel->index(ind, 0);
//     qDebug() << "\nset selected tab : " << index.row()
//              << "\n rows count = " << _tabsModel->rowCount() << "\n";
//     _tabsList->setCurrentIndex(index);
//     _tabsList->selectionModel()->select(
//         index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
// }
