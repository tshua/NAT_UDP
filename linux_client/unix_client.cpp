#include "unix_client.h"
#include <pthread.h>

UnixClient uc;

void* pthread_recv(void*)
{
	uc.parseMessage();

	return NULL;
}

int main()
{
	cout<< "\033c" << flush;
	cout<< "----------------------NAT PUNCH--------------------------" << endl;
	cout << "input your host name:" <<endl;
	cin >> uc.myName;

	uc.login();
	pthread_t pid = 0;
	int ret = pthread_create(&pid, NULL, pthread_recv, NULL);
	if(ret < 0)
	{
		perror("pthread_create is fail");
		exit(-1);
	}

	uc.outputMessage();

	string commoned;
	while(1)
	{
		cin>>commoned;
		if(uc.parseCommoned(commoned) < 0)
			break;
	}


}
