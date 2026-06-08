#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QTextEdit>
#include <QDockWidget>
#include "fsmworker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    void appendLog(const QString &line);

private slots:
    void onStateChanged(const QString &stateName);
    void onTeaButtonClicked(int index);
    void onCoinButtonClicked(int cents);
    void onConfirmClicked();
    void onCancelClicked();
    void onReturnCoins();

private:
    void showScreen(const QString &main, const QString &sub = "");
    void showPanel(const QString &panel);
    void updateBalance();

    Ui::MainWindow *ui;
    FsmWorker *fsm;

    float balance = 0.0f;
    float required = 0.0f;
    int selectedTea = 0;
    QString currentState;

    QTextEdit   *logView = nullptr;
    QDockWidget *logDock = nullptr;
};

#endif