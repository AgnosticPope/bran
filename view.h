#pragma once

#include <QWebEngineView> 
#include "page.h"

class View: public QWebEngineView {
	Q_OBJECT
public:
	View(QWebEngineProfile *profile, QWidget *parent=0);
	void setCertificateErrorHandler(CertificateErrorHandler *handler);

public slots:
	void zoomIn();
	void zoomOut();
	void zoomReset();
	void setDefaultZoom(qreal zoomFactor);
signals:
	void linkHovered(const QString& url);

protected slots:
	void loadIsFinished();

private:
	qreal m_defaultZoom;
};
