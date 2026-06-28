#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QFrame>
#include <QLineEdit>
#include <QHBoxLayout>
#include <core/types.h>

class SearchBar : public QFrame
{
    Q_OBJECT
public:
    SearchBar(SearchEngine defaultEngine);
    QString text();
    void setText(QString text);
    void setCursorPosition(int pos);
    void setSearchEngine(SearchEngine engine);
    void switchBookmark(bool toggled);
signals:
    void searchFinished(QString query);
    void searchEngineChanged(SearchEngine engine);
    void bookmarkToggled();

private:
    QHBoxLayout *_hLayout;
    QLineEdit *_search;
    QComboBox *_engineComboBox;
    QToolButton *_bookmarkButton;

};

#endif // SEARCHBAR_H
