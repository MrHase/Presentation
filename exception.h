#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QString>
#include <exception>
#include <string>

using namespace std;

class InconsitentScreenException : public exception
{
public:
	InconsitentScreenException();

	string OUTPUT_SCREENS_INCONSISTENT = "Inconsistent output screen list";
	virtual const char* what() const throw() { return OUTPUT_SCREENS_INCONSISTENT.c_str(); }
};

class RendererException : public exception
{
public:
	RendererException();
	string NO_DOCUMENT_SET_IN_RENDERER_EXCEPTION = "No document was set in this renderer";
	virtual const char* what() const throw() { return NO_DOCUMENT_SET_IN_RENDERER_EXCEPTION.c_str(); }
};

#endif  // EXCEPTION_H
