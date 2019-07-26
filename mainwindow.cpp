#include "mainwindow.h"
#include "ui_mainwindow.h"

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    console = new Console(ui->consoleTextEdit);
    gs = new QGraphicsScene(this);
    highlighter = new Highlighter(ui->textEdit->document());

    map = MapLoader::load(":/map/map1.rbm");
    drawMap(map);

    ui->graphicsView->setScene(gs);

    prog = new Prog(this, map, console);

    connect(ui->actionRun, SIGNAL(triggered()), this, SLOT(run()));
    connect(ui->actionReload, SIGNAL(triggered()), this, SLOT(reload()));

    fixImage();
}

void MainWindow::drawMap(RobotMap* map) {

    QGraphicsPixmapItem* item;
    gs->clear();

    // 1 for tiles
    for (size_t i = 0; i < map->h; i++) {
        for (size_t j = 0; j < map->w; j++) {
            item = getItem(":/img/t.jpg");
            item->setPos(j*TILE_WIDTH, i*TILE_HEIGHT);
            gs->addItem(item);
        }
    }

    // robot
    if(map->isRobotAlive()) { item = getItem("://img/r.png"); }
    else { item = getItem("://img/boom_r.png"); }
    item->setPos(map->robot->j*TILE_WIDTH, map->robot->i*TILE_HEIGHT);
    gs->addItem(item);

    // 2 for walls
    for (size_t i = 0; i < map->h; i++) {
        for (size_t j = 0; j < map->w; j++) {
            int val = map->get(i, j);
            bool top    = (val&TILE_TOPWALL)    > 0;
            bool right  = (val&TILE_RIGHTWALL)  > 0;
            bool bottom = (val&TILE_BOTTOMWALL) > 0;
            bool left   = (val&TILE_LEFTWALL)   > 0;

            if(top) {
                item = getItem(":/img/wh.png");
                item->setPos(j*TILE_WIDTH, i*TILE_HEIGHT-TILE_HEIGHT*0.5);
                gs->addItem(item);
            }

            if(bottom) {
                item = getItem(":/img/wh.png");
                item->setPos(j*TILE_WIDTH, i*TILE_HEIGHT+TILE_HEIGHT*0.5);
                gs->addItem(item);
            }

            if(left) {
                item = getItem(":/img/wv.png");
                item->setPos(j*TILE_WIDTH-TILE_WIDTH*0.5, i*TILE_HEIGHT);
                gs->addItem(item);
            }

            if(right) {
                item = getItem(":/img/wv.png");
                item->setPos(j*TILE_WIDTH+TILE_WIDTH*0.5, i*TILE_HEIGHT);
                gs->addItem(item);
            }
        }
    }

}

QGraphicsPixmapItem* MainWindow::getItem(QString itemName) {
    return new QGraphicsPixmapItem(QPixmap::fromImage(QImage(itemName)));
}

void MainWindow::paintEvent(QPaintEvent* e) {
    drawMap(map);
}

void MainWindow::run() {
    QString progText = ui->textEdit->toPlainText();
    prog->interpret(progText);
}

void MainWindow::reload() {
    map = MapLoader::load(":/map/map1.rbm");
    console->clear();
    prog = new Prog(this, map, console);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fixImage() {
    /*QPixmap pixmap;
    pixmap.load("F:/Projects/Volsu/Comp/ROBOT/img/boom.png");

    QFile file("F:/Projects/Volsu/Comp/ROBOT/img/boom_r.png");
    file.open(QIODevice::WriteOnly);
    pixmap.save(&file, "PNG");*/
}

