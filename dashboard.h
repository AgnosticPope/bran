#pragma once

#include <QLineEdit>

class Dashboard: public QWidget {
	Q_OBJECT
public:
	Dashboard(QWidget *parent=0);
public slots:
	void setInput(const QString& text);
	void selectInput();
signals:
	void loadUrl(const QUrl& url);
protected slots:
	void inputReturnPressed();
private:
	QLineEdit *m_input;
};

