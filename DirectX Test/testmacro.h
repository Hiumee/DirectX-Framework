#pragma once
#include <exception>
#include <string>

class __exception
{
private:
    const char* errorMessage;
    int hResult;
public:
    __exception(const char* const string, int hr)
    {
        errorMessage = string;
        hResult = hr;
    }
    const char* what()
    {
        return errorMessage;
    }
    int result()
    {
        return hResult;
    }
};

#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)
#define S__FILE__ S_(__FILE__)
#define TEST_FAILED(x) if((hr = x)!=0){throw __exception("Ceva a mers prost:\n"  S__LINE__  "\n"  S__FILE__ "\n", hr);}
