#include"system.h"
#include"addressee.h"
#include"Cdate.h"
#include"staff.h"

System::System(Staff& staff,DbConnection& Db)
{
	this->staff = &staff;
	this->Db = &Db;
}

System::~System()
{
	if (this->addressee) {
		delete this->addressee;
	}
}

void System::initInterface()
{
	int a;
	cout << "����������(1:Ա����2���û���3���˳�)" << endl;
	cin >> a;
	switch (a) {
	case 1:staffLogin();
		break;
	case 2:addresseeInterface();
		break;
	case 3:exit(0);
		break;
	default:
		break;
	}
}

void System::addresseeInterface()
{
	int a;
	cout << "�������ѡ��1��ע�᣻2����¼��3�����������棩" << endl;
	cin >> a;
	switch (a) {
	case 1:addresseeRegist();
		break;
	case 2:addresseeLogin();
		break;
	case 3: initInterface();
		break;
	default:
		break;
	}
}

void System::addrresseeFunction()
{
	cout << "�������ѡ��1����ѯ�����Ϣ��2��ȡ����3�����������棩" << endl;
	int a;
	string pickupCode;
	cin >> a;
	switch (a) {
	case 1:
		this->addressee->checkPackage();
		addrresseeFunction();
		break;
	case 2:
		cout << "������ȡ����" << endl;
		cin >> pickupCode;
		if (this->addressee->pickup(pickupCode)) {
			cout << "ȡ���ɹ�" << endl;
		}
		else {
			cout << "ȡ��ʧ��" << endl;
		}
		addrresseeFunction();
		break;
	case 3: 
		initInterface();
		break;
	default:
		break;
	}
	return;
}

void System::staffFunction()
{
	cout << "�������ѡ��1����ѯ�����Ϣ��2����ѯĳ����Ϣ��3�����¿����Ϣ��4����ȡ���;5:���������棩" << endl;
	int a;
	cin >> a;
	int choice;
	int b;
	string c;
	string pickupCode;
	Cdate goalDate;
	int year, month, day;
	switch (a) {
	case 1:
		cout << "����1���ֻ������ѯ������2��ȡ�����ѯ,����3��ѯ���еĿ����Ϣ" << endl;
		cin >> choice;
		if (choice == 1) {
			cout << "�������ֻ���" << endl;
			cin >> c;
			this->staff->checkPackage_number(c);
			staffFunction();
		}
		else if(choice==2) {
			cout << "������ȡ����" << endl;
			cin >> c;
			this->staff->checkPackage_pickupCode(c);
			staffFunction();
		}
		else {
			this->staff->checkPackage_ALL();
			staffFunction();
		}
		break;
	case 2:
		cout << "����������Ҫ��ѯ���ڵ�������" << endl;
		cin >> year >> month >> day;
		goalDate.initDate(year, month, day);
		this->staff->checkDate(goalDate);
		staffFunction();
		break;
	case 3:
		cout << "���������ѡ��1�����ӣ�2��ɾ����3���޸ģ�" << endl;
		cin >> choice;
		switch (choice) {
		case 1:
			staffAdd();
			staffFunction();
			break;
		case 2:
			cout << "�����루1��ֻɾ�����������Ϣ��2��ɾ����������ȡ��ݵ���Ϣ��" << endl;
			cin >> b;
			if (b == 1) {
				staffDelete();
			}
			else {
				this->staff->dropPackageALL();
			}
			staffFunction();
			break;
		case 3:
			staffAlter();
			staffFunction();
			break;
		default:
			break;
		}
		break;
	case 4:
		cout << "������ȡ����" << endl;
		cin >> pickupCode;
		if (this->staff->getPackage(pickupCode)) {
			cout << "ȡ���ɹ�" << endl;
		}
		else {
			cout << "ȡ��ʧ��" << endl;
		}
		staffFunction();
		break;
	case 5: initInterface();
		break;
	default:
		break;
	}
}

void System::addresseeLogin()
{
	cout << "����������ֻ��ź�����" << endl;
	string number, password;
	cin >> number >> password;
	
	string SQL;
	char sql[200];
	SQL = "select * from addressee where number='" + number + "';";
	strcpy_s(sql, SQL.length() + 1, SQL.c_str());
	MYSQL_RES* res = Db->executeQuery(sql);
	MYSQL_ROW row = mysql_fetch_row(res);
	if (mysql_num_rows(res)) {
		if (password==row[3]) {
			if (this->addressee) {
				delete this->addressee;
			}
			this->addressee = new Addressee(row[0], row[1], row[2], row[3], Db);
			cout << "��¼�ɹ�,��ӭ�� " << row[0] << endl;
			addrresseeFunction();
		}
		else {
			cout << "��¼ʧ�ܣ��������" << endl;
			initInterface();
		}
	}
	else {
		cout << "��¼ʧ�ܣ�����δע���" << endl;
		initInterface();
	}

	return;
}

