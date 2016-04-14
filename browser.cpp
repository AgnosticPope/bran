#include "browser.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QDebug>

Browser::Browser(QWebEngineProfile *profile, QWidget *parent)
:QWidget(parent)
{
	m_view = new View(profile, this);
	m_view->setCertificateErrorHandler(this);
	m_urlLabel = new QLabel(this);

	m_progress = new QProgressBar(this);
	m_progress->setRange(0, 100);
	m_progress->setVisible(false);
	m_progress->setTextVisible(false);
	m_progress->setMaximumHeight(4);
	m_certificate_error = new CertificateErrorWidget(this);
	m_certificate_error->hide();

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(m_urlLabel);
	layout->addWidget(m_progress);
	layout->addWidget(m_certificate_error);
	layout->addWidget(m_view);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	setLayout(layout);

	connect(m_view, &View::urlChanged,
			this, &Browser::urlChanged);
	connect(m_view, &View::loadProgress,
			m_progress, &QProgressBar::setValue);
	connect(m_view, &View::loadStarted,
			this, &Browser::loadStarted);
	connect(m_view, &View::loadFinished,
			this, &Browser::loadFinished);
	connect(m_view, &View::linkHovered,
			this, &Browser::linkHovered);
}

void Browser::load(const QUrl& url)
{
	m_view->load(url);
	emit loadRequested(url);
}

void Browser::urlChanged(const QUrl& url)
{
	m_urlLabel->setText(url.toString());
}

void Browser::loadStarted()
{
	m_progress->setVisible(true);
}

void Browser::loadFinished(bool ok)
{
	m_progress->setVisible(false);
}

void Browser::setDefaultZoom(qreal zoomFactor)
{
	m_view->setDefaultZoom(zoomFactor);
}

QUrl Browser::url() const
{
	return m_view->url();
}

bool Browser::handleCertificateError(const QWebEngineCertificateError& err)
{
	m_view->hide();
	m_urlLabel->hide();
	m_certificate_error->show();
	m_certificate_error->setIgnoreable(err.isOverridable());
	m_certificate_error->setMessage(err.errorDescription());
	m_certificate_error->setUrl(err.url());

	QEventLoop loop;
	// Exit if ignored or when the user asks for a new URL
	connect(m_certificate_error, &CertificateErrorWidget::ignored,
			&loop, &QEventLoop::quit);
	connect(this, &Browser::loadRequested,
			&loop, &QEventLoop::quit);

	bool result = false;
	connect(m_certificate_error, &CertificateErrorWidget::ignored,
			[&result](){
		result = true;	
	});
	loop.exec();

	m_view->show();
	m_urlLabel->show();
	m_certificate_error->hide();

	return result;
}

void Browser::linkHovered(const QString& url)
{
	if (url.isEmpty()) {
		m_urlLabel->setText(m_view->url().toString());
	} else {
		m_urlLabel->setText(url);
	}
}
