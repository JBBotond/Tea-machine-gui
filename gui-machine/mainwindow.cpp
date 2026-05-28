#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listWidget->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //  this is S_WAIT_FOR_COINS
    std::cout << "Button clicked! Money input list visible" << std::endl;
    ui->label->setText("Choose an action!");

    //  set back to true on reset
    ui->pushButton->setVisible(false);
    ui->listWidget->setVisible(true);
}
void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    std::cout << item->text().toStdString() << std::endl;
}

