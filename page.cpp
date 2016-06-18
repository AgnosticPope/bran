#include "page.h"
#include "view.h"

Page::Page(QWebEngineProfile *profile, QObject *parent)
:QWebEnginePage(profile, parent), m_certificate_handler(0)
{
	
}

void Page::setCertificateErrorHandler(CertificateErrorHandler *handler)
{
	m_certificate_handler = handler;
}

bool Page::certificateError(const QWebEngineCertificateError &err)
{
	if (m_certificate_handler) {
		return m_certificate_handler->handleCertificateError(err);
	} else {
		return QWebEnginePage::certificateError(err);
	}
}
