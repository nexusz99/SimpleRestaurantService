#include "RestraurantManager.h"
#include "Config.h"
#include "rapidjson\document.h"
#include "rapidjson\prettywriter.h"

#include <fstream>
#include <Shlwapi.h>

using namespace rapidjson;

void RestraurantManager::startup()
{
	loadUserList();
	loadPaymentHistory();
	loadMenuList();
}

void RestraurantManager::loadUserList()
{
	ifstream userfile(USERFILE);

	// Check If Userfile is Empty
	userfile.seekg(0, ifstream::end);
	int size = userfile.tellg();
	userfile.seekg(0, ifstream::beg);
	if(size ==0)
		return;

	Document doc;
	while(!userfile.eof())
	{
		string raw, id, pw;
		getline(userfile, raw);
		doc.Parse(raw.c_str());

		id = doc["username"].GetString();
		pw = doc["password"].GetString();

		UserData d;
		d.username = id;
		d.password = pw;
		userTable.put(id, d);
	}

	userfile.close();
}

void RestraurantManager::loadPaymentHistory()
{
	LinkedList<char*>* keyList = userTable.getKeys();
	keyList->ResetIterator();

	char* username;
	string line;

	Document doc;
	while((username=keyList->iter())!=NULL)
	{
		char filename[1024];
		strcpy(filename, USERDIR);
		strcat(filename, username);

		ifstream uph(filename);

		uph.seekg(0, ifstream::end);
		int size = uph.tellg();
		uph.seekg(0, ifstream::beg);

		if(size <= 0)
			continue;

		

		while(!uph.eof())
		{
			getline(uph, line);
			doc.Parse(line.c_str());
			
			PayData *p = new PayData();
			p->username = doc["username"].GetString();
			p->item = doc["item"].GetString();
			p->amount = doc["amount"].GetInt();
			p->cash = doc["cash"].GetInt();

			userTable.get(username).payList->InsertItem(p);
		}
		
		uph.close();
	}
}

bool RestraurantManager::verifyLogin(string username, string password)
{
	UserData d = userTable.get(username);

	if(d.username == username)
		return true;
	return false;
}

bool RestraurantManager::createNewUser(string username, string password)
{
	if(verifyLogin(username, password))
		return false;
	ofstream userfile(USERFILE, ios::app);
	StringBuffer buf;
	PrettyWriter<StringBuffer> writer(buf);
	writer.StartObject();
	writer.String("username");
	writer.String(username.c_str());
	writer.String("password");
	writer.String(password.c_str());
	writer.EndObject();

	userfile.seekp(0, ifstream::end);
	int size = userfile.tellp();

	if(size != 0)
		userfile << endl;

	userfile << buf.GetString();
	userfile.close();

	UserData ud;
	ud.username = username;
	ud.password = password;
	ud.payList = new LinkedList<PayData*>;
	userTable.put(username, ud);
	return true;
}

void RestraurantManager::writePaymentInfo(PayData* p)
{
	Json j;
	PayData tmp;
	tmp.amount = p->amount;
	tmp.cash = p->cash;
	tmp.item = p->item;
	tmp.username = p->username;
	string js = j.serialize((Serializable&)tmp);

	ofstream allhistory(PAYFILE, ios::app);
	allhistory.seekp(0, ifstream::end);
	int size = allhistory.tellp();

	if(size != 0)
		allhistory << endl;

	allhistory << js;
	allhistory.close();

	char filename[1024];
	strcpy(filename, USERDIR);
	strcat(filename, tmp.username.c_str());
	
	if(!PathFileExistsA(filename))
	{
		ofstream o(filename);
		o.close();
	}

	ofstream userhistory(filename, ios::app);
	userhistory.seekp(0, ifstream::end);
	size = userhistory.tellp();

	if(size != 0)
		userhistory << endl;

	userhistory << js;
	userhistory.close();

	payList.InsertItem(p);
	LinkedList<PayData*> &l = *(userTable.get(tmp.username).payList);
	l.InsertItem(p);
}

void RestraurantManager::loadMenuList()
{
	ifstream menufile(MENUFILE);

	menufile.seekg(0, ifstream::end);
	int size = menufile.tellg();
	menufile.seekg(0, ifstream::beg);

	if(size <= 0)
		return;

	Document doc;
	while(!menufile.eof())
	{
		string line;
		getline(menufile, line);

		doc.Parse(line.c_str());

		MenuData *m = new MenuData();
		m->name = doc["name"].GetString();
		m->id = doc["id"].GetInt();
		m->cost = doc["cost"].GetInt();
		m->time = doc["time"].GetInt();

		menuList.InsertItem(m);

	}
	menufile.close();
}

void RestraurantManager::createNewMenu(string name, int time, int cost)
{
	MenuData *m = new MenuData();
	MenuData jm;
	m->id = menuList.getCount() + 1;
	m->name = name;
	m->time = time;
	m->cost = cost;

	jm.id = m->id;
	jm.name = m->name;
	jm.time = m->time;
	jm.cost = m->cost;

	Json j;
	string js = j.serialize((Serializable&)jm);

	ofstream menufile(MENUFILE, ios::app);
	menufile.seekp(0, ifstream::end);
	int size = menufile.tellp();

	if(size != 0)
		menufile << endl;
	menufile << js;

	menufile.close();

	menuList.InsertItem(m);
}

string RestraurantManager::getItemNameById(int id)
{
	menuList.ResetIterator();

	MenuData* tmp;
	try
	{
		while((tmp = menuList.iter())!= NULL)
		{
			if(tmp->id == id)
				return tmp->name;
		}
	}
	catch(int i)
	{
	}
	return "";
}


int RestraurantManager::getCookTimeById(int id)
{
	menuList.ResetIterator();

	MenuData* tmp;
	while((tmp = menuList.iter())!= NULL)
	{
		if(tmp->id == id)
			return tmp->time;
	}
	return 0;
}