#include "tabbarwithcontrol.h"
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QScrollBar>

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

    _tabsList = new QListView();
    _tabsList->setObjectName("tabsList");
    _tabsList->setFlow(QListView::LeftToRight);
    _tabsList->setWrapping(false);
    _tabsList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _tabsList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _tabsList->setContentsMargins(0, 0,0,0);
    _tabsList->setSpacing(0);
    _tabsList->setResizeMode(QListView::ResizeMode::Fixed);
    // _tabsList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    _tabsList->setGridSize(QSize(100, 30));
    _tabsList->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    _tabsList->setModel(_tabsModel);
    _tabsList->setItemAlignment(Qt::AlignmentFlag::AlignCenter);
    // _tabsList->setUniformItemSizes(true);
    _tabsList->setWordWrap(false);
    _tabsList->setItemDelegate(new ItemDelegate());

    _tabsList->setSelectionRectVisible(false);
    _tabsList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    int initialHeight = _tabsList->gridSize().height() + (_tabsList->spacing() * 2);
    _tabsList->setFixedHeight(initialHeight);

    connect(_tabsList->horizontalScrollBar(), &QScrollBar::rangeChanged, this, [this](int min, int max) {
        int itemHeight = _tabsList->gridSize().height(); // 30
        int frameWidth =_tabsList->frameWidth() * 2;     // Рамки виджета (обычно 2-4px)
        int spacing = 0;           // Отступы вокруг элемента (4px)

        // Базовая высота без скроллбара
        int targetHeight = itemHeight+ spacing; // ~36-38px

        // Если max > 0, значит контент не влезает и скроллбар появился
        if (max > 0) {
            targetHeight += _tabsList->horizontalScrollBar()->sizeHint().height();
        }

        // Динамически меняем высоту списка
        _tabsList->setFixedHeight(targetHeight);
    });

    connect(_tabsList, &QListView::clicked, this,
            &TabBarWithControl::onTabClicked);
    // _tabsList->setMaximumHeight(35);
    qDebug() << "\nTAB LIST SIZE = " << _tabsList->size();
    _layout->addWidget(_tabsList, 0, Qt::AlignTop);

    auto spacer = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);
    _layout->addItem(spacer);

    _controlPanel = new QFrame();
    _controlPanel->setObjectName("controlPanel");
    auto _controlLayout = new QHBoxLayout(_controlPanel);

    _addNewTabButton = new QPushButton("➕");
    _addNewTabButton->setObjectName("newTabButton");
    connect(_addNewTabButton, &QPushButton::clicked, this,
            [this] { emit this->newTabClicked(); });

    _minimiseButton = new QPushButton("—");
    _minimiseButton->setObjectName("minimiseButton");
    connect(_minimiseButton, &QPushButton::clicked, this,
            [this] { emit this->minimiseClicked(); });
    _closeButton = new QPushButton("✕");
    _closeButton->setObjectName("closeButton");
    connect(_closeButton, &QPushButton::clicked, this,
            [this] { emit this->closeClicked(); });

    _controlLayout->setContentsMargins(0, 0, 0, 0);
    _controlLayout->setSpacing(0);
    _controlLayout->addWidget(_addNewTabButton, 0, Qt::AlignmentFlag::AlignLeft);
    _controlLayout->addItem(
        new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Fixed));
    _controlLayout->addWidget(_minimiseButton);
    _controlLayout->addWidget(_closeButton);
    _layout->addWidget(_controlPanel);
    qDebug() << _layout->count();
}

void TabBarWithControl::mouseDoubleClickEvent(QMouseEvent *event) {
    qDebug() << "\nTab bar double clicked\n";
    emit barDoubleClicked();
}
void TabBarWithControl::onTabClicked(const QModelIndex &index) {
    emit tabClicked(index.row());
}

void TabBarWithControl::setTabSelected(int ind) {
    if (ind >= _tabsModel->rowCount()) {
        qDebug() << "invalid tab index to select: " << ind << " tabs count = " << _tabsModel->rowCount() << "\n";
        return;
    }
    QModelIndex index = _tabsModel->index(ind, 0);
    qDebug() << "\nset selected tab : " << index.row() << "\n rows count = " << _tabsModel->rowCount() << "\n";
    _tabsList->setCurrentIndex(index);
    _tabsList->selectionModel()->select(
        index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}
