#include "widget.h"
#include "ui_widget.h"
#include "pipeitem.h"
#include "birditem.h"
#include "ground.h"
#include <QGraphicsPixmapItem>
#include <QIcon>
#include <QPixmap>
#include <QTextEdit>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle("FlappyBird");
    setWindowIcon(QIcon(":/new/prefix1/0.png"));
    scene = new Scene(this);

    scene->setSceneRect(0,0,432,644);
    //设置场景矩形大小
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->pushButton->setGeometry(120,360,195,75);
    ui->pushButton->setIcon(QPixmap(":/new/prefix1/start_button.png"));
    ui->pushButton->setIconSize(QSize(195,75));
    ui->pushButton->setStyleSheet("border: none; background: transparent;");

    ui->pushButton_2->setGeometry(120,360,195,75);
    ui->pushButton_2->setIcon(QPixmap(":/new/prefix1/restart.png"));
    ui->pushButton_2->setIconSize(QSize(30,30));
    ui->pushButton_2->setStyleSheet("border: none; background: transparent;");
    ui->pushButton_2->hide();

    ui->pushButton_3->setGeometry(400,0,30,30);
    ui->pushButton_3->setIcon(QPixmap(":/new/prefix1/pause.png"));
    ui->pushButton_3->setIconSize(QSize(30,30));
    ui->pushButton_3->setStyleSheet("border: none; background: transparent;");
    ui->pushButton_3->hide();

    QGraphicsPixmapItem* pixItem = new QGraphicsPixmapItem(QPixmap(":/new/prefix1/bg.png"));
    scene->addItem(pixItem);
    pixItem->setPos(QPointF(0,0) );
    //游戏背景载入

    //game audio loading
    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("C:/Users/86130/Documents/Qt-Flappy-Bird-main/audio/BGM.mp3"));
    QObject::connect(player, &QMediaPlayer::mediaStatusChanged, [=](QMediaPlayer::MediaStatus status){
            if (status == QMediaPlayer::EndOfMedia)
            {
                player->setPosition(0); // 将播放位置设置为0
                player->play(); // 重新开始播放
            }
        });

    player->play();
    ui->Box->setScene(scene);

    timer = new QTimer(this);

    get_ready = new QGraphicsPixmapItem(QPixmap(":/new/prefix1/get_ready.png"));

    for(int i = 0; i < 3; ++i)
    {
        switch(i)
        {
        case 0:
            count[i]=new QGraphicsPixmapItem(QPixmap(":/new/prefix1/num1.png"));
            break;
        case 1:
            count[i]=new QGraphicsPixmapItem(QPixmap(":/new/prefix1/num2.png"));
            break;
        case 2:
            count[i]=new QGraphicsPixmapItem(QPixmap(":/new/prefix1/num3.png"));
            break;
        }

    }

    connect(ui->pushButton,&QPushButton::clicked,this,[&]()
    {
        scene->birdi->groundstop();
        scene->removeItem(scene->birdi);
        scene->removeItem(scene->startImage);
        delete scene->startImage;
        delete scene->birdi;
        qDebug("%d",scene->clickcnt);
        ui->pushButton->hide();
        ui->pushButton_2->hide();
        emit begin();
    });

    connect(this,SIGNAL(begin()),this,SLOT(start_ticking()));
    connect(timer,SIGNAL(timeout()),this,SLOT(score_update()));
    connect(timer,SIGNAL(timeout()),this,SLOT(check_over()));

}
Widget::~Widget()
{
    delete ui;
}

void Widget::score_update()
{
    QFont font("Consolas",10,50);
    QString score= QString::number(scene->score);
    ui->label->setFont(font);
    this -> ui->label->setText("score: "+score);
    ui->label->show();

}
void Widget::check_over()
{
    if(scene->gameoverbool)
    {
       ui->label->hide();
       ui->pushButton_3->hide();
       ui->pushButton_2->show();
       player->stop();
    }
}

void Widget::start_ticking()
{
    scene->addItem(get_ready);
    get_ready->setPos(118,180);
    timer1 = new QTimer(this);
    timer1->start(1000);
    connect(timer1,SIGNAL(timeout()),this,SLOT(rcount()));
    connect(this,SIGNAL(end()),this,SLOT(gamebegin()));
}

void Widget::rcount()
{
    if(cnter != 0)
    {
        scene->removeItem(count[3 - cnter]);
        delete count[3 - cnter];
    }
    if(cnter<3)
    {
        scene->addItem(count[2 - cnter]);
        count[2 - cnter]->setPos(208,280);
    }
    if(cnter==3)
    {
        timer1->stop();
        emit end();
    }
    cnter++;
    return;
}

void Widget::gamebegin()
{
    scene->removeItem(get_ready);
    ui->pushButton_3->show();
    delete get_ready;
    scene->mainstart();
    connect(scene->bird,&birditem::collidesignal2,[=](){
        scene->gameover();
     });
    timer->start(100);
}

void Widget::on_pushButton_3_clicked()
{
    // stop everything if the game is going on
    if(!pause)
    {
        pause = true;
        scene->bird->birdpause();   //鸟停止运动
        scene->ground->groundani->pause();   //地板停止运动
        QList<QGraphicsItem*> sceneItems = scene->items();
            for(int i=0; i<sceneItems.size(); i++){
                PipeItem * pipe = qgraphicsitem_cast<PipeItem*>(sceneItems[i]);
                if(pipe){
                    pipe->xani->pause();
                }
            }
            scene->pipetimer->stop();
        ui->pushButton_3->setIcon(QPixmap(":/new/prefix1/continue.png"));
    }
    // continue otherwise
    else
    {
        pause = false;
        scene->pipetimer->start(2500);
        scene->bird->birdresume();
        scene->ground->groundani->resume();
        QList<QGraphicsItem*> sceneItems = scene->items();
            for(int i=0; i<sceneItems.size(); i++){
                PipeItem * pipe = qgraphicsitem_cast<PipeItem*>(sceneItems[i]);
                if(pipe){
                    pipe->xani->resume();
                }
            }
        ui->pushButton_3->setIcon(QPixmap(":/new/prefix1/pause.png"));
    }
}

void Widget::on_pushButton_2_clicked()
{
    hide();
    Widget* new_w = new Widget();
    new_w->show();
}
