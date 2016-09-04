#include "server.h"

int main(int argc, char **argv)
{
	cout<< "\033c" << flush;
	cout<< "----------------------NAT PUNCH SERVER------------------------" <<endl;
	Server s(SERVER_PORT);
	s.start();
}
