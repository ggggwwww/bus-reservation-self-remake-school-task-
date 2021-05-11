#pragma once
#include <iostream>
#include <sqlite3.h>
#include <cstdio>
using namespace std;


enum DataExists {
	EXISTS = 0,
	NOT_EXISTS,
};

enum CheckSeatNull{
	IS_NULL =0,
	NOT_NULL,
};

const char* dir = "c:\\DeleteMe\\Bus.db";

sqlite3* DB;
string cm = ",";
string a = "'";
char* messaggeError;
const char* tbl_bus = "BUS";
const char* tbl_seat = "SEATS";
const char* col_bus = "NAME";
const char* col_seat = "SEAT";

int db_temp;
static string strSeat;
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
	static int CheckBusExists(const char* dir, const char* num);
	static int IsSeatNull(const char* dir, const char* seat);
	static int SelectBus(const char* dir);
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
			cout << "버스테이블이 만들어졌습니다." << endl;
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
		"SEAT VARCHAR(50) ,"//PRIMARY KEY 있엇으나 제거
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
			cout << "버스테이블이 만들어졌습니다." << endl;
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

	cout << "sql:" << sql << endl;

	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK) {// db 연결
		cerr << "Error! 버스" << endl;
		sqlite3_free(messaggeError);
	}
	else
		cout << "Success! 버스" << endl;

	return 0;
}

int db::InsertTicketNum(const char* dir, const char* bus_no) {
	int exit = sqlite3_open(dir, &DB);
	string strnum = bus_no;
	string sql("INSERT INTO SEATS (NUMBER)"
		"VALUES(" + strnum + ");"
	);

	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK) {// db 연결
		cerr << "Error! 번호" << endl;
		sqlite3_free(messaggeError);
	}
	else
		cout << "Success! 번호" << endl;

	return 0;
}

int db::InsertTicketSeat(const char* dir, const char* seat) {
	int exit = sqlite3_open(dir, &DB);
	string strseat = seat;
	//string sql("INSERT INTO SEATS (SEAT)"
	//	"VALUES('" + strseat + "');"
	//);
	string sql("UPDATE SEATS SET SEAT = "
		+strseat+
		" WHERE SEAT IS NULL");
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK) {// db 연결
		cerr << "Error! 시트" << endl;
		sqlite3_free(messaggeError);
	}                            
	else
		cout << "Success! 시트" << endl;

	return 0;
}

int db::InsertTicketUser(const char* dir, const char* user) {
	int exit = sqlite3_open(dir, &DB);
	string struser = user;
	
	string sql("UPDATE SEATS SET USER = "
		+a+struser+a+
		" WHERE USER IS NULL");

	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

	if (exit != SQLITE_OK) {
		cerr << "Error! 유저 데이터" << endl;
		sqlite3_free(messaggeError);
	}
	else
		cout << "Success! 유저 데이터" << endl;

	return 0;
}

int db::Callback(void* NotUsed, int argc, char** argv, char** azColName) {
	for (int i = 0; i < argc; i++) {
		cout << azColName[i] << ": " << argv[i] << endl;
	}
	cout << endl;
	return 0;
}

int db::CheckBusExists(const char* dir, const char* num) {

	int exit = sqlite3_open(dir, &DB);
	string strnum = num;
	string sql = ("SELECT NUMBER FROM BUS "
		"WHERE NUMBER IS " + a + strnum + a
		);

	//exit = sqlite3_exec(DB, sql.c_str(), Callback, 0, 0);
	db_temp = 0;
	exit = sqlite3_exec(DB, sql.c_str(), 
		[](void* NotUsed, int argc, char** argv, char** azColName) -> int{//lambda(Callback Function.) 
			db_temp = argc;
			return 0;
		}, 0, 0);

	//if (exit != SQLITE_OK) return NOT_EXISTS;
	if (db_temp <= 0) return NOT_EXISTS;
	else return EXISTS;
	sqlite3_free(DB);

	//if (exit == SQLITE_OK)
	//	return -1;
	//else if(exit != SQLITE_OK)
	//	return 1;

}

int db::IsSeatNull(const char* dir, const char* seat) {
	int exit = sqlite3_open(dir, &DB);
	strSeat = seat;
	/*string strBusNum = bus_num;
	int bus_order = atoi(bus_num);*/
	string sql = ("SELECT SEAT FROM SEATS WHERE EXISTS (SELECT SEAT FROM SEATS WHERE SEAT IS "+strSeat+")");
	db_temp = 0;

	exit = sqlite3_exec(DB, sql.c_str(), [](void* NotUsed, int argc, char** argv, char** azColName)->int {
		for (int i = 0; i < argc; i++) {
			//cout << azColName[i] << ": " << argv[i] << endl;
			if (argv[i] == strSeat) {
				cout << argv[i] << endl;
				return NOT_NULL;
			}
			else {
				//cout << argv[i] << endl;
				continue;
			}
		}
		cout << endl;
		}, 0, 0);

	sqlite3_free(DB);
	
	return exit;
}

int db::SelectBus(const char* dir) {
	int exit = sqlite3_open(dir, &DB);

	string sql = ("SELECT * FROM BUS");
	exit = sqlite3_exec(DB, sql.c_str(), Callback, 0, 0);
	cout << exit;
	sqlite3_free(DB);
	return 0;
}



