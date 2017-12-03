#include "CampingException.h"

CampingException::CampingException(QString message, int code)
{
	this->setMessage(message);
	this->setCode(code);
}

QString CampingException::message() const
{
	return this->_message;
}

void CampingException::setMessage(QString message)
{
	this->_message = message;
}

int CampingException::code() const
{
	return this->_code;
}

void CampingException::setCode(int code)
{
	this->_code = code;
}
