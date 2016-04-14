#pragma once

#include <QWidget>
#include "view.h"
#include "certificateerrorwidget.h"

class QLabel;
class QProgressBar;
class Browser: public QWidget, public CertificateErrorHandler
{
	Q_OBJECT
public:
	Browser(QWebEngineProfile *profile, QWidget *parent=0);
	void setDefaultZoom(qreal zoomFactor);
	QUrl url() const;
	virtual bool handleCertificateError(const QWebEngineCertificateError& err);
public slots:
	void load(const QUrl& url);
protected slots:
	void urlChanged(const QUrl& url);
	void loadStarted();
	void loadFinished(bool ok);
	void linkHovered(const QString& url);
signals:
	void loadRequested(const QUrl& url);
private:
	QLabel *m_urlLabel;
	QProgressBar *m_progress;
	View *m_view;
	CertificateErrorWidget *m_certificate_error;
};

