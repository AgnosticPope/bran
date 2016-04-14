#include "dashboard.h"
#include <QVBoxLayout>
#include <QUrl>
#include <QShortcut>

Dashboard::Dashboard(QWidget *parent)
:QWidget(parent)
{
	m_input = new QLineEdit(this);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(m_input);
	setLayout(layout);

	connect(m_input, &QLineEdit::returnPressed,
			this, &Dashboard::inputReturnPressed);
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
}
void Dashboard::setInput(const QString& text)
{
	m_input->setText(text);
}
