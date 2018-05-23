#include "apiclient.h"

apiclient::apiclient()
{

}

QString apiclient::getMethod() const
{
    return method;
}

void apiclient::setMethod(const QString &value)
{
    method = value;
}

void apiclient::addQuery(QString key, QString value)
{
    this->query.addQueryItem(key, value);
}

QUrl apiclient::getUrl()
{
    QUrl url("https://ue.zubro.net/rest/");
    query.addQueryItem("method", getMethod());
    query.addQueryItem("format", "json");
    url.setQuery(query);
    return url;
}

void apiclient::clear()
{
    query.clear();
}

