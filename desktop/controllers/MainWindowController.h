#ifndef MAIN_WINDOW_CONTROLLER_H
#define MAIN_WINDOW_CONTROLLER_H

#include <QObject>
#include <core/Identifier.h>
#include "adapter/CoreAdapter.h"
#include "ui/mainwindow.h"



class MainWindowController : public QObject
{
    Q_OBJECT

public:
    MainWindowController(CoreAdapter *coreAdapter);

signals:

private:
    void _setupEvents();

    // MainWindow *_mainWindow;
    CoreAdapter *_coreAdapter;

    TabsModel *_tabsModel;
    TabId _activeTabId;
};

#endif
