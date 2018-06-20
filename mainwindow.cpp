#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "imagecarousel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ImageCarousel *imageCarousel = new ImageCarousel(this);
    for(int i = 0; i < 10; i++){
        imageCarousel->addPage(QPixmap(QString(":/resource/%1.jpg").arg(i)));
    }
    imageCarousel->updateImageView();
    setCentralWidget(imageCarousel);
}

MainWindow::~MainWindow()
{
    delete ui;
}
