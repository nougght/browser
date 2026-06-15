#include "historypage.h"
#include "historydelegate.h"
#include <QLabel>
#include <QMessageBox>
HistoryPage::HistoryPage(QWidget *parent, QAbstractListModel *model)
    : QFrame(parent), _historyModel(model)
{
    setupUI();
}

void HistoryPage::setupUI(){
    this->setObjectName("historyPage");
    _vertLayout = new QVBoxLayout(this);
    this->setLayout(_vertLayout);


    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _clearButton = new QPushButton("Очистить историю");
    _clearButton->setObjectName("clearButton");
    _clearButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(_clearButton, &QPushButton::clicked, this, &HistoryPage::onClearClicked);

    
    _historyList = new QListView();
    _historyList->setObjectName("historyList");
    _historyList->setFlow(QListView::TopToBottom);
    _historyList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _historyList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _historyList->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    _historyList->setContentsMargins(0, 0, 0, 0);
    _historyList->setSpacing(0);
    _historyList->setGridSize(QSize(200, 30));
    _historyList->setModel(_historyModel);
    _historyList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto delegate = new HistoryDelegate();
    connect(delegate, &HistoryDelegate::historyClicked, this, &HistoryPage::onHistoryClicked);
    connect(delegate, &HistoryDelegate::deleteClicked, this, &HistoryPage::onDeleteClicked);
    
    _historyList->setItemDelegate(delegate);
    _vertLayout->addWidget(new QLabel("История"),0 , Qt::AlignHCenter);
    _vertLayout->addWidget(_clearButton, 0, Qt::AlignRight | Qt::AlignVCenter);
    _vertLayout->addWidget(_historyList, 0);
}

void HistoryPage::onHistoryClicked(const QModelIndex &index) {
    emit historyClicked(index.row());
}


void HistoryPage::onDeleteClicked(const QModelIndex &index) {
    emit deleteClicked(index.row());
}

void HistoryPage::onClearClicked() {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Очистить историю", "Вы уверены, что хотите очистить историю?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        qDebug() << "clear history approved";
        emit clearClicked();
    } else {
        qDebug() << "clear history denied";
    }
}