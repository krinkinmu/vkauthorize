#include "vkauth.h"

#include <QUrl>
#include <QUrlQuery>

VKAuth::VKAuth(QWidget *parent) :
    QWebView(parent)
{
    connect(this, SIGNAL(urlChanged(QUrl)), SLOT(handleUrlChange(QUrl)));
}

QString VKAuth::accessToken() const
{
    return m_token;
}

QString VKAuth::userId() const
{
    return m_user;
}

QString VKAuth::scope() const
{
    return m_scope;
}

QString VKAuth::appId() const
{
    return m_app;
}

int VKAuth::expireIn() const
{
    return m_expire;
}

namespace
{
    QString const redirectUrl("https://oauth.vk.com/blank.html");
}

void VKAuth::authorize(const QString &app, const QString &scope)
{
    static QString const apiVersion("5.21");
    static QString const vkAuthorizeUrl("https://oauth.vk.com/authorize");

    QUrlQuery query;
    query.addQueryItem(QString("client_id"), app);
    query.addQueryItem(QString("redirect_uri"), redirectUrl);
    query.addQueryItem(QString("v"), apiVersion);
    query.addQueryItem(QString("scope"), scope);
    query.addQueryItem(QString("display"), QString("popup"));
    query.addQueryItem(QString("response_type"), QString("token"));

    QUrl url(vkAuthorizeUrl);
    url.setQuery(query);

    m_app = app;
    m_scope = scope;

    load(url);
    show();
}

void VKAuth::handleUrlChange(const QUrl &url)
{
    static QUrl const redirect(redirectUrl);
    if (url.matches(redirect, QUrl::RemoveFragment | QUrl::RemoveQuery)) {
        if (url.hasFragment()) {
            QUrlQuery const query(url.fragment());
            m_token = query.queryItemValue(QString("access_token"));
            m_user = query.queryItemValue(QString("user_id"));
            m_expire = query.queryItemValue(QString("expires_in")).toInt();

            emit success(m_token, m_user);
        } else {
            QUrlQuery const query(url.query());
            QString const err(query.queryItemValue(QString("error")));
            QString const descr(query.queryItemValue(QString("error_description")));

            m_token.clear();
            m_user.clear();
            m_expire = -1;

            emit failure(err, descr);
        }
        hide();
    }
}


