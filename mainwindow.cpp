#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDebug>
#include <QToolBar>

extern "C" {
#include "events.h"
extern float teaPrice;
extern float insertedMoney;
extern char* selectedTea;
}

MainWindow::MainWindow(QWidget *parent) //this is the main class where everything is initialized
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    logView = new QTextEdit(this);
    logView->setReadOnly(true);
    logView->setFontFamily("Courier New");

    logDock = new QDockWidget("Debug log", this);
    logDock->setWidget(logView);
    addDockWidget(Qt::BottomDockWidgetArea, logDock);

    QToolBar *logBar = addToolBar("Log");
    QAction *toggle = logDock->toggleViewAction();
    toggle->setText("Show / hide debug log");
    logBar->addAction(toggle);

    qInfo() << "Application started";

    ui->teaPanel->setVisible(false);
    ui->coinPanel->setVisible(false);
    ui->makingPanel->setVisible(false);
    ui->actionPanel->setVisible(false);
    ui->returnBtn->setVisible(false);

    connect(ui->teaBtn0, &QPushButton::clicked, this, [this](){ onTeaButtonClicked(0); });
    connect(ui->teaBtn1, &QPushButton::clicked, this, [this](){ onTeaButtonClicked(1); });
    connect(ui->teaBtn2, &QPushButton::clicked, this, [this](){ onTeaButtonClicked(2); });
    connect(ui->teaBtn3, &QPushButton::clicked, this, [this](){ onTeaButtonClicked(3); });
    connect(ui->coin10c, &QPushButton::clicked, this, [this](){ onCoinButtonClicked(10); });
    connect(ui->coin20c, &QPushButton::clicked, this, [this](){ onCoinButtonClicked(20); });
    connect(ui->coin50c, &QPushButton::clicked, this, [this](){ onCoinButtonClicked(50); });
    connect(ui->coin1e,  &QPushButton::clicked, this, [this](){ onCoinButtonClicked(100); });
    connect(ui->coin2e,  &QPushButton::clicked, this, [this](){ onCoinButtonClicked(200); });
    connect(ui->confirmBtn, &QPushButton::clicked, this, &MainWindow::onConfirmClicked);
    connect(ui->cancelBtn,  &QPushButton::clicked, this, &MainWindow::onCancelClicked);
    connect(ui->returnBtn,  &QPushButton::clicked, this, &MainWindow::onReturnCoins);
//main connections to the fsm worker and the queing mechanism
    fsm = new FsmWorker(this);
    connect(fsm, &FsmWorker::stateChanged, this, &MainWindow::onStateChanged,
            Qt::QueuedConnection);
    connect(fsm, &FsmWorker::displayMessage, this, [this](const QString &msg){
        ui->screenSub->setText(msg);
    }, Qt::QueuedConnection);
    fsm->start();
}

MainWindow::~MainWindow()
{
    fsm->quit();
    fsm->wait();
    delete ui;
}

void MainWindow::appendLog(const QString &line) //logging function
{
    if (logView)
        logView->append(line);
}

void MainWindow::showScreen(const QString &main, const QString &sub)
{
    ui->screenMsg->setText(main);
    ui->screenSub->setText(sub);
}

void MainWindow::showPanel(const QString &panel) //showpanel just paints whatever panel is
{
    ui->teaPanel->setVisible(panel == "tea");
    ui->coinPanel->setVisible(panel == "coin");
    ui->makingPanel->setVisible(panel == "making");
    ui->actionPanel->setVisible(panel == "coin" || panel == "idle" || panel == "done");
}

void MainWindow::updateBalance()
{
    ui->balanceLabel->setText(QString("€%1").arg(balance, 0, 'f', 2));
    float change = balance - required;
    if (change > 0.01f)
        ui->changeLabel->setText(QString("Change: €%1").arg(change, 0, 'f', 2));
    else
        ui->changeLabel->setText("");
    ui->confirmBtn->setEnabled(balance >= required && required > 0.0f);
}

