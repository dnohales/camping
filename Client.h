#ifndef CLIENT_H
#define CLIENT_H

#include "ActiveRecord.h"

class Client : public ActiveRecord
{
    Q_OBJECT
	ACTIVE_RECORD(Client, ClientCollection)

public:
	QString tableName();
};

#endif // CLIENT_H
