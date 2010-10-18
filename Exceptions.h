/*****************************************************************************
 * 
 * Jonathan Doman
 * 905260988
 * jmdoman@vt.edu
 * ECE 3574
 * 
\****************************************************************************/
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

// This macro allows you to easily define new exception classes to throw
#define exceptionClass(_X_) \
class _X_ : public std::runtime_error \
{ \
public: \
   _X_() : std::runtime_error( #_X_ ){} \
   _X_( const std::string& m ) : std::runtime_error( #_X_ ": " + m){} \
};

exceptionClass(FileAccessException)
exceptionClass(InvalidArgumentException)

#endif // EXCEPTIONS_H
