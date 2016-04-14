#include "cookiejar.h"
#include <QDateTime>
#include <QDebug>

CookieJar::CookieJar(QWebEngineCookieStore *store, QObject *parent)
:QNetworkCookieJar(parent), m_store(store)
{
	connect(m_store, &QWebEngineCookieStore::cookieAdded,
			this, &CookieJar::addCookie);
}

bool CookieJar::saveAs(const QString& path)
{
	QFile f(path);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return false;
	}

	foreach(QNetworkCookie cookie, allCookies()) {
		QTextStream ts(&f);
		if (cookie.isHttpOnly()) {
			ts << "#HttpOnly_";
		}
		ts << cookie.domain() << "\t";

		if (cookie.domain().startsWith('.')) {
			ts << "TRUE" << "\t";
		} else {
			ts << "FALSE" << "\t";
		}

		ts << cookie.path() << "\t";

		if (cookie.isSecure()) {
			ts << "TRUE" << "\t";
		} else {
			ts << "FALSE" << "\t";
		}

		if (!cookie.isSessionCookie()) {
			ts << cookie.expirationDate().toTime_t();
		}
		ts << ("\t");

		ts << cookie.name() << "\t" << cookie.value();
		ts << "\n";
	}
	return true;
}

void CookieJar::addCookie(const QNetworkCookie& cookie)
{
	// TODO: block cookies
	//m_storage->deleteCookie(cookie);
	insertCookie(cookie);
}

/// Loads cookies into the Jar
///
/// The file format is the same as used by Netscape/Curl/Wget/Firefox, each
/// line holds one cookie with multiple fields separated by a tab(\t)
///
/// .netscape.com     TRUE   /  FALSE  946684799   NETSCAPE_ID  100103
///
/// domain - The domain that created AND that can read the variable.
/// flag - A TRUE/FALSE value indicating if all machines within a given domain
///     can access the variable. This value is set automatically by the browser,
///     depending on the value you set for domain.
/// path - The path within the domain that the variable is valid for.
/// secure - A TRUE/FALSE value indicating if a secure connection with the domain is
///     needed to access the variable.
/// expiration - The UNIX time that the variable will expire on. UNIX time is defined
///     as the number of seconds since Jan 1, 1970 00:00:00 GMT.
/// name - The name of the variable.
/// value - The value of the variable. 
///
/// Lines starting with a "#" are comments, some implementations prepend the domain
/// with the #HttpOnly attribute
bool CookieJar::loadFile(const QString& path)
{
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return false;
	}

	while (!file.atEnd()) {
		QString line = file.readLine();
		if (line.isEmpty()) {
			continue;
		}
		// Skip comments but not HttpOnly cookies
		if (!line.startsWith("#HttpOnly_") && line.startsWith('#')) {
			continue;
		}

		auto parts = line.split('\t');
		if (parts.size() < 7) {
			qDebug() << "Invalid cookie" << line;
			continue;
		}

		bool http_only = false;
		QString domain = parts[0];
		if (domain.startsWith("#HttpOnly_")) {
			domain = domain.mid(10);
			http_only = true;
		}


		QNetworkCookie c(parts[5].toLocal8Bit());
		c.setHttpOnly(http_only);
		c.setDomain(domain);
		c.setPath(parts[2]);
		c.setSecure(parts[3] != "FALSE");

		bool exp_ok = false;
		uint exp_int = parts[4].toUInt(&exp_ok);
		if (exp_ok) {
			QDateTime expires = QDateTime::fromTime_t(exp_int);
			c.setExpirationDate(expires);
		}
		c.setValue(parts.mid(6).join("\t").toLocal8Bit());
		m_store->setCookie(c);
		insertCookie(c);
	}
	return true;
}

