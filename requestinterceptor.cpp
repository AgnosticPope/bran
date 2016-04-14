#include "requestinterceptor.h"
#include <QDebug>


RequestInterceptor::RequestInterceptor(QObject *parent)
:QWebEngineUrlRequestInterceptor(parent), m_allow_fonts(false)
{
}

bool RequestInterceptor::allowFonts() const
{
	return m_allow_fonts;
}
void RequestInterceptor::setAllowFonts(bool allow)
{
	m_allow_fonts = allow;
}
bool RequestInterceptor::setLogFile(const QString& path)
{
	if (m_logfile.isOpen()) {
		return false;
	}
	m_logfile.setFileName(path);
	return m_logfile.open(QIODevice::WriteOnly | QIODevice::Text);
}

bool RequestInterceptor::loadHostBlacklist(const QString& path)
{
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return false;
	}

	while (!file.atEnd()) {
		QString host = file.readLine().trimmed();
		if (host.isEmpty() || host.startsWith('#')) {
			continue;
		}
		m_hosts_blacklist.insert(host);
	}
	return true;
}

void RequestInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
{
	bool blocked = false;
	auto rType = info.resourceType();
	if (rType == QWebEngineUrlRequestInfo::ResourceTypeFavicon
		|| rType == QWebEngineUrlRequestInfo::ResourceTypePing) {
		// Pings and Favicons are dropped
		blocked = true;
	} else if (!m_allow_fonts &&
		rType == QWebEngineUrlRequestInfo::ResourceTypeFontResource) {
		// WebFonts are optional (blocked by default)
		blocked = true;
	} else if (rType == QWebEngineUrlRequestInfo::ResourceTypeMainFrame) {
		// Don't block the main frame
	} else {
		if (m_hosts_blacklist.contains(info.requestUrl().host())) {
			qDebug() << "Blocking" << info.requestUrl();
			blocked = true;
		}
	}

	info.block(blocked);
	if (m_logfile.isOpen()) {
		QTextStream ts(&m_logfile);
		ts << info.requestMethod()
			<< " " << info.requestUrl().toString() << "\n";
	}
}
