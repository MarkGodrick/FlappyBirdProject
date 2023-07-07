#include "scene.h"
#include <QKeyEvent>
#include <birditem.h>
#include "ground.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent),startsign(0),gameoverbool(0),score(0)
{
    setpipetimer();
    //水管定时器 用于定时间间隔生成新水管
    startImage = new QGraphicsPixmapItem(QPixmap(":/new/prefix1/title.png"));
    addItem(startImage);
    startImage->setPos(127,180);
    startImage->setZValue(10);//放在最顶层

    gameoverImage = new QGraphicsPixmapItem(QPixmap(":/new/prefix1/gameover.png"));

    birdi = new groundItem(QString(":/new/prefix1/0.png"));
    addItem(birdi);
    birdi->setZValue(10000);


    ground = new groundItem;
    addItem(ground);
    ground->setZValue(10); // 设置Z值为10，放在最上面
}

void Scene::birddef()
{
    bird = new birditem(QPixmap(":/new/prefix1/0.png"));
    addItem(bird);
    bird->setZValue(20);
}

void Scene::mainstart()
{
    startsign=1;
    clickcnt++;
    birddef();
    bird->birdstart();
    if(!pipetimer->isActive())
    {
        pipetimer->start(2500);
    }
    //若不设置则在出现水管前鸟坠地不死
    connect(bird,&birditem::collidesignal2,[=](){
        gameover();
     });
}

void Scene::Scoreadd()
{
    score++;
}

void Scene::setpipetimer()
{
 pipetimer = new QTimer(this);
 connect(pipetimer,&QTimer::timeout,[=](){
     PipeItem* pipe = new PipeItem;

     //如果小鸟与水管碰撞，游戏结束
     connect(pipe,&PipeItem::collidesignal,[=](){
         pipetimer->stop();
         gameover();
     });
     //如果小鸟与地板碰撞，游戏结束
     connect(bird,&birditem::collidesignal2,[=](){
         gameover();
     });
     addItem(pipe);
 });
}

void Scene::gameover()
{
    gameoverbool=1; //已结束游戏
    bird->birdstop();   //鸟停止运动
    ground->groundstop();   //地板停止运动

    showscore();

    addItem(gameoverImage);
    gameoverImage->setPos(0,0);
    gameoverImage->setZValue(100);


    //将画面内所有水管都停止运动
    QList<QGraphicsItem*> sceneItems = items();
        for(int i=0; i<sceneItems.size(); i++){
            PipeItem * pipe = qgraphicsitem_cast<PipeItem*>(sceneItems[i]);
            if(pipe){
                pipe->pipestop();
            }
        }
        pipetimer->stop();
        //停止水管计时器 不再生成新水管
}

void Scene::showscore()
{
    scoretext = new QGraphicsTextItem();
    QString lastscore="Your Score:" + QString::number(score);
    scoretext->setHtml(lastscore);
    QMediaPlayer* ptr = new QMediaPlayer;
    ptr->setMedia(QUrl::fromLocalFile("C:/Users/86130/Documents/Qt-Flappy-Bird-main/audio/gameover.mp3"));
    ptr->play();
    //设置分数显示界面字体和颜色
    QFont font("Consolas",15,QFont::Bold);
    scoretext->setFont(font);
    QColor color(0,0,0);
    scoretext->setDefaultTextColor(color);

    //设置位置
    addItem(scoretext);
    scoretext->setPos(140 ,250);
}

//void Scene::keyPressEvent(QKeyEvent *event)
//{
//    if(startsign==0)
//        mainstart();
//    //第一次空格将移除开始界面图片，并让小鸟，水管开始运动
//    if(!gameoverbool)
//    {
//if(event->key()==Qt::Key_Space){
//    bird->jump();
//}
// //若游戏已结束 则空格不再有任何作用
//    }
//QGraphicsScene::keyPressEvent(event);
//}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    //第一次点击移除开始界面图片，让小鸟，水管开始运动
    qDebug("clicked!");
    if(startsign)
    {
        QMediaPlayer* ptr = new QMediaPlayer;
        ptr->setMedia(QUrl::fromLocalFile("C:/Users/86130/Documents/Qt-Flappy-Bird-main/audio/Flip.mp3"));
        ptr->play();
        if(!gameoverbool)
        {
            bird->jump();
        }
        QGraphicsScene::mousePressEvent(event);
    }
}
