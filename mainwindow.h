#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "history.h"
#include <QObject>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void downloadFinished(QNetworkReply *reply);
    void imgFinished(QNetworkReply *reply);
    void on_pushButtonZobrazit_clicked();
    void on_pushButtonHistorie_clicked();
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager manager;
    QNetworkAccessManager imgManager;
    void viewImage(QString url);
    void saveHistory();
    void historySelected();
    QStringList history;
    QMovie *loadingAnimation;
    QMovie *loadingImg;
    History *h;
};

#endif // MAINWINDOW_H
