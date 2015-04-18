#ifndef OVERLAY_H
#define OVERLAY_H

#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QDragEnterEvent>
#include <QScreen>
#include <X11/XKBlib.h>
#include <iostream>

using namespace std;

typedef std::vector<std::string> string_vector;

class Overlay: public QWidget
{
    Q_OBJECT

public:
    Overlay(QWidget *parent = 0);
//    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

public slots:
    void timeout();

private:
    QPoint dragPosition;
    QString language;

    Display *display;
    XkbDescPtr kbdDescPtr;
};

#endif
