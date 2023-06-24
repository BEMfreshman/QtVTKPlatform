#include "FileReader.h"

#include <AbstractFileReader.h>
#include <NASTRANFileReader.h>

FileReader::FileReader()
{
}

FileReader::~FileReader() {
	delete m_pAFR;
}

int FileReader::Read(const string& file) {

	if (file.find(".bdf") != string::npos) {
		if (m_pAFR) {
			delete m_pAFR;
		}
		m_pAFR = new MK::NASTRANFileReader(file);
	}

	return m_pAFR->Read();
}
