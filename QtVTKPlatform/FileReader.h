#pragma once
#include <qobject.h>

#include <string>

using std::string;

namespace MK {
    class AbstractFileReader;
}

class FileReader :
    public QObject
{
    Q_OBJECT

public:
    FileReader();
    ~FileReader();

    int Read(const string& file);
    MK::AbstractFileReader* GetReader() const {
        return m_pAFR;
    }

protected:

    MK::AbstractFileReader* m_pAFR{ nullptr };
};

