#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <random>
#include <time.h>
#include <stdio.h>
#include "rapidcsv.h"
#include <stdlib.h>
#include <stdio.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    lockAnimation = true; // don't let multiple wiggle animations overlap
    mostRecentIndex = -1; // stop PRNG from generating two values in a row
    srand (time(NULL));



    // read in CSV
    QFile file(":/text/items.csv");
    file.open(QIODevice::ReadOnly);
    std::string str(file.readAll().toStdString());
    std::istringstream contents(str);
    dataCsv = rapidcsv::Document(contents);
    size = dataCsv.GetRowCount();

    file.close();

    ui->setupUi(this);

    initialTransparencyEffect = new QGraphicsOpacityEffect(this);
    initialTransparencyEffect->setOpacity(0.1);
    ui->main->setGraphicsEffect(initialTransparencyEffect);

    ui->stackedWidget->setCurrentWidget(ui->splash);


}

MainWindow::~MainWindow()
{
    delete ui;
}

QPropertyAnimation* MainWindow::FadeIn(QWidget *widget) {
    // fadein animation
    // https://stackoverflow.com/questions/19087822/how-to-make-qt-widgets-fade-in-or-fade-out/32985126

    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(this);
    widget->setGraphicsEffect(effect);
    QPropertyAnimation* a = new QPropertyAnimation(effect, "opacity");

    a->setDuration(1750);
    a->setStartValue(0);
    a->setEndValue(1);
    a->setEasingCurve(QEasingCurve::InQuart);

    a->start(QPropertyAnimation::DeleteWhenStopped);

    return a;

}
QPropertyAnimation* MainWindow::wiggle(QWidget* widget) { // get wiggle animation for a widget
    QPropertyAnimation* vibrateAnimation = new QPropertyAnimation(widget, "geometry");
    vibrateAnimation->setDuration(500); // TOTAL animation length, ms

    QRect leftGeom = widget->geometry();
    QRect rightGeom = widget->geometry();
    QRect originalGeom = leftGeom;
    leftGeom.translate(-20, 0);
    rightGeom.translate(20, 0);

    for (int i = 0; i < 5; i++) {
        vibrateAnimation->setKeyValueAt((float)i/5, leftGeom);
        vibrateAnimation->setKeyValueAt((float)i/5+0.1, rightGeom);
    }
    vibrateAnimation->setKeyValueAt(1, originalGeom);
    vibrateAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    return vibrateAnimation;


}



void MainWindow::popupAfterAnimation() {
    lockAnimation = false; // also reset lockAnimation


    int index = std::mt19937(time(NULL))() % size; // mersenne twister to get index
    if (index == mostRecentIndex) {
        index = (mostRecentIndex + 1) % size;
    }
    mostRecentIndex = index;

    QString text = QString::fromStdString( \
                dataCsv.GetCell<std::string>("text", index) \
                );
    QString html = QString::fromStdString( "<img src=\"qrc:/images/" + \
                dataCsv.GetCell<std::string>("img", index) + \
                "\" height=\"350\">" \
                );

    QMessageBox messageBox(QMessageBox::Information, "TITLE", \
                            "La bola mágica 8 dice<br>\"" + text + "\"<br>" \
                           + html);
    messageBox.exec();


}


void MainWindow::on_btn_pressed()
{
    if (!lockAnimation) {
        lockAnimation = true;

        QPropertyAnimation* imgAnimation = wiggle(ui->img);
        connect(imgAnimation, &QPropertyAnimation::finished, this, &MainWindow::popupAfterAnimation);

        wiggle(ui->tocar);

    }
}


void MainWindow::on_splashButton_clicked()
{   ui->stackedWidget->setCurrentWidget(ui->main);
    initialTransparencyEffect->~QGraphicsOpacityEffect();
    FadeIn(ui->main);
    lockAnimation = false;
    ui->splash->deleteLater();
}

