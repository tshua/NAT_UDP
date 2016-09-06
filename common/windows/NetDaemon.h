#include <windows.h>

#include "WinSock.h"
#include <list>
using namespace std;
class NetDaemon
{
public:
	NetDaemon();
	virtual ~NetDaemon();
private:
	list<WinSock> socks;

};