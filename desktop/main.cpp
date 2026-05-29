#include "binder/AppBinder.h"
#include "controllers/HistoryController.h"
#include "controllers/MainWindowController.h"
#include "models/UIContext.h"
#include "models/bookmarkmodel.h"
#include "models/historymodel.h"
#include "ui/mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QLocale>
#include <QTimer>
#include <QTranslator>

int main(int argc, char *argv[]) {
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

    auto context = new UIContext(coreAdapter.get());

    auto mainWindow = new MainWindow(context->getTabsModel(),
                                     context->getHistoryModel(),
                                     context->getBookmarkModel());

    auto mainController =
        std::make_unique<MainWindowController>(coreAdapter.get());
    auto tabsController =
        std::make_unique<TabsController>(coreAdapter.get(), context);
    auto historyController =
        std::make_unique<HistoryController>(coreAdapter.get(), context);
    auto bookmarkController =
        std::make_unique<BookmarkController>(coreAdapter.get(), context);
    // bind controller and view signals
    AppBinder binder(mainWindow, mainController.get(), tabsController.get(),
                     bookmarkController.get());

    QTimer::singleShot(10, [&] {
        QFile style(":/ui/style.css");
        if (style.open(QFile::ReadOnly)) {

            a.setStyleSheet(style.readAll());
        } else {
            qDebug() << "\nCan't open style file";
        }
    });

    coreAdapter->loadTabs();
    coreAdapter->loadHistory();
    coreAdapter->loadBookmarks();
    mainWindow->setFixedSize(1000, 800);
    mainWindow->show();

    return a.exec();
}
