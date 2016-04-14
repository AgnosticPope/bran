#pragma once

#include <QWidget>
#include <QUrl>
#include "ui_certificateerror.h"

class CertificateErrorWidget: public QWidget
{
	Q_OBJECT
public:
	CertificateErrorWidget(QWidget *parent=0);
public slots:
	void setUrl(const QUrl& url);
	void setMessage(const QString& msg);
	void setIgnoreable(bool);
signals:
	void ignored();
private:
	Ui::CertificateError ui;
};
