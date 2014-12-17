#include "PostOffice.h"

#include "Json.h"
#include "Config.h"

#include <fstream>

using namespace std;

void PostOffice::sendMessage(Letter l)
{
	char filename[1024];
	strcpy(filename, USERDIR);
	strcat(filename, l.client_id.c_str());
	strcat(filename, "-letter");

	ofstream letter(filename);
	
	Json j;
	string json = j.serialize((Serializable&)l);
	letter << json << endl;
}