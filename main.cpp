#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>
#include <QMetaObject>

static MainWindow *g_mainWindow = nullptr;
static QMutex      g_logMutex;

static void messageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg) //this is the log functionality
{
    const char *level = "INFO";
    switch (type) {
    case QtDebugMsg:    level = "DEBUG";    break;
    case QtInfoMsg:     level = "INFO";     break;
    case QtWarningMsg:  level = "WARNING";  break;
    case QtCriticalMsg: level = "CRITICAL"; break;
    case QtFatalMsg:    level = "FATAL";    break;
    }
    const QString line = QString("%1 [%2] %3")
                             .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                             .arg(level)
                             .arg(msg);

    {
        QMutexLocker locker(&g_logMutex); //mutex for writing to the file
        QFile file("/home/mf/Downloads/gui-machine/logs/teamachine.log"); //change this to whatever dir you want
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << line << "\n";
        }
    }

    fprintf(stderr, "%s\n", line.toLocal8Bit().constData());

    if (g_mainWindow)
        QMetaObject::invokeMethod(g_mainWindow, "appendLog",
                                  Qt::QueuedConnection, Q_ARG(QString, line));
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler(messageHandler);
    MainWindow w;
    g_mainWindow = &w;
    w.show();
    return a.exec();
}