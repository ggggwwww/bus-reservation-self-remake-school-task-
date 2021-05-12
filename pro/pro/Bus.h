#pragma once
#pragma warning(disable:4996)
#include <iostream>
#include <string>
#include <typeinfo>
#include <stdlib.h>
#include <fstream>
#include <io.h>
#include <sqlite3.h>
#include <stdio.h>
#include "forbus.h"
#include "DB.h"

//#define IS_EXISTS -1
//#define noExists 1


using namespace std;

static int o = 0;
int bus_order;

class Bus {
private:
	char bus_no[1000], driver[100], arrivalT[50], departT[50], from[100], to[100];

	class seat {
	public:
		char name[100];

	}seat[32];

public:
	void Install();
	void Reservate();
	void ShowInform();
	void Available();
}bus[1000];


void Bus::Install() {
	ofstream fout;
	int length;

	while (1) {
		cout << "\n버스 번호: ";
		cin >> bus_order;
		string s = to_string(bus_order);
		char const* num_char = s.c_str();

		/*if (*bus[bus_order].bus_no != NULL ) {
			cout << "\n이미 있는 버스 번호 입니다!\n";
			continue;
			*/

		if (db.CheckBusExists(dir, num_char) == EXISTS) {
			cout << "이미 존재하는 번호입니다!";
			continue;
		}

		else if (bus_order > 999 || bus_order < 1) {
			cout << "\n유효 범위 초과!\n";
			continue;
		}
		else {
			strcpy(bus[bus_order].bus_no, num_char);

			cout << "\n운전수 이름: ";
			cin >> bus[bus_order].driver;

			cout << "\n출발 시간: ";
			cin >> bus[bus_order].departT;

			cout << "\n도착 시간: ";
			cin >> bus[bus_order].arrivalT;

			cout << "\n출발점: ";
			cin >> bus[bus_order].from;

			cout << "\n도착점: ";
			cin >> bus[bus_order].to;
			break;
		}
	}

	db.InsertBUSData(dir, bus[bus_order].bus_no, bus[bus_order].driver,
		bus[bus_order].departT, bus[bus_order].arrivalT, bus[bus_order].from, bus[bus_order].to);
}



void Bus::Reservate() {
	int seat;
	int bus_order;
	static int temp_bus_order;
	static string temp_s;
	static const char* temp_num_char;

	char userName[10];
	char null[4] = " ";
	
	while (1) {
		cout << "\n버스 번호: ";
		cin >> bus_order;
		string s = to_string(bus_order);
		char const* num_char = s.c_str();

		if (bus_order > 1000 || bus_order < 1)
			cout << "유효 범위 초과!(* 1~999까지의 번호만 입력가능. *)";

		/*if (strcmp(bus[bus_order].bus_no, num_char) != 0) {
			cout << "선택하신 버스는 존재하지 않습니다!!\n";
		}
		*/
		if (db.CheckBusExists(dir, num_char) == NOT_EXISTS) {
			cout << "선택하신 버스는 존재하지 않습니다!!\n";
			break;
		}
		else{
			db.InsertTicketNum(dir, num_char);
			temp_s = to_string(temp_bus_order);
			temp_num_char = temp_s.c_str();
			break;
		}
				
	}

	while (1) {
		cout << "\n좌석 번호: ";
		cin >> seat;
		string sSeat = to_string(seat);
		char const* cSeat = sSeat.c_str();
	
		if (seat > 32) {
			cout << "지정된 좌석 수 보다 큽니다!\n";
			continue;
		}
		/*else if (*bus[bus_order].seat[seat].name != NULL) {
			cout << "이미 다른 이용객이 있습니다!\n";
			continue;
		}*/
		if (db.IsSeatNull(dir, cSeat) == NOT_NULL) {
			cout << "이미 다른 이용객이 있습니다!\n";
			continue;
		}
		else if (db.IsSeatNull(dir, cSeat) == IS_NULL) {
			string s = to_string(seat);
			const char* num_char = s.c_str();
			db.InsertTicketSeat(dir, num_char);
			break;

		}

			
	}


	while (1) {
		cout << "\n예매자 이름: ";
		cin >> userName;
		if (userName == null) {//공백을 입력했을 경우
			cout << "공백은 안됩니다!\n";
			continue;
		}
		else {
			//strcpy(bus[bus_order].seat[seat].name, userName);
			db.InsertTicketUser(dir, userName);
			break;
		}
	}
	
}

void Bus::ShowInform() {
	while (1) {
		cout << "\n버스 번호: ";
		cin >> bus_order;
		string s = to_string(bus_order);
		char const* num_char = s.c_str();
		if (!cin)
			IsEnteredNum(bus_order);
		else if (bus_order >= 1000 || bus_order < 1) {
			cout << "유효 범위 초과!\n";
			continue;
		}
	/*	else if (strcmp(bus[bus_order].bus_no, num_char) != 0) {
			cout << "해당 버스는 존재하지 않습니다!\n";
			break;
		}*/
		else if (db.CheckBusExists(dir, num_char) == NOT_EXISTS) {
			cout << "선택하신 버스는 존재하지 않습니다!!\n";
			break;
		}
		else {
			//printf("\n버스 번호: %s 운전수: %s 출발시간: %s 도착시간: %s\n출발지: %s 도착지: %s\n",
			//	bus[bus_order].bus_no, bus[bus_order].driver, bus[bus_order].departT,
			//	bus[bus_order].arrivalT, bus[bus_order].from, bus[bus_order].to);
			cout << db.SelectBus(dir);
		/*	for (int i = 1; i <= 32; i++) {
				if (i == 32)
					printf("%d. %s\n ", i, bus[bus_order].seat[i].name);
				else
					printf("%d. %s ", i, bus[bus_order].seat[i].name);
			}*/
			cout << db.SelectSeat(dir);
	
			break;
		}
	}
}

void Bus::Available() {//기입된 번호들 출력
	for (bus_order = 0; bus_order < 1000; bus_order++) {
		string s = to_string(bus_order);
		char const* num_char = s.c_str();

		if (strcmp(bus[bus_order].bus_no, num_char) != 0) {
			continue;
		}
		else if (strcmp(bus[bus_order].bus_no, num_char) == 0) {
			cout << bus[bus_order].bus_no << "번 ";
		}
		else {
			cout << "어떤 버스도 없습니다!\n";
			break;
		}

	}
}