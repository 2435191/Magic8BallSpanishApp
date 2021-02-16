#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QPropertyAnimation>
#include <QMessageBox>
#include <QWidget>
#include <QDesktopServices>
#include <QThread>
#include <QUrl>
#include <QGraphicsOpacityEffect>
#include "rapidcsv.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_pressed();
    void popupAfterAnimation();
    void on_splashButton_clicked();

private:
    Ui::MainWindow *ui;
    rapidcsv::Document dataCsv;
    QGraphicsOpacityEffect* initialTransparencyEffect;
    int size;

    QPropertyAnimation* wiggle(QWidget *widget); // return wiggle animation for a widget
    QPropertyAnimation* FadeIn(QWidget *widget);

    int mostRecentIndex;
    bool lockAnimation;


};
#endif // MAINWINDOW_H
