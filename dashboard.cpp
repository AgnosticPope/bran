#include "dashboard.h"
#include <QVBoxLayout>
#include <QUrl>
#include <QShortcut>
#include <QWebEngineHistoryItem>

Dashboard::Dashboard(QWebEngineHistory *history, QWidget *parent)
:QWidget(parent)
{
	m_input = new QLineEdit(this);
	m_input->setPlaceholderText("Insert URL");
	m_history = history;
	m_completionView = new QListWidget(this);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(m_input);
	layout->addWidget(m_completionView);
	layout->setContentsMargins(1, 1, 1, 1);
	layout->setSpacing(1);
	setLayout(layout);

	connect(m_input, &QLineEdit::returnPressed,
			this, &Dashboard::inputReturnPressed);
	connect(m_input, &QLineEdit::textChanged,
			this, &Dashboard::textChanged);
	connect(m_completionView, &QListWidget::itemActivated,
			this, &Dashboard::itemActivated);
}

void Dashboard::resetView()
{
	m_completionView->clear();
	foreach(QWebEngineHistoryItem item, m_history->items()) {
		QListWidgetItem *w = new QListWidgetItem();
		w->setText(item.originalUrl().toString());
		w->setToolTip(item.title());
		m_completionView->addItem(w);
	}
}

void Dashboard::textChanged(const QString& text)
{
	if (text.isEmpty()) {
		for(int i=0; i<m_completionView->count(); i++) {
			QListWidgetItem *item = m_completionView->item(i);
			item->setHidden(false);
		}
	} else {
		for(int i=0; i<m_completionView->count(); i++) {
			QListWidgetItem *item = m_completionView->item(i);
			item->setHidden(!item->text().contains(text, Qt::CaseInsensitive) && 
					!item->toolTip().contains(text, Qt::CaseInsensitive));
		}
	}
}

void Dashboard::inputReturnPressed()
{
	if (!m_input->text().isEmpty()) {
		emit loadUrl(QUrl::fromUserInput(m_input->text()));
	}
}

void Dashboard::selectInput()
{
	m_input->selectAll();
	m_input->setFocus();
}
void Dashboard::setInput(const QString& text)
{
	m_input->setText(text);
}

void Dashboard::itemActivated(QListWidgetItem *item)
{
	emit loadUrl(QUrl::fromUserInput(item->text()));
}
