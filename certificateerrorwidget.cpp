#include "certificateerrorwidget.h"

CertificateErrorWidget::CertificateErrorWidget(QWidget *parent)
:QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.button_ignore, &QPushButton::clicked,
			this, &CertificateErrorWidget::ignored);
}

void CertificateErrorWidget::setUrl(const QUrl& url)
{
	ui.url->setText(url.toString());
}

void CertificateErrorWidget::setMessage(const QString& msg)
{
	ui.error_message->setText(msg);
}

void CertificateErrorWidget::setIgnoreable(bool yes)
{
	ui.button_ignore->setVisible(yes);
}
