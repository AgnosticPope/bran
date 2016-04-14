#include "view.h"

#include <QShortcut>
#include "page.h"

View::View(QWebEngineProfile *profile, QWidget *parent)
:QWebEngineView(parent), m_defaultZoom(1.0)
{
	Page* p = new Page(profile, this);
	setPage(p);

	connect(p, &QWebEnginePage::linkHovered,
			this, &View::linkHovered);

	connect(this, &QWebEngineView::titleChanged,
			this, &QWidget::setWindowTitle);

	QShortcut *zoomIn = new QShortcut(QKeySequence::ZoomIn, this);
	connect(zoomIn, &QShortcut::activated, this, &View::zoomIn);

	QShortcut *zoomOut = new QShortcut(QKeySequence::ZoomOut, this);
	connect(zoomOut, &QShortcut::activated, this, &View::zoomOut);

	QShortcut *zoomReset = new QShortcut(QKeySequence("Ctrl+0"), this);
	connect(zoomReset, &QShortcut::activated, this, &View::zoomReset);

	QShortcut *back = new QShortcut(QKeySequence::Back, this);
	connect(back, &QShortcut::activated, this, &View::back);

	QShortcut *forward = new QShortcut(QKeySequence::Forward, this);
	connect(forward, &QShortcut::activated, this, &View::forward);

	QShortcut *reload = new QShortcut(QKeySequence::Refresh, this);
	connect(reload, &QShortcut::activated, this, &View::reload);

	connect(this, &QWebEngineView::loadFinished,
			this, &View::loadIsFinished);
}

void View::zoomIn()
{
	if (zoomFactor() < 5.0) {
		setZoomFactor(zoomFactor() + 0.15);
	}
}
void View::zoomOut()
{
	if (0.25 < zoomFactor()) {
		setZoomFactor(zoomFactor() - 0.15);
	}
}
void View::zoomReset()
{
	setZoomFactor(m_defaultZoom);
}

void View::setDefaultZoom(qreal zoom)
{
	m_defaultZoom = zoom;
}

void View::loadIsFinished()
{
	// Restore zoom to correct value after the page is loaded
	setZoomFactor(m_defaultZoom);
}

void View::setCertificateErrorHandler(CertificateErrorHandler *handler)
{
	Page *p = static_cast<Page*>(page());
	p->setCertificateErrorHandler(handler);
}

