#pragma warning(disable : 4996)
#include <cstdlib>
#include <sstream>
#include <string>
#include "wcharUtil.h"
using std::stringstream;
using std::wstringstream;
using std::string;
using std::wstring;

// global variables
const int WCHAR_MAX_COUNT = 16;                                 // max number of string buffers
const int WCHAR_MAX_LENGTH = 1024;                              // max string length per buffer
static wchar_t wchar_wideStr[WCHAR_MAX_COUNT][WCHAR_MAX_LENGTH];// circular buffer for wchar_t*
static char wchar_str[WCHAR_MAX_COUNT][WCHAR_MAX_LENGTH];       // circular buffer for char*
static int wchar_indexWchar = 0;                                // current index of circular buffer
static int wchar_indexChar = 0;                                 // current index of circular buffer


const wchar_t* toWchar(const char *src)
{
    wchar_indexWchar = (++wchar_indexWchar) % WCHAR_MAX_COUNT;  // circulate index

    mbstowcs(wchar_wideStr[wchar_indexWchar], src, WCHAR_MAX_LENGTH); // copy string as wide char
    wchar_wideStr[wchar_indexWchar][WCHAR_MAX_LENGTH-1] = L'\0';      // in case when source exceeded max length

    return wchar_wideStr[wchar_indexWchar];                     // return string as wide char
}

const wchar_t* toWchar(double number)
{
    wchar_indexWchar = (++wchar_indexWchar) % WCHAR_MAX_COUNT;  // circulate index

    // convert a number to string
    wstring wstr;
    wstringstream wss;
    wss << number;
    wstr = wss.str();

    wcscpy(wchar_wideStr[wchar_indexWchar], wstr.c_str());      // copy it to circular buffer

    return wchar_wideStr[wchar_indexWchar];                     // return string as wide char
}
const wchar_t* toWchar(long number)
{
    wchar_indexWchar = (++wchar_indexWchar) % WCHAR_MAX_COUNT;  // circulate index

    // convert a number to string
    wstring wstr;
    wstringstream wss;
    wss << number;
    wstr = wss.str();

    wcscpy(wchar_wideStr[wchar_indexWchar], wstr.c_str());      // copy it to circular buffer

    return wchar_wideStr[wchar_indexWchar];                     // return string as wide char
}

const char* toChar(const wchar_t* src)
{
    wchar_indexChar = (++wchar_indexChar) % WCHAR_MAX_COUNT;    // circulate index

    wcstombs(wchar_str[wchar_indexChar], src, WCHAR_MAX_LENGTH);// copy string as char
    wchar_str[wchar_indexChar][WCHAR_MAX_LENGTH-1] = '\0';      // in case when source exceeded max length

    return wchar_str[wchar_indexChar];                          // return string as char
}

const char* toChar(double number)
{
    wchar_indexChar = (++wchar_indexChar) % WCHAR_MAX_COUNT;    // circulate index

    // convert a number to string
    string s;
    stringstream ss;
    ss << number;
    s = ss.str();

    strcpy(wchar_str[wchar_indexChar], s.c_str());              // copy to circular buffer

    return wchar_str[wchar_indexChar];                          // return string as char
}

const char* toChar(long number)
{
    wchar_indexChar = (++wchar_indexChar) % WCHAR_MAX_COUNT;    // circulate index

    // convert a number to string
    string s;
    stringstream ss;
    ss << number;
    s = ss.str();

    strcpy(wchar_str[wchar_indexChar], s.c_str());              // copy to circular buffer

    return wchar_str[wchar_indexChar];                          // return string as char
}
