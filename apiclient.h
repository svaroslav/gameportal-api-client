#ifndef APICLIENT_H
#define APICLIENT_H

#include <QString>
#include <QUrlQuery>

class apiclient
{
public:
    apiclient();

    QString getMethod() const;
    void setMethod(const QString &value);
    void addQuery(QString key, QString value);
    QUrl getUrl();
    void clear();

private:
    QString method;
    QUrlQuery query;
};

#endif // APICLIENT_H
