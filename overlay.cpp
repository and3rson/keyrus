#include <QtWidgets>

#include "overlay.h"

Overlay::Overlay(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->start(100);

    QAction *quitAction = new QAction(tr("E&xit"), this);
    quitAction->setShortcut(tr("Ctrl+Q"));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    addAction(quitAction);

    setContextMenuPolicy(Qt::ActionsContextMenu);
    setToolTip(tr("Use the right mouse button to open a context menu."));
    setWindowTitle(tr("KeyRus"));

    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    this->setGeometry(QApplication::desktop()->geometry());

    this->hide();
    this->language = "us";
}

void Overlay::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    QColor *color = 0;

    if (this->language == "ua") {
        color = new QColor(255, 255, 64);
    } else if (this->language == "ru") {
        color = new QColor(0, 255, 255);
    } else {
        color = NULL;
    }

    if (color) {
        painter.setPen(*color);
        painter.setBrush(*color);
        painter.drawRect(QRect(0, 0, width(), height()));
    }
}

void Overlay::resizeEvent(QResizeEvent * /* event */)
{
    QRegion mask;
    for(int i = 0; i < QApplication::desktop()->screenCount(); i++) {
        QWidget *screen = QApplication::desktop()->screen(i);
        QRect geom = screen->geometry();
        mask = mask + QRegion(geom.left(), geom.top(), geom.width(), 10);
        mask = mask + QRegion(geom.left(), geom.height() - 10, geom.width(), 10);
        mask = mask + QRegion(geom.left(), geom.top(), 10, geom.height());
        mask = mask + QRegion(geom.width() - 10, geom.top(), 10, geom.height());
    }
    int side = qMin(width(), height());
    setMask(mask);
}

void Overlay::timeout()
{
    //
    Display *display = XOpenDisplay(":0");

    //
    XkbDescPtr kbdDescPtr = XkbAllocKeyboard();
    if (kbdDescPtr == NULL) {
        qFatal("Failed to get keyboard description.");
    }

    kbdDescPtr->dpy = display;

    //
    XkbStateRec state;
    XkbGetState(display, XkbUseCoreKbd, &state);
    quint8 group = static_cast<int>(state.group);

    //
    XkbGetControls(display, XkbAllControlsMask, kbdDescPtr);
    XkbGetNames(display, XkbSymbolsNameMask, kbdDescPtr);

    Atom symNameAtom = kbdDescPtr->names->symbols;

    char* kbsC = XGetAtomName(display, symNameAtom);
    std::string kbs(kbsC);
    XFree(kbsC);

    QString symString(kbs.c_str());

    QStringList dirtySyms = symString.split("+");
    QStringList cleanSyms;

    foreach(QString sym, dirtySyms) {
        if (sym == "pc") {
            continue;
        }
        sym = sym.split(":").first();
        if (sym.length() == 2) {
            cleanSyms.append(sym);
        }
    }

    this->language = cleanSyms.at(group);

    this->setVisible(this->language != "us");

    this->update();
}
