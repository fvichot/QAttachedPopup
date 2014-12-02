#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QColor>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QMouseEvent>
#include <QVBoxLayout>


medAttachedPopup::medAttachedPopup(QWidget *parent)
    : QWidget(parent)
    , m_hostWidget(0)
    , m_gridLayout(new QGridLayout)
    , m_center(new QWidget(this))
    , m_arrow(new QPixmap(":arrow_bottom.png"))
    , m_arrowLabel(new QLabel(this))
    , m_orientation(BOTTOM)
{
    setWindowFlags(Qt::Popup|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);

    QGraphicsDropShadowEffect * effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(10);
    effect->setOffset(1);
    effect->setColor(Qt::black);
    this->setGraphicsEffect(effect);

    m_gridLayout->setSpacing(0);
    m_gridLayout->setContentsMargins(effect->blurRadius(),effect->blurRadius(),
                                     effect->blurRadius(),effect->blurRadius());

    m_center->setAttribute(Qt::WA_TranslucentBackground, false);
    m_center->setObjectName("centerWidget");
    m_center->setStyleSheet("#centerWidget {background-color:black; border:5px; border-radius:5px;}");
    setLayout(m_gridLayout.data());
    m_gridLayout->addWidget(m_center.data(), 1, 1, Qt::AlignCenter);

    placeArrow();
}


void medAttachedPopup::placeArrow()
{
    int x,y,a;
    switch(m_orientation) {
        case TOP:    x=1; y=2; a=0;   break;
        case BOTTOM: x=1; y=0; a=180; break;
        case LEFT:   x=2; y=1; a=270; break;
        case RIGHT:  x=0; y=1; a=90;  break;
        default:  return;
    }

    QTransform t;
    t.rotate(a);
    m_arrowLabel->setPixmap(m_arrow->transformed(t));
    m_gridLayout->addWidget(m_arrowLabel.data(), y, x, Qt::AlignCenter);
}


void medAttachedPopup::attachTo(QWidget * w, medAttachedPopup::Orientation o)
{
    m_hostWidget = w;
    m_orientation = o;
}


QWidget * medAttachedPopup::centerWidget()
{
    return m_center.data();
}


medAttachedPopup::Orientation medAttachedPopup::orientation() const
{
    return m_orientation;
}


void medAttachedPopup::display()
{
    // this goes first to update this->size() etc
    placeArrow();
    this->show();
    QPoint p = m_hostWidget->mapToGlobal(QPoint(0,0));
    QMargins m = m_gridLayout->contentsMargins();
    QRect frame = frameGeometry();

    if(m_orientation == TOP || m_orientation == BOTTOM) {
        p.rx() -= frame.width()/2 - m_hostWidget->width()/2 - 0;
        p.ry() -= (m_orientation == TOP) ? frame.height()-m.bottom() : -m_hostWidget->height()+m.top();
    } else if (m_orientation == LEFT || m_orientation == RIGHT) {
        p.ry() -= frame.height()/2 - m_hostWidget->height()/2 + 0;
        p.rx() -= (m_orientation == LEFT) ? frame.width()-m.right() : -m_hostWidget->width()+m.left();
    }

    this->move(p);
}


void medAttachedPopup::setOrientation(medAttachedPopup::Orientation arg)
{
    if (m_orientation != arg) {
        m_orientation = arg;
    }
}


void medAttachedPopup::mouseReleaseEvent(QMouseEvent *event)
{
    // dismiss popup if we click anywhere but on the center widget
    if( ! m_center->geometry().contains(event->pos())) {
        this->close();
        event->accept();
    }
}


void medAttachedPopup::resizeEvent(QResizeEvent *)
{
    if (this->isVisible())
        display();
}

#include <QCalendarWidget>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , popup(new medAttachedPopup)
{
    ui->setupUi(this);

    QWidget * cw = popup->centerWidget();
    QCalendarWidget * w = new QCalendarWidget(cw);
    cw->setLayout(new QVBoxLayout);
    cw->layout()->addWidget(w);
    popup->attachTo(ui->pushButton, medAttachedPopup::TOP);
    connect(ui->pushButton, SIGNAL(clicked()), popup, SLOT(display()));

    QTimer::singleShot(5000,this, SLOT(doShit()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::doShit()
{
    QWidget * cw = popup->centerWidget();
    QCalendarWidget * w = new QCalendarWidget(cw);
    cw->layout()->addWidget(w);
}
