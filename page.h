#pragma once

#include <QWebEnginePage>

class CertificateErrorHandler {
public:
	virtual bool handleCertificateError(const QWebEngineCertificateError&)=0;
};

class View;
class Page: public QWebEnginePage
{
	Q_OBJECT
public:
	Page(QWebEngineProfile *profile, QObject *parent=0);
	void setCertificateErrorHandler(CertificateErrorHandler *handler);
protected:
	virtual bool certificateError(const QWebEngineCertificateError &err) Q_DECL_OVERRIDE;

private:
	CertificateErrorHandler *m_certificate_handler;
};
