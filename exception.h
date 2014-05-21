#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QString>
#include <string>
using namespace std;

class Exception
{
public:
    Exception();

    static string OUTPUT_SCREENS_INCONSISTENT(){return "Inconsistent output screen list";}
    static string TEST(){return "TEST";}
};

#endif // EXCEPTION_H
