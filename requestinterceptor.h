#pragma once

#include <QWebEngineUrlRequestInterceptor> 
#include <QWebEngineUrlRequestInfo> 
#include <QFile>
#include <QSet>

class RequestInterceptor: public QWebEngineUrlRequestInterceptor
{
	Q_OBJECT
	Q_PROPERTY(bool allowFonts READ allowFonts)
public:
	RequestInterceptor(QObject *parent=0);
	virtual void interceptRequest(QWebEngineUrlRequestInfo &info) Q_DECL_OVERRIDE;
	bool allowFonts() const;
	void setAllowFonts(bool allow);
	bool setLogFile(const QString& path);
	bool loadHostBlacklist(const QString& path);
private:
	bool m_allow_fonts;
	QFile m_logfile;
	QSet<QString> m_hosts_blacklist;
};
