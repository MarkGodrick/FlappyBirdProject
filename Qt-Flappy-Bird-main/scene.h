#ifndef SCENE_H
#define SCENE_H

#include <QDebug>
#include <QTimer>
#include <QGraphicsScene>
#include <birditem.h>
#include "pipeitem.h"
#include "ground.h"
#include <QGraphicsPixmapItem>
#include <QMediaPlayer>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);
    void birddef();
    void mainstart();
    void Scoreadd();
    bool gameoverbool;
    QGraphicsPixmapItem* startImage;
    birditem* bird;
    groundItem* birdi;
    int clickcnt = 0;
    void gameover();
    int score;
    groundItem* ground;
    QTimer* pipetimer;
signals:

private:
    void setpipetimer();
    QGraphicsPixmapItem* gameoverImage;
    bool startsign;
    QGraphicsTextItem* scoretext;
    void showscore();

public slots:

    // QGraphicsScene interface
protected:
//    void keyPressEvent(QKeyEvent *event);
      void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCENE_H
