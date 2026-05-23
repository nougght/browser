#include "ui/mainwindow.h"
#include "binder/AppBinder.h"
#include "controllers/MainWindowController.h"
#include "controllers/HistoryController.h"
#include "models/historymodel.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>
#include <QTimer>

int main(int argc, char *argv[])
{
    // QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    // QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QApplication a(argc, argv);
    // a.setStyle("Fusion");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "desktop_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    
    std::unique_ptr<IBrowserCore> core(CreateBrowserCore());
    auto coreAdapter = std::make_unique<CoreAdapter>(core.get());
    auto tabsModel = new TabsModel();
    auto historyModel = new HistoryModel();
    auto mainWindow = new MainWindow(tabsModel, historyModel);
    auto mainController = std::make_unique<MainWindowController>(coreAdapter.get());
    auto tabsController = std::make_unique<TabsController>(coreAdapter.get(), tabsModel);
    auto historyController = std::make_unique<HistoryController>(coreAdapter.get(), historyModel);

    // bind controller and view signals
    AppBinder binder(mainWindow, mainController.get(), tabsController.get());
    
    QTimer::singleShot(10, [&]{
        QFile style(":/ui/style.css");
        if (style.open(QFile::ReadOnly)){

            a.setStyleSheet(style.readAll());
        }
        else {
            qDebug() << "\nCan't open style file";
        }
    });


    coreAdapter->loadTabs();
    coreAdapter->loadHistory();
    mainWindow->setFixedSize(1000, 800);
    mainWindow->show();

    return a.exec();
}
