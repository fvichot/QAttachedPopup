#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class medAttachedPopup : public QWidget {
    Q_OBJECT
public:
    enum Orientation {TOP, BOTTOM, LEFT, RIGHT};
    Q_PROPERTY(medAttachedPopup::Orientation m_orientation READ orientation WRITE setOrientation)

    medAttachedPopup(QWidget *parent = 0);
    void attachTo(QWidget * w, medAttachedPopup::Orientation orientation = TOP);
    QWidget * centerWidget();

    medAttachedPopup::Orientation orientation() const;

public slots:
    void display();

    void setOrientation(medAttachedPopup::Orientation arg);

protected:
    virtual void mouseReleaseEvent(QMouseEvent * event);

private:
    void placeArrow();

    QWidget * m_hostWidget;
    QScopedPointer<QGridLayout> m_gridLayout;
    QScopedPointer<QWidget> m_center;
    QScopedPointer<QPixmap> m_arrow;
    QScopedPointer<QLabel> m_arrowLabel;
    Orientation m_orientation;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    medAttachedPopup * popup;
};

#endif // MAINWINDOW_H
