#ifndef WEBENGINEPAGE_H
#define WEBENGINEPAGE_H

#include <QWebEngineView>

class WebEnginePage : public QWebEnginePage {
    Q_OBJECT
public:
    WebEnginePage(QWebEngineProfile *profile, QObject *parent = nullptr);
    ~WebEnginePage(){}
private:
    bool acceptNavigationRequest(const QUrl &url, NavigationType type,
                                     bool isMainFrame) override;
};

#endif // WEBENGINEPAGE_H
