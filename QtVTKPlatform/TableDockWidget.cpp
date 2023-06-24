#include "TableDockWidget.h"

TableDockWidget::TableDockWidget(QWidget* parent)
	: QDockWidget(parent)
{
	m_pQTableWidget = new QTableWidget(this);
	m_pQTableWidget->setEnabled(false);

	this->setWidget(m_pQTableWidget);
}
