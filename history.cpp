#include "history.h"
#include "ui_history.h"
#include <QDebug>

History::History(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::History)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("../gameportal-api-client/icon.png"));
}

History::~History()
{
    delete ui;
}

void History::set(QStringList list)
{
    qDebug() << list;
    for(int i = 0; i < list.length(); ++i)
    {
        ui->comboBox->addItem(QIcon(), QString(list[i]), QVariant());
    }
}

void History::on_comboBox_activated(const QString &arg1)
{
    qDebug() << "comboBox clicked";
    selection = arg1;
    emit historyClicked();
}

QString History::getSelection() const
{
    return selection;
}
