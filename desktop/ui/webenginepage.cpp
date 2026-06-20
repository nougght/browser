#include "webenginepage.h"

WebEnginePage::WebEnginePage(QWebEngineProfile *profile, QObject *parent)
    : QWebEnginePage(profile, parent) {}

bool WebEnginePage::acceptNavigationRequest(const QUrl &url,
                                            NavigationType type,
                                            bool isMainFrame) {
    qDebug() << "WEPAGE navigation request: " << url << " " << type;
    return true;
}
