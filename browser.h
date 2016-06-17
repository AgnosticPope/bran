#pragma once

#include <QWidget>
#include "view.h"
#include "certificateerrorwidget.h"

class Location;
class QProgressBar;
class Browser: public QWidget, public CertificateErrorHandler
{
	Q_OBJECT
public:
	Browser(QWebEngineProfile *profile, QWidget *parent=0);
	void setDefaultZoom(qreal zoomFactor);
	QUrl url() const;
	virtual bool handleCertificateError(const QWebEngineCertificateError& err);
	QWebEngineHistory* history();
public slots:
	void load(const QUrl& url);
protected slots:
	void urlChanged(const QUrl& url);
	void loadStarted();
	void loadFinished(bool ok);
signals:
	void loadRequested(const QUrl& url);
private:
	Location *m_location;
	QProgressBar *m_progress;
	View *m_view;
	CertificateErrorWidget *m_certificate_error;
};

