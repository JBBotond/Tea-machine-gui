#include "fsmworker.h"
#include <QMetaObject>
#include <QDebug>
extern "C" {
#include "fsm_functions/fsm.h"
#include "events.h"
#include "states.h"

extern state_t state;
extern event_t fsm_event;
extern char* stateEnumToText[];

void fsm_setup(void);
}

static FsmWorker *g_worker = nullptr; //the fsm thread that controlls the mutex for pendingevent s

extern "C" int Bridge_waitForGuiInput(void)// bridge function that is needed because C cant call things with objects
{
    return g_worker->waitForGuiInput();
}

FsmWorker::FsmWorker(QObject *parent) : QThread(parent)
{
    g_worker = this; //making it the parent for qthread
}

void FsmWorker::sendEvent(int eventValue) //sending events to the fsm
{
    QMutexLocker locker(&mutex);
    pendingEvent = eventValue;
    condition.wakeAll();
}

int FsmWorker::waitForGuiInput() //this here locks the mutex until pending event is a value between 0 and 15 for the event handle and it waits for input
{
    QMutexLocker locker(&mutex);
    pendingEvent = -1;
    while (pendingEvent == -1)
        condition.wait(&mutex);
    return pendingEvent;
}

void FsmWorker::run() //here everything is set up
{
    QThread::msleep(100); //without this it initialized too fast and it breaks
    fsm_setup();

    while (fsm_event != E_SHUTDOWN) {
        if (!FSM_NoEvents()) {
            fsm_event = FSM_GetEvent();
            state = FSM_EventHandler(state, fsm_event); //loading the states
            emit stateChanged(QString(stateEnumToText[state]));//passing whatever state we are in
            qInfo() << "FSM entered state:" << stateEnumToText[state]; //for the logs
        }
    }
}
extern "C" void Bridge_onStateChanged(const char* stateName) // bridge function that is needed because C cant call things with objects
{
    qDebug() << "Bridge_onStateChanged called:" << stateName;
    if (g_worker)
        QMetaObject::invokeMethod(g_worker, "stateChanged",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, QString(stateName)));
}
extern "C" void Bridge_showMessage(const char* msg)// bridge function that is needed because C cant call things with objects
{
    if (g_worker)
        QMetaObject::invokeMethod(g_worker, "displayMessage",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, QString(msg)));
}