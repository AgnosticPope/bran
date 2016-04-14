#pragma once

#include <QStackedWidget>
#include <QListView>
#include <QAbstractItemModel>
#include "dashboard.h"
#include "browser.h"

class MainWindow: public QStackedWidget
{
	Q_OBJECT
public:
	MainWindow(Browser *browser, QWidget *parent=0);
	void setDownloadModel(QAbstractItemModel *model);

public slots:
	void loadUrl(const QUrl& url);
	void browserMode();
	void dashboardMode();
	void downloadMode();
protected:
	virtual void keyPressEvent(QKeyEvent *ev) Q_DECL_OVERRIDE;

signals:
	void downloadActivated(const QModelIndex &index);
private:
	Dashboard *m_dashboard;
	Browser *m_browser;
	QListView *m_downloads;
};
