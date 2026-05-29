#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QFrame>
#include <QLineEdit>
#include <QHBoxLayout>

class SearchBar : public QFrame
{
    Q_OBJECT
public:
    SearchBar();
    QString text();
    void setText(QString text);
    void setCursorPosition(int pos);
    void switchBookmark(bool toggled);
signals:
    void searchFinished(QString query);
    void bookmarkToggled();

private:
    QHBoxLayout *_hLayout;
    QLineEdit *_search;
    QToolButton *_bookmarkButton;

};

#endif // SEARCHBAR_H
