#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QDebug>
#include <QPixmap>
#include <QGraphicsPixmapItem>

#include "highlighter.h"
#include "maploader.h"
#include "main/prog.h"
#include "console.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QGraphicsScene* gs;
    Prog* prog;
    RobotMap* map;
    Console* console;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* e);
    void drawMap(RobotMap* map);
    void fixImage();

    ~MainWindow();

    QGraphicsPixmapItem* getItem(QString itemName);

public slots:
    void run();
    void reload();

private:
    Ui::MainWindow *ui;
    Highlighter *highlighter;

};

#endif // MAINWINDOW_H
