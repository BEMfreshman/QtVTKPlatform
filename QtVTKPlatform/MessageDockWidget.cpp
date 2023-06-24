#include "MessageDockWidget.h"

MessageDockWidget::MessageDockWidget(QWidget* parent, 
	Qt::WindowFlags flags)
	:QDockWidget(parent, flags)
{
	m_pQTextEdit = new QTextEdit(this);
	m_pQTextEdit->setReadOnly(true);

	this->setWidget(m_pQTextEdit);
}
