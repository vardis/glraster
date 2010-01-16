/*
 * GLException.h
 *
 *  Created on: Jan 15, 2010
 *      Author: giorgos
 */

#ifndef GLEXCEPTION_H_
#define GLEXCEPTION_H_

enum ErrorCode {
	// success
	E_SUCCESS = 0,

	// operation was not valid for the current state
	E_BADOP,

	// the method argument was wrong
	E_BADARG,

	// unimplemented code
	E_NOTIMPL,

	// index was out of range or negative
	E_BADINDEX,

	// the internal state of an object was bad, e.g. an assertion failed
	E_BADSTATE,

	// failed to allocate enough memory
	E_NOMEM
};

class GLException: public std::exception {

private:
	enum ErrorCode m_code;

public:
	GLException(enum ErrorCode code) :
		std::exception(), m_code(code) {

	}

	virtual const char* what() const throw () {
		return errorCodeToString(m_code);
	}

private:
	const char* errorCodeToString(enum ErrorCode code) const {
		switch (code) {
		case E_BADOP:
			return "The operation was not valid for the current state";
		case E_BADARG:
			return "One of the method's arguments was invalid";
		case E_NOTIMPL:
			return "Unimplemented method";
		case E_BADINDEX:
			return "index was out of range or negative";
		case E_BADSTATE:
			return "the internal state of an object was bad, e.g. an assertion failed";
		case E_NOMEM:
			return "Failed to allocate enough memory";
		default:
			return "An unknown error occured";
		}
	}
};

#endif /* GLEXCEPTION_H_ */
