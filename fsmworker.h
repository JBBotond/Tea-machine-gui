#pragma once
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class FsmWorker : public QThread
{
    Q_OBJECT
public:
    explicit FsmWorker(QObject *parent = nullptr);
    void sendEvent(int eventValue);//event value is held in pending event and passed in it
    int  waitForGuiInput();

signals:
    void stateChanged(const QString &stateName);
    void displayMessage(const QString &msg);

protected:
    void run() override;

private:
    QMutex mutex; //mutex for g worker
    QWaitCondition condition; //this is the condition needed for locking and unlocking it
    int pendingEvent = -1; //starting value for pending event so when no events are passed its -1
};