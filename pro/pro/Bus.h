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
		cout << "\n���� ��ȣ: ";
		cin >> bus_order;
		string s = to_string(bus_order);
		char const* num_char = s.c_str();

		/*if (*bus[bus_order].bus_no != NULL ) {
			cout << "\n�̹� �ִ� ���� ��ȣ �Դϴ�!\n";
			continue;
			*/

		if (db.CheckBusExists(dir, num_char) == EXISTS) {
			cout << "�̹� �����ϴ� ��ȣ�Դϴ�!";
			continue;
		}

		else if (bus_order > 999 || bus_order < 1) {
			cout << "\n��ȿ ���� �ʰ�!\n";
			continue;
		}
		else {
			strcpy(bus[bus_order].bus_no, num_char);

			cout << "\n������ �̸�: ";
			cin >> bus[bus_order].driver;

			cout << "\n��� �ð�: ";
			cin >> bus[bus_order].departT;

			cout << "\n���� �ð�: ";
			cin >> bus[bus_order].arrivalT;

			cout << "\n�����: ";
			cin >> bus[bus_order].from;

			cout << "\n������: ";
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
		cout << "\n���� ��ȣ: ";
		cin >> bus_order;
		string s = to_string(bus_order);
		char const* num_char = s.c_str();

		if (bus_order > 1000 || bus_order < 1)
			cout << "��ȿ ���� �ʰ�!(* 1~999������ ��ȣ�� �Է°���. *)";

		/*if (strcmp(bus[bus_order].bus_no, num_char) != 0) {
			cout << "�����Ͻ� ������ �������� �ʽ��ϴ�!!\n";
		}
		*/
		if (db.CheckBusExists(dir, num_char) == NOT_EXISTS) {
			cout << "�����Ͻ� ������ �������� �ʽ��ϴ�!!\n";
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
		cout << "\n�¼� ��ȣ: ";
		cin >> seat;
		string sSeat = to_string(seat);
		char const* cSeat = sSeat.c_str();
	
		if (seat > 32) {
			cout << "������ �¼� �� ���� Ů�ϴ�!\n";
			continue;
		}
		/*else if (*bus[bus_order].seat[seat].name != NULL) {
			cout << "�̹� �ٸ� �̿밴�� �ֽ��ϴ�!\n";
			continue;
		}*/
		if (db.IsSeatNull(dir, cSeat) == NOT_NULL) {
			cout << "�̹� �ٸ� �̿밴�� �ֽ��ϴ�!\n";
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
		cout << "\n������ �̸�: ";
		cin >> userName;
		if (userName == null) {//������ �Է����� ���
			cout << "������ �ȵ˴ϴ�!\n";
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
		cout << "\n���� ��ȣ: ";
		cin >> bus_order;
		string s = to_string(bus_order);
		char const* num_char = s.c_str();
		if (!cin)
			IsEnteredNum(bus_order);
		else if (bus_order >= 1000 || bus_order < 1) {
			cout << "��ȿ ���� �ʰ�!\n";
			continue;
		}
	/*	else if (strcmp(bus[bus_order].bus_no, num_char) != 0) {
			cout << "�ش� ������ �������� �ʽ��ϴ�!\n";
			break;
		}*/
		else if (db.CheckBusExists(dir, num_char) == NOT_EXISTS) {
			cout << "�����Ͻ� ������ �������� �ʽ��ϴ�!!\n";
			break;
		}
		else {
			//printf("\n���� ��ȣ: %s ������: %s ��߽ð�: %s �����ð�: %s\n�����: %s ������: %s\n",
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

void Bus::Available() {//���Ե� ��ȣ�� ���
	for (bus_order = 0; bus_order < 1000; bus_order++) {
		string s = to_string(bus_order);
		char const* num_char = s.c_str();

		if (strcmp(bus[bus_order].bus_no, num_char) != 0) {
			continue;
		}
		else if (strcmp(bus[bus_order].bus_no, num_char) == 0) {
			cout << bus[bus_order].bus_no << "�� ";
		}
		else {
			cout << "� ������ �����ϴ�!\n";
			break;
		}

	}
}