#pragma once

#include <qdockwidget.h>
#include <qtablewidget.h>

class TableDockWidget : public QDockWidget
{
public:
	TableDockWidget(QWidget* parent = nullptr);
	~TableDockWidget() = default;

protected:

	QTableWidget* m_pQTableWidget{ nullptr };
};

