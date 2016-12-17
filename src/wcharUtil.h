#pragma once

// conversion utility between multi-byte char and wide char

const wchar_t* toWchar(const char *str);        // convert char* to wchar_t*
const wchar_t* toWchar(double number);          // convert floating to wchar_t*
const wchar_t* toWchar(long number);            // convert integer to wchar_t*

const char* toChar(const wchar_t *str);         // convert wchar_t* to char*
const char* toChar(double number);              // convert floating to char*
const char* toChar(long number);                // convert integer to char*

