#ifndef VKAUTH_H
#define VKAUTH_H

#include <QWebView>
#include <QString>

class VKAuth : public QWebView
{
    Q_OBJECT
public:
    explicit VKAuth(QWidget *parent = 0);

    QString accessToken() const;
    QString userId() const;
    QString scope() const;
    QString appId() const;
    int expireIn() const;

signals:
    void success(QString const &token, QString const & user);
    void failure(QString const &err, QString const &descr);

public slots:
    void authorize(QString const &app, QString const &scope);

private slots:
    void handleUrlChange(QUrl const &url);

private:
    QString m_token;
    QString m_user;
    QString m_scope;
    QString m_app;
    int m_expire;
};

#endif // VKAUTH_H
