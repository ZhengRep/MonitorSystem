#include "WindowFinder.h"

struct WindowsParam
{
    std::vector<HWND>* hwndVector;
    StringVector* classNames;
};

std::vector<HWND> WindowFinder::findWindowsByClass(StringVector classNames)
{
    std::vector<HWND> hwndVector;
    if (classNames.empty()) {
        return hwndVector;
    }
    WindowsParam windowsParam;
    windowsParam.classNames = &classNames;
    windowsParam.hwndVector = &hwndVector;
    EnumWindows(findWindowsByClassFunc, (LPARAM)&windowsParam);
    return hwndVector;
}

HWND WindowFinder::findFirstWindowByName(const StringStorage windowName)
{
    std::vector<HWND> hwndVector;
    StringVector winNameVector;
    winNameVector.push_back(windowName);
    winNameVector[0].toLowerCase();
    WindowsParam winParams = { &hwndVector, &winNameVector };

    EnumWindows(findWindowsByNameFunc, (LPARAM)&winParams);
    if (hwndVector.size() != 0) {
        return hwndVector[0];
    }
    else {
        return 0;
    }
}

BOOL WindowFinder::findWindowsByClassFunc(HWND hwnd, LPARAM lParam)
{
    if (IsWindowVisible(hwnd)) {
        WindowsParam* windowsParam = (WindowsParam*)lParam;
        StringVector::iterator classNameIter;
        const size_t maxTcharCount = 256;
        TCHAR winName[maxTcharCount];
        if (GetClassName(hwnd, winName, maxTcharCount) != 0) {
            StringStorage nextWinName(winName);

            if (nextWinName.getLength() > 0 && hwnd != 0) {
                for (classNameIter = windowsParam->classNames->begin();
                    classNameIter != windowsParam->classNames->end(); classNameIter++) {
                    if (nextWinName.isEqualTo(&(*classNameIter))) {
                        windowsParam->hwndVector->push_back(hwnd);
                    }
                }
            }

            EnumChildWindows(hwnd, findWindowsByClassFunc, (LPARAM)windowsParam);
        }
    }
    return TRUE;
}

BOOL WindowFinder::findWindowsByNameFunc(HWND hwnd, LPARAM lParam)
{
    if (IsWindowVisible(hwnd) != 0) {
        const size_t maxTcharCount = 256;
        TCHAR nameChars[maxTcharCount];
        if (GetWindowText(hwnd, nameChars, maxTcharCount)) {
            StringStorage winName(nameChars);
            winName.toLowerCase();

            if (winName.getLength() > 0 && hwnd != 0) {
                WindowsParam* winParams = (WindowsParam*)lParam;
                StringStorage* subStr = &(*(winParams->classNames)).front();
                if (_tcsstr(winName.getString(), subStr->getString())) {
                    (*(winParams->hwndVector)).push_back(hwnd);
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}
