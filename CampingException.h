#ifndef CAMPINGEXCEPTION_H
#define CAMPINGEXCEPTION_H

#include <QString>

class CampingException
{
public:
    explicit CampingException(QString message = "", int code = 0);
	QString message() const;
	void setMessage(QString message);
	int code() const;
	void setCode(int code);
private:
	QString _message;
	int _code;
};

#endif // CAMPINGEXCEPTION_H
