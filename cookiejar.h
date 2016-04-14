#pragma once

#include <QWebEngineCookieStore>
#include <QNetworkCookieJar>
#include <QSet>
#include <QFile>

class CookieJar: public QNetworkCookieJar
{
	Q_OBJECT
public:
	CookieJar(QWebEngineCookieStore *store, QObject *parent=0);
	bool loadFile(const QString& path);

public slots:
	void addCookie(const QNetworkCookie& cookie);
	bool saveAs(const QString& path);

private:
	QWebEngineCookieStore *m_store;
	QList<QNetworkCookie> m_saved_cookies;
};