void MainWindow::onStateChanged(const QString &stateName) //this is the function that draws every page of the app
{
    qDebug() << "onStateChanged called:" << stateName;
    currentState = stateName;
    ui->statusLabel->setText("State: " + stateName);

    if (stateName == "S_IDLE") {
        showScreen("Welcome!", "Press Confirm to select tea or Cancel to shutdown");
        ui->confirmBtn->setText("Select Tea");
        ui->confirmBtn->setEnabled(true);
        ui->cancelBtn->setText("Shutdown");
        ui->cancelBtn->setVisible(true);
        ui->returnBtn->setVisible(false);
        showPanel("idle");
    }
    else if (stateName == "S_SELECT_TEA") {
        showScreen("Choose your tea", "Tap a variety to continue");
        showPanel("tea");
    }
    else if (stateName == "S_ASK_FOR_MONEY") {
        required = teaPrice;
        balance = insertedMoney;
        showScreen(
            QString("Insert €%1").arg(required, 0, 'f', 2),
            QString("Selected: %1").arg(selectedTea)
            );
        ui->confirmBtn->setText("Pay");
        ui->cancelBtn->setText("Cancel");
        ui->cancelBtn->setVisible(true);
        ui->returnBtn->setVisible(true);
        updateBalance();
        showPanel("coin");
    }
    else if (stateName == "S_MAKE_TEA") {
        showScreen("Making your tea...", QString("Brewing %1").arg(selectedTea));
        ui->returnBtn->setVisible(false);
        showPanel("making");
    }
    else if (stateName == "S_DONE_TEA") {
        float change = balance - required;
        QString msg = QString("Enjoy your %1!").arg(selectedTea);
        if (change > 0.01f)
            msg += QString("\nChange returned: €%1").arg(change, 0, 'f', 2);
        showScreen("Your tea is ready!", msg);
        ui->confirmBtn->setText("Done");
        ui->confirmBtn->setEnabled(true);
        ui->cancelBtn->setVisible(false);
        ui->returnBtn->setVisible(false);
        showPanel("done");
    }
    else if (stateName == "S_SHUTDOWN") {
        showScreen("Shutting down...", "Thank you!");
        showPanel("");
    }
}

void MainWindow::onTeaButtonClicked(int index) //different teas with different idexes
{
    selectedTea = index;
    int teaIndex = index;
    qInfo() << "User selected tea index:" << teaIndex;
    QTimer::singleShot(300, this, [this, teaIndex](){
        fsm->sendEvent(teaIndex);
    });
}

void MainWindow::onCoinButtonClicked(int cents) //updating the balance after i pressed on a coin
{
    balance += cents / 100.0f;
    insertedMoney = balance;
    qInfo() << "Coin inserted:" << cents << "cents. New balance:" << balance;
    updateBalance();
}

void MainWindow::onConfirmClicked()// this is hte confirm button clicked and the states it depends on
{
    if (currentState == "S_IDLE") {
        qInfo() << "User chose to start tea selection";
        fsm->sendEvent(E_SELECT_TEA);
    } else if (currentState == "S_ASK_FOR_MONEY") {
        if (balance >= required) {
            qInfo() << "Payment accepted. Balance:" << balance << "Required:" << required;
            fsm->sendEvent(E_ENOUGH_MONEY);
        } else {
            qWarning() << "Pay pressed with too little money. Balance:" << balance
                       << "Required:" << required;
            fsm->sendEvent(E_NOT_ENOUGH_MONEY);
        }
    } else if (currentState == "S_DONE_TEA") {
        balance = 0.0f;
        insertedMoney = 0.0f;
        ui->cancelBtn->setVisible(true);
        qInfo() << "Transaction finished, returning to idle";
        fsm->sendEvent(E_RESET);
    }
}

void MainWindow::onCancelClicked() // this is the cancel button functioncality
{
    if (currentState == "S_IDLE") {
        qInfo() << "User requested shutdown";
        fsm->sendEvent(E_SHUTDOWN);
    } else if (currentState == "S_ASK_FOR_MONEY") {
        qInfo() << "Payment cancelled, clearing balance";
        balance = 0.0f;
        insertedMoney = 0.0f;
        ui->changeLabel->setText("");
        fsm->sendEvent(E_NO);
    }
}

void MainWindow::onReturnCoins() //returning coiunts makes insert coint 0
{
    if (currentState == "S_ASK_FOR_MONEY") {
        qInfo() << "Returning coins:" << balance;
        showScreen(
            QString("Returned: €%1").arg(balance, 0, 'f', 2),
            "Coins returned"
            );
        balance = 0.0f;
        insertedMoney = 0.0f;
        ui->changeLabel->setText("");
        updateBalance();
    }
}