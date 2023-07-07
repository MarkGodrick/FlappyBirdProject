#ifndef WIDGET_H
#define WIDGET_H

#include <QDebug>
#include <QWidget>
#include "scene.h"
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    friend class Scene;
    void showblock();
    QTimer *timer;
    QTimer *timer1;
    QGraphicsPixmapItem* get_ready;
    QGraphicsPixmapItem* count[3];
    int cnter = 0;
    QMediaPlayer* player;
    bool pause;
public slots:
    void score_update();
    void check_over();
    void start_ticking();
    void gamebegin();
    void rcount();
private:
    Ui::Widget *ui;
    Scene* scene;
signals:
    void begin();
    void end();
private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
};
#endif // WIDGET_H
