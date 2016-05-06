#include "location.h"

Location::Location(QWidget *parent)
{

}

void Location::setLocation(const QString& url)
{
	m_url = url;
	setPalette(QPalette());
	setText(m_url);
}

void Location::setNext(const QString& newUrl)
{
	if (newUrl.isEmpty()) {
		setPalette(QPalette());
		setText(m_url);
	} else {
		QPalette p = palette();
		p.setColor(foregroundRole(), Qt::blue);
		setPalette(p);
		setText(newUrl);
	}
}
