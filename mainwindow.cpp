#include "mainwindow.h"

#include <QShortcut>
#include <QKeyEvent>

const int MODE_DASHBOARD = 0;
const int MODE_BROWSER = 1;
const int MODE_DOWNLOADS = 2;

MainWindow::MainWindow(Browser *browser, QWidget *parent)
:QStackedWidget(parent), m_browser(browser)
{
	m_dashboard = new Dashboard(m_browser->history(), this);
	m_downloads = new QListView(this);
	connect(m_downloads, &QListView::activated,
			this, &MainWindow::downloadActivated);

	addWidget(m_dashboard);
	addWidget(m_browser);
	addWidget(m_downloads);

	connect(m_dashboard, &Dashboard::loadUrl,
			this, &MainWindow::loadUrl);

	QShortcut *dashShortcut = new QShortcut(QKeySequence("Ctrl+L"), this);
	connect(dashShortcut, &QShortcut::activated,
			this, &MainWindow::dashboardMode);
	QShortcut *downloadShortcut = new QShortcut(QKeySequence("Ctrl+D"), this);
	connect(downloadShortcut, &QShortcut::activated,
			this, &MainWindow::downloadMode);
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
	if (ev->key() == Qt::Key_Escape) {
		if (currentIndex() != MODE_BROWSER) {
			browserMode();
			return;
		}
	}
	QWidget::keyPressEvent(ev);
}

void MainWindow::dashboardMode()
{
	if (currentIndex() == MODE_DASHBOARD) {
		return;
	}

	qDebug() << "DASHBOARD mode";
	setCurrentIndex(MODE_DASHBOARD);
	m_dashboard->setInput(m_browser->url().toString());
	m_dashboard->selectInput();
	// Reset the view AFTER setting the text and skip the
	// filter
	m_dashboard->resetView();
}
void MainWindow::loadUrl(const QUrl& url)
{
	m_browser->load(url);
	browserMode();
}
void MainWindow::browserMode()
{
	if (currentIndex() == MODE_BROWSER) {
		return;
	}
	qDebug() << "BROWSER mode";
	setCurrentIndex(MODE_BROWSER);
}
void MainWindow::downloadMode()
{
	if (currentIndex() == MODE_DOWNLOADS) {
		return;
	}
	qDebug() << "DOWNLOAD mode";
	setCurrentIndex(MODE_DOWNLOADS);
}

void MainWindow::setDownloadModel(QAbstractItemModel *model)
{
	m_downloads->setModel(model);
}
