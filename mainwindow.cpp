#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include "apiclient.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTimer>
#include "history.h"
#include <QDebug>
#include <QMovie>
#include <QMessageBox>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("../gameportal-api-client/icon.png"));

    h = new History;
    connect(h, &History::historyClicked, this, &MainWindow::historySelected);

    loadingAnimation = new QMovie("../gameportal-api-client/loading.gif");
    ui->labelAnimace->setMovie(loadingAnimation);
    ui->labelAnimace->setVisible(false);

    loadingImg = new QMovie("../gameportal-api-client/loading.gif");
    ui->labelImage->setVisible(false);
    ui->labelImgValidated->setVisible(false);
    ui->labelLabelValidated->setVisible(false);
    ui->labelImgVIP->setVisible(false);
    ui->labelLabelVIP->setVisible(false);

    connect(&manager,SIGNAL(finished(QNetworkReply*)),SLOT(downloadFinished(QNetworkReply*)));
    connect(&imgManager,SIGNAL(finished(QNetworkReply*)),SLOT(imgFinished(QNetworkReply*)));
    ui->textEditDisplay->setVisible(false);//schova widget

    ui->labelLabelAboutShort->setVisible(false);
    ui->labelLabelBirthDate->setVisible(false);
    ui->labelLabelCoinsSpecial->setVisible(false);
    ui->labelLabelEmail->setVisible(false);
    ui->labelLabelFacebook->setVisible(false);
    ui->labelLabelFirstname->setVisible(false);
    ui->labelLabelGoogle->setVisible(false);
    ui->labelLabelLastname->setVisible(false);
    ui->labelLabelMidname->setVisible(false);
    ui->labelLabelPermission->setVisible(false);
    ui->labelLabelRegDate->setVisible(false);
    ui->labelLabelTelephone->setVisible(false);
    ui->labelLabelTwitter->setVisible(false);
    ui->labelLabelUsername->setVisible(false);
    ui->labelLabelXP->setVisible(false);

    QFile file("../gameportal-api-client/history.yeet");
    if(!file.exists())
    {
        if(file.open(QIODevice::WriteOnly))
        {
            qDebug() << "Soubor vytvořen";
            file.close();
        }
    }
    if(file.open(QIODevice::ReadOnly))
    {
        QString data = file.readAll();
        QStringList dataList = data.split("\n");
        for(int i = 0; i < dataList.length(); ++i)
        {
            if(dataList[i].startsWith("+ "))
            {
                dataList[i].remove(0, 2);
                history.append(dataList[i]);
            }
            else
            {
                //jina data
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonZobrazit_clicked()
{
    ui->labelAnimace->setVisible(true);
    loadingAnimation->start();

    history.append(ui->lineEditUsername->text());
    apiclient c;
    c.setMethod("get_user_info");
    c.addQuery("username", ui->lineEditUsername->text());
    QNetworkRequest request;
    qDebug() << c.getUrl();
    request.setUrl(c.getUrl());
    manager.get(request);
    c.clear();

    ui->labelLabelAboutShort->setVisible(false);
    ui->labelLabelBirthDate->setVisible(false);
    ui->labelLabelCoinsSpecial->setVisible(false);
    ui->labelLabelEmail->setVisible(false);
    ui->labelLabelFacebook->setVisible(false);
    ui->labelLabelFirstname->setVisible(false);
    ui->labelLabelGoogle->setVisible(false);
    ui->labelLabelLastname->setVisible(false);
    ui->labelLabelMidname->setVisible(false);
    ui->labelLabelPermission->setVisible(false);
    ui->labelLabelRegDate->setVisible(false);
    ui->labelLabelTelephone->setVisible(false);
    ui->labelLabelTwitter->setVisible(false);
    ui->labelLabelUsername->setVisible(false);
    ui->labelLabelXP->setVisible(false);

    ui->labelAboutShort->clear();
    ui->labelBirthDate->clear();
    ui->labelCoinsSpecial->clear();
    ui->labelEmail->clear();
    ui->labelFacebook->clear();
    ui->labelFirstname->clear();
    ui->labelGoogle->clear();
    ui->labelLastname->clear();
    ui->labelMidname->clear();
    ui->labelPermission->clear();
    ui->labelRegDate->clear();
    ui->labelTel->clear();
    ui->labelTwitter->clear();
    ui->labelUsername->clear();
    ui->labelXP->clear();

    ui->labelImage->setVisible(false);
    ui->labelImgValidated->setVisible(false);
    ui->labelLabelValidated->setVisible(false);
    ui->labelImgVIP->setVisible(false);
    ui->labelLabelVIP->setVisible(false);
    QPixmap vip("../gameportal-api-client/VIP.png");
    ui->labelImgVIP->setPixmap(vip.scaled(24, 24));
    QPixmap validated("../gameportal-api-client/validated.png");
    ui->labelImgValidated->setPixmap(validated.scaled(24, 24));
}

void MainWindow::downloadFinished(QNetworkReply *reply)
{
    QByteArray text = reply->readAll();

    loadingAnimation->stop();
    ui->labelAnimace->setVisible(false);

    ui->textEditDisplay->setText(text);
    if(text.startsWith("err_"))
    {
        qDebug() << "Nastala chyba: " << text;
        QMessageBox::warning(this, "Nastala chyba - " + windowTitle(), text);
    }
    else
    {
        if(reply->url().toString().contains("get_user_info"))
        {
            loadingImg->start();
            ui->labelImage->setMovie(loadingImg);
            ui->labelImage->setVisible(true);

            ui->labelLabelAboutShort->setVisible(true);
            ui->labelLabelBirthDate->setVisible(true);
            ui->labelLabelCoinsSpecial->setVisible(true);
            ui->labelLabelEmail->setVisible(true);
            ui->labelLabelFacebook->setVisible(true);
            ui->labelLabelFirstname->setVisible(true);
            ui->labelLabelGoogle->setVisible(true);
            ui->labelLabelLastname->setVisible(true);
            ui->labelLabelMidname->setVisible(true);
            ui->labelLabelPermission->setVisible(true);
            ui->labelLabelRegDate->setVisible(true);
            ui->labelLabelTelephone->setVisible(true);
            ui->labelLabelTwitter->setVisible(true);
            ui->labelLabelUsername->setVisible(true);
            ui->labelLabelXP->setVisible(true);

            qDebug() << "zpracovavam user";
            QJsonDocument doc_user = QJsonDocument::fromJson(text);
            QJsonObject doc_user_obj = doc_user.object();
            QJsonObject user = doc_user_obj["user"].toObject();
            QJsonValue permission = user["perm_lvl"].toString();
            QString perm;
            if(permission.toString() == "administrator")
                perm = "Správce";
            else if(permission.toString() == "moderator")
                perm = "Moderátor";
            else if(permission.toString() == "developer")
                perm = "Vývojář";
            else if(permission.toString() == "user")
                perm = "Uživatel";
            else
                perm = "err_perm_invalid";
            ui->labelPermission->setText(perm);
            if(user["basic"].isObject())
            {
                QJsonObject basic;
                basic = user["basic"].toObject();
                ui->labelFirstname->setText(basic["firstname"].toString());
                ui->labelMidname->setText(basic["midname"].toString());
                ui->labelLastname->setText(basic["lastname"].toString());
            }
            else
            {
                QJsonValue basic;
                basic = user["basic"].toString();
                ui->labelFirstname->setText("Neveřejné");
                ui->labelMidname->setText("Neveřejné");
                ui->labelLastname->setText("Neveřejné");
            }
            if(user["about"].isObject())
            {
                QJsonObject about;
                about = user["about"].toObject();
                ui->labelEmail->setText("<a href='mailto:" + about["email"].toString() + "'>" + about["email"].toString() + "</a>");
                ui->labelEmail->setOpenExternalLinks(true);
                ui->labelTel->setText(about["telephone"].toString());
                ui->labelAboutShort->setText(about["about_short"].toString());
                QJsonValue birthDate = about["birth_date"].toInt();
                uint birthTimestamp = birthDate.toInt();
                QDateTime birthDateTime = QDateTime::fromTime_t(birthTimestamp);
                ui->labelBirthDate->setText(birthDateTime.toString());
            }
            else
            {
                QJsonValue about;
                about = user["about"].toString();
                ui->labelEmail->setText("Neveřejné");
                ui->labelTel->setText("Neveřejné");
                ui->labelAboutShort->setText("Neveřejné");
            }
            QJsonValue regDate = user["reg_date"].toInt();
            uint timestamp = regDate.toInt();
            QDateTime regDateTime = QDateTime::fromTime_t(timestamp);
            ui->labelRegDate->setText(regDateTime.toString());
            QJsonValue validated = user["validated"].toBool();
            if(validated.toBool())
            {
                qDebug() << "--> nastavit obrazek imgValidated";
                ui->labelImgValidated->setVisible(true);
                ui->labelLabelValidated->setVisible(true);
            }
            QJsonValue imgUrl = user["img_url"].toString();
            viewImage(imgUrl.toString());
            QJsonObject social;
            social = user["social"].toObject();
            if(social["facebook_url"] != "err_forbidden")
            {
                ui->labelFacebook->setText("<a href='" + social["facebook_url"].toString() + "'>" + social["facebook_url"].toString() + "</a>");
                ui->labelFacebook->setOpenExternalLinks(true);
            }
            else
            {
                ui->labelFacebook->setText("Neveřejné");
            }
            if(social["twitter_url"] != "err_forbidden")
            {
                ui->labelTwitter->setText("<a href='" + social["twitter_url"].toString() + "'>" + social["twitter_url"].toString() + "</a>");
                ui->labelTwitter->setOpenExternalLinks(true);
            }
            else
            {
                ui->labelTwitter->setText("Neveřejné");
            }
            if(social["google_url"] != "err_forbidden")
            {
                ui->labelGoogle->setText("<a href='" + social["google_url"].toString() + "'>" + social["google_url"].toString() + "</a>");
                ui->labelGoogle->setOpenExternalLinks(true);
            }
            else
            {
                ui->labelGoogle->setText("Neveřejné");
            }
            QJsonValue xpValue = user["xp_value"].toInt();
            ui->labelXP->setText(QString::number(xpValue.toInt()));
            QJsonValue vip = user["vip"].toBool();
            if(vip.toBool())
            {
                qDebug() << "--> nastavit obrazek imgVIP";
                ui->labelImgVIP->setVisible(true);
                ui->labelLabelVIP->setVisible(true);
            }

            ui->labelUsername->setText(user["username"].toString());
        }
    }
}

void MainWindow::imgFinished(QNetworkReply *reply)
{
    loadingImg->stop();

    QByteArray imgData = reply->readAll();
    QPixmap pixmap;
    pixmap.loadFromData(imgData);
    ui->labelImage->setPixmap(pixmap);
    qDebug() << "obrazek stazen";
}

void MainWindow::on_pushButtonHistorie_clicked()
{
    History h;
    h.set(history);
    if(h.exec() == QDialog::Accepted)
    {

    }
    else
    {
        qDebug() << "dialog history closed";
        //        QTimer::singleShot(0,this,SLOT(close()));
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "close";
    saveHistory();
}

void MainWindow::viewImage(QString url)
{
    QNetworkRequest imgRequest;
    imgRequest.setUrl(url);
    imgManager.get(imgRequest);
    qDebug() << "pozadavek na obrazek";
}

void MainWindow::saveHistory()
{
    QFile file("../gameportal-api-client/history.yeet");
    if(file.open(QIODevice::WriteOnly))
    {
        QString data = "";
        for(int i = 0; i < history.length(); ++i)
        {
            data.append(QString("+ " + history[i]));
            if(i + 1 < history.length())
            {
                data.append("\n");
            }
        }
        file.write(data.toLatin1());
    }
    file.close();
}

void MainWindow::historySelected()
{
    qDebug() << "signal succesful";
    ui->lineEditUsername->setText(h->getSelection());
}
