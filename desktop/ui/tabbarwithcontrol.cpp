#include "tabbarwithcontrol.h"
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QSizePolicy>
#include "models/tabsmodel.h"


TabBarWithControl::TabBarWithControl(QWidget *parent, QAbstractListModel *model) : QFrame(parent), _tabsModel(model)
{
    setupUI();
}

void TabBarWithControl::setupUI()
{
    this->setObjectName("tabBar");
    _layout = new QHBoxLayout(this);
    _layout->setSpacing(0);
    _layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(_layout);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    _tabsList = new QListView();
    _tabsList->setFlow(QListView::LeftToRight);
    _tabsList->setWrapping(false);
    _tabsList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _tabsList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(_tabsList, &QListView::clicked, this, &TabBarWithControl::onTabClicked);

    // for (int i = 0; i < 6; ++i)
    // {
    //     model->addTab("вкладка " + QString::number(i));
    // }
    _tabsList->setModel(_tabsModel);
    _tabsList->setItemAlignment(Qt::AlignmentFlag::AlignCenter);
    _tabsList->setFlow(QListView::Flow::LeftToRight);
    _tabsList->setWrapping(false);
    // _tabsList->setUniformItemSizes(true);
    _tabsList->setWordWrap(false);
    _tabsList->setItemDelegate(new ItemDelegate());


    _tabsList->setSelectionRectVisible(true);
    // политика высоты - ignored, чтобы список не растягивал layout
    _tabsList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);


    // _tabsList->setMaximumHeight(35);
    qDebug() << "\n TAB LIST SIZE = " << _tabsList->size();
    _layout->addWidget(_tabsList, Qt::AlignmentFlag::AlignLeft);

    auto spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _layout->addItem(spacer);

    _controlPanel = new QFrame();
    _controlPanel->setObjectName("controlPanel");
    auto _controlLayout = new QHBoxLayout(_controlPanel);

    _addNewTabButton = new QPushButton("➕");
    _addNewTabButton->setObjectName("newTabButton");
    connect(_addNewTabButton, &QPushButton::clicked, this, [this]{emit this->newTabClicked();});

    _minimiseButton = new QPushButton("—");
    _minimiseButton->setObjectName("minimiseButton");
    connect(_minimiseButton, &QPushButton::clicked, this, [this]{emit this->minimiseClicked();});
    _closeButton = new QPushButton("✕");
    _closeButton->setObjectName("closeButton");
    connect(_closeButton, &QPushButton::clicked, this, [this]{emit this->closeClicked();});


    _controlLayout->setContentsMargins(0, 0, 0, 0);
    _controlLayout->setSpacing(0);
    _controlLayout->addWidget(_addNewTabButton, Qt::AlignmentFlag::AlignVCenter);
    _controlLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Fixed));
    _controlLayout->addWidget(_minimiseButton);
    _controlLayout->addWidget(_closeButton);
    _layout->addWidget(_controlPanel);
    qDebug() << _layout->count();
}

void TabBarWithControl::onTabClicked(const QModelIndex &index)
{
    // TabId id = _tabsModel->getTabIdByIndex(index.row());
    emit tabClicked(index.row());
}

// void TabBarWithControl::addTab(const TabInfo &tab)
// {
//     _tabsModel->addTab(tab);
// }

// void TabBarWithControl::updateTabTitle(TabId id, std::string title)
// {
//     _tabsModel->updateTabTitle(id, title);
// }

// void TabBarWithControl::updateTabUrl(TabId id, Url url)
// {
//     _tabsModel->updateTabUrl(id, url);
// }
// void TabBarWithControl::updateTabLoading(TabId id, bool isLoading)
// {
//     _tabsModel->updateTabLoading(id, isLoading);
// }
// void TabBarWithControl::updateTabNavigation(TabId id, bool canGoBack, bool canGoForward)
// {
//     _tabsModel->updateTabNavigation(id, canGoBack, canGoForward);
// }

// void TabBarWithControl::setInitialTabs(std::vector<TabInfo> tabs)
// {
//     _tabsModel->setInitialTabs(tabs);
// }
