#pragma once
#include <iostream>
#include <sqlite3.h>

using namespace std;

const char* dir = "c:\\DeleteMe\\Bus.db";

sqlite3* DB;
string cm = ",";
string a = "'";
char* messaggeError;
const char* tbl_bus = "BUS";
const char* tbl_seat = "SEATS";
const char* col_bus = "NAME";
const char* col_seat = "SEAT";
class db {
public:
	static int CreateDB(const char* dir);
	static int CreateBUSTable(const char* dir);
	static int CreateTicketTable(const char* dir);
	static int InsertBUSData(const char* dir, const char* num, const char* name, const char* arrv, const char* dep, const char* from, const char* to);
	static int InsertTicketSeat(const char* dir, const char* seat);
	static int InsertTicketUser(const char* dir, const char* user);
	static int InsertTicketNum(const char* dir, const char* bus);
	static int Callback(void* NotUsed, int argc, char** argv, char** azColName);
	static int SelectData(const char* dir, const char* num);
	static int CheckBusExists(const char* dir, const char* num, int bus_num);
	
}db;

int db::CreateDB(const char* dir) {
	
	int exit = 0;
	exit = sqlite3_open(dir, &DB);
	sqlite3_close(DB);
	return 0;
}

int db::CreateBUSTable(const char* dir) {
	//string number = bus_num;

	string sql ( "CREATE TABLE IF NOT EXISTS BUS"
		"(NUMBER VARCHAR(50) PRIMARY KEY, "
		"NAME VARCHAR(50), "
		"DEPART VARCHAR(50), "
		"ARRIVAL VARCHAR(50), "
		"START VARCHAR(50), "//START = FROM
		"END VARCHAR(50)"//END = TO
		");" );

	try {
		int exit = 0;
		exit = sqlite3_open(dir, &DB);
		char* errorMsg;
		exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errorMsg);

		if (exit != SQLITE_OK) {
			cerr << "BusError!" << endl;
			sqlite3_free(errorMsg);
		}
		else {
			cout << "�������̺��� ����������ϴ�." << endl;
		}
		sqlite3_close(DB);
	}
	catch (const exception& e) {
		cerr << e.what();
	}
	
	return 0;
}

int db::CreateTicketTable(const char* dir) { 
	string sql("CREATE TABLE IF NOT EXISTS SEATS"
		"(NUMBER VARCHAR(50), "
		"USER VARCHAR(50),"
		"SEAT VARCHAR(50) PRIMARY KEY,"
		"FOREIGN KEY (NUMBER) REFERENCES BUS (NUMBER)"
		");");

	try {
		int exit = 0;
		exit = sqlite3_open(dir, &DB);
		char* errorMsg;
		exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errorMsg);

		if (exit != SQLITE_OK) {
			cerr << "BusError!" << endl;
			sqlite3_free(errorMsg);
		}
		else {
			cout << "�������̺��� ����������ϴ�." << endl;
		}
		sqlite3_close(DB);
	}
	catch (const exception& e) {
		cerr << e.what();
	}

	return 0;
}

int db::InsertBUSData(const char* dir, const char* num, const char* name, const char* dep, const char* arrv,  const char* from, const char* to) {
	
	string strnum = num, strname = name, strdep = dep, strarrv = arrv, strfrom = from, strto = to;
	
	int exit = sqlite3_open(dir, &DB);
	
	string sql("INSERT INTO BUS (NUMBER, NAME, DEPART, ARRIVAL, START, END) VALUES("
		+ strnum + cm +  "'"+strname+"'" + cm + "'"+ strdep+"'"+  cm + "'"+strarrv+"'" + cm 
		+ "'"+strfrom + "'"+"," + "'"+strto + "'"+");"
	);

	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK) {// db ����
		cerr << "Error! ����" << endl;
		sqlite3_free(messaggeError);
	}
	else
		cout << "Success! ����" << endl;

	return 0;
}
int db::InsertTicketNum(const char* dir, const char* bus_no) {
	int exit = sqlite3_open(dir, &DB);
	string strnum = bus_no;
	string sql("INSERT INTO SEATS (NUMBER)"
		"VALUES(" + strnum + ");"
	);

	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK) {// db ����
		cerr << "Error! ��ȣ" << endl;
		sqlite3_free(messaggeError);
	}
	else
		cout << "Success! ��ȣ" << endl;

	return 0;
}
int db::InsertTicketSeat(const char* dir, const char* seat) {
	int exit = sqlite3_open(dir, &DB);
	string strseat = seat;
	//string sql("INSERT INTO SEATS (SEAT)"
	//	"VALUES('" + strseat + "');"
	//);
	string sql("UPDATE SEATS SET SEAT = "
		+ a + strseat + a +
		" WHERE SEAT is NULL");
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK) {// db ����
		cerr << "Error! ��Ʈ" << endl;
		sqlite3_free(messaggeError);
	}                            
	else
		cout << "Success! ��Ʈ" << endl;

	return 0;
}

int db::InsertTicketUser(const char* dir, const char* user) {
	int exit = sqlite3_open(dir, &DB);
	string struser = user;
	
	string sql("UPDATE SEATS SET USER = "
		+a+struser+a+
		" WHERE USER is NULL");

	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

	if (exit != SQLITE_OK) {
		cerr << "Error! ���� ������" << endl;
		sqlite3_free(messaggeError);
	}
	else
		cout << "Success! ���� ������" << endl;
	sqlite3_close(DB);

	return 0;
}

int db::Callback(void* NotUsed, int argc, char** argv, char** azColName) {
	for (int i = 0; i < argc; i++) {
		cout << azColName[i] << ": " << argv[i] << endl;
	}
	cout << endl;
	return 0;
}


int db::SelectData(const char* dir, const char* num) {

	int exit = sqlite3_open(dir, &DB);
	string strnum = num;
	string sql = ("SELECT NUMBER FROM BUS "
		"WHERE NUMBER IS " + a + strnum + a
		);

	exit = sqlite3_exec(DB, sql.c_str(), Callback, 0, 0);

	return 0;
}


int db::CheckBusExists(const char* dir, const char* num, int bus_num) {

	int exit = sqlite3_open(dir, &DB);
	string strnum = num;
	string sql = ("SELECT NUMBER FROM BUS "
		"WHERE NUMBER IS " + a + strnum + a
		);

	exit = sqlite3_exec(DB, sql.c_str(), Callback, 0, 0);
	

	return 0;
}
