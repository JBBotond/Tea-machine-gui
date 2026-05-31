#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , credit(0, 0, 100)
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
    std::string widgetText = item->text().toStdString();
    int coinValue = 0;
    // debug display in terminal, later add to logs too
    std::cout << widgetText << std::endl;

    if (widgetText == "5c")            coinValue = 5;
    else if (widgetText == "10c")      coinValue = 10;
    else if (widgetText == "20c")      coinValue = 20;
    else if (widgetText == "50c")      coinValue = 50;
    else if (widgetText == "1 Euro")   coinValue = 100;

    credit.addBalance(coinValue);

    int currentBalance = credit.getBalance();
    std::cout << "Current balance: " << currentBalance << std::endl;
}

