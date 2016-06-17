#pragma once

#include <QLineEdit>
#include <QListWidget>
#include <QWebEngineHistory>

class Dashboard: public QWidget {
	Q_OBJECT
public:
	Dashboard(QWebEngineHistory *history, QWidget *parent=0);
public slots:
	void setInput(const QString& text);
	void selectInput();
	void resetView();
signals:
	void loadUrl(const QUrl& url);
protected slots:
	void inputReturnPressed();
	void textChanged(const QString& text);
	void itemActivated(QListWidgetItem *item);
private:
	QLineEdit *m_input;
	QListWidget *m_completionView;
	QWebEngineHistory *m_history;
};

