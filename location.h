#pragma once

#include <QLabel>
#include <QUrl>

class Location: public QLabel
{
	Q_OBJECT
public:
	Location(QWidget *parent=0);

public slots:
	void setLocation(const QString& url);
	void setNext(const QString& newUrl);

private:
	QString m_url;
};
