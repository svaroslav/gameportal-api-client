#ifndef HISTORY_H
#define HISTORY_H

#include <QDialog>
#include <QObject>

namespace Ui {
class History;
}

class History : public QDialog
{
    Q_OBJECT
signals:
    void historyClicked();

public:
    explicit History(QWidget *parent = 0);
    ~History();
    void set(QStringList list);
    QString getSelection() const;

private slots:
    void on_comboBox_activated(const QString &arg1);

private:
    Ui::History *ui;
    QString selection;
};

#endif // HISTORY_H
