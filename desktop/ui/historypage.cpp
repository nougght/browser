#include "historypage.h"
#include <QLabel>
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

    _historyList = new QListView();
    _historyList->setFlow(QListView::TopToBottom);
    _historyList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _historyList->setModel(_historyModel);
    _vertLayout->addWidget(new QLabel("История"),0 , Qt::AlignHCenter);
    _vertLayout->addWidget(_historyList, 0, Qt::AlignHCenter);

}
