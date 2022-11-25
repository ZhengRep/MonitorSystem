#include "StringTable.h"
#include "ResourceLoader.h"

std::map<UINT, StringStorage> StringTable::_cache;

const TCHAR* StringTable::getString(UINT id)
{
	if (_cache.find(id) == _cache.end()) {
		StringStorage string;
		if (ResourceLoader::getInstance()->loadString(id, &string)) {
			_cache[id] = string;
		}
		else {
			return _T("Requested string from StringTable cannot be received");
		}
	}
	return (*_cache.find(id)).second.getString();
}

StringTable::StringTable()
{
}
