#pragma once

#include <qdockwidget.h>
#include <qtextedit.h>

class MessageDockWidget :
    public QDockWidget
{
public:
    MessageDockWidget(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~MessageDockWidget() = default;
protected:

    QTextEdit* m_pQTextEdit{nullptr};
};