void System::addresseeRegist()
{
	cout << "���������� ���� �ֻ��� �ռ���ַ ����" << endl;
	string name, number, address, password;
	cin >> name >> number >> address >> password;
	Addressee addressee(name, number, address, password, Db);

	if (addressee.regist()) {
		cout << "ע��ɹ��������û��������µ�¼" << endl;	
	}
	else {
		cout << "ע��ʧ�ܣ����������Ѿ�ӵ�и��û�" << endl;
	}
	addresseeInterface();
}

void System::staffAdd()
{
	cout << "��������Ҫ¼��������Ϣ���ռ��˵绰���ļ����������ļ��˵绰���ļ��˵�ַ����ݵ��ţ���ݹ�˾���������ʱ࣬ȡ���룩" << endl;
	cout << "ע�⣺�ռ��˵绰 �������Ѿ��ڱ���վע����ĵ绰����¼�룬����¼��ʧ��" << endl;
	string addresseeNumber, senderName, senderNumber, senderAddress, trackingNumber, courierCompany, zipCode,pickupCode;
	int weight;
	cin >> addresseeNumber>> senderName >> senderNumber >> senderAddress >> trackingNumber >> courierCompany >> weight >> zipCode>>pickupCode;
	if (this->staff->addPackage(addresseeNumber, senderName, senderNumber, senderAddress, trackingNumber, courierCompany, weight, zipCode,pickupCode)) {
		cout << "¼��ɹ�" << endl;
	}
	else {
		cout << "¼��ʧ�ܣ��ռ�����δע��� ���� ��ݵ����ظ�" << endl;
	}
	staffFunction();
}

void System::staffAlter()
{
	cout << "�������ݵ���" << endl;
	string trackingNumber;
	cin >> trackingNumber;

	cout << "��������Ҫ�޸ĵ���Ϣ���1���ļ�����Ϣ��2���ռ�����Ϣ��3�������Ϣ��" << endl;
	int choice,type;
	cin >> type;
	switch (type) {
	case 1:
		cout << "��������Ҫ�޸ĵ���Ϣ(1���ļ���������2���ļ��˵绰��3���ļ��˵�ַ)" << endl;
		break;
	case 2:
		cout << "��������Ҫ�޸ĵ���Ϣ(1���ռ��˵绰)" << endl;
		break;
	case 3:
		cout << "��������Ҫ�޸ĵ���Ϣ(1����ݹ�˾��2�����������3���ʱ�)" << endl;
		break;
	default:
		break;
	}
	cin >> choice;
	cout << "�������޸ĺ����Ϣ" << endl;
	string newInformation;
	cin >> newInformation;
	bool a=true, b=true, c=true;
	switch (type) {
	case 1:
		a=this->staff->subAlterPackage1(trackingNumber, choice, newInformation);
		break;
	case 2:
		b=this->staff->subAlterPackage2(trackingNumber, choice, newInformation);
		break;
	case 3:
		c=this->staff->subAlterPackage3(trackingNumber, choice, newInformation);
		break;
	default:
		break;
	}
	if (a && b && c) {
		cout << "�޸ĳɹ�" << endl;
	}
	else {
		cout << "�޸�ʧ��" << endl;
	}
	staffFunction();
}


void System::staffDelete()
{
	string trickingNumber;
	cout << "������Ҫɾ���Ŀ�ݵ���" << endl;
	cin >> trickingNumber;
	if (this->staff->dropPackage(trickingNumber)) {
		cout << "ɾ���ɹ�" << endl;
	}
	else {
		cout << "ɾ��ʧ��" << endl;
	}
}

void System::SetAddressee(Addressee& addreessee)
{
	this->addressee = &addreessee;
}

void System::staffLogin()
{
	cout << "���������룬����Զ�����" << endl;
	cout << "��ʾ��Ա����ʼ����Ϊ123" << endl;
	string password;
	cin >> password;
	if (staff->identify_staff(password)){
		staffFunction();
	}
	else {
		initInterface();
	}
}
