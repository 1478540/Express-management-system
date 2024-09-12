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
	cout << "输入你的身份(1:员工；2：用户；3：退出)" << endl;
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
	cout << "输入你的选择（1：注册；2：登录；3：返回主界面）" << endl;
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
	cout << "输入你的选择（1：查询快件信息；2：取件；3：返回主界面）" << endl;
	int a;
	string pickupCode;
	cin >> a;
	switch (a) {
	case 1:
		this->addressee->checkPackage();
		addrresseeFunction();
		break;
	case 2:
		cout << "请输入取件码" << endl;
		cin >> pickupCode;
		if (this->addressee->pickup(pickupCode)) {
			cout << "取件成功" << endl;
		}
		else {
			cout << "取件失败" << endl;
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
	cout << "输入你的选择（1：查询快件信息；2：查询某日信息；3：更新快件信息；4：代取快件;5:返回主界面）" << endl;
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
		cout << "输入1用手机号码查询，输入2用取件码查询,输入3查询所有的快递信息" << endl;
		cin >> choice;
		if (choice == 1) {
			cout << "请输入手机号" << endl;
			cin >> c;
			this->staff->checkPackage_number(c);
			staffFunction();
		}
		else if(choice==2) {
			cout << "请输入取件码" << endl;
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
		cout << "请依次输入要查询日期的年月日" << endl;
		cin >> year >> month >> day;
		goalDate.initDate(year, month, day);
		this->staff->checkDate(goalDate);
		staffFunction();
		break;
	case 3:
		cout << "请输入你的选择（1：增加；2：删除；3：修改）" << endl;
		cin >> choice;
		switch (choice) {
		case 1:
			staffAdd();
			staffFunction();
			break;
		case 2:
			cout << "请输入（1：只删除单件快递信息，2：删除掉所有已取快递的信息）" << endl;
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
		cout << "请输入取件码" << endl;
		cin >> pickupCode;
		if (this->staff->getPackage(pickupCode)) {
			cout << "取件成功" << endl;
		}
		else {
			cout << "取件失败" << endl;
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
	cout << "请输入你的手机号和密码" << endl;
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
			cout << "登录成功,欢迎你 " << row[0] << endl;
			addrresseeFunction();
		}
		else {
			cout << "登录失败，密码错误" << endl;
			initInterface();
		}
	}
	else {
		cout << "登录失败，你尚未注册过" << endl;
		initInterface();
	}

	return;
}

void System::addresseeRegist()
{
	cout << "请依次输入 姓名 手机号 收件地址 密码" << endl;
	string name, number, address, password;
	cin >> name >> number >> address >> password;
	Addressee addressee(name, number, address, password, Db);

	if (addressee.regist()) {
		cout << "注册成功，请在用户界面重新登录" << endl;	
	}
	else {
		cout << "注册失败，输入错误或已经拥有该用户" << endl;
	}
	addresseeInterface();
}

void System::staffAdd()
{
	cout << "请输入你要录入快件的信息（收件人电话，寄件人姓名，寄件人电话，寄件人地址，快递单号，快递公司，重量，邮编，取件码）" << endl;
	cout << "注意：收件人电话 必须是已经在本驿站注册过的电话才能录入，否则录入失败" << endl;
	string addresseeNumber, senderName, senderNumber, senderAddress, trackingNumber, courierCompany, zipCode,pickupCode;
	int weight;
	cin >> addresseeNumber>> senderName >> senderNumber >> senderAddress >> trackingNumber >> courierCompany >> weight >> zipCode>>pickupCode;
	if (this->staff->addPackage(addresseeNumber, senderName, senderNumber, senderAddress, trackingNumber, courierCompany, weight, zipCode,pickupCode)) {
		cout << "录入成功" << endl;
	}
	else {
		cout << "录入失败，收件人尚未注册过 或者 快递单号重复" << endl;
	}
	staffFunction();
}

void System::staffAlter()
{
	cout << "请输入快递单号" << endl;
	string trackingNumber;
	cin >> trackingNumber;

	cout << "请输入你要修改的信息类别（1、寄件人信息；2、收件人信息；3、快件信息）" << endl;
	int choice,type;
	cin >> type;
	switch (type) {
	case 1:
		cout << "请输入你要修改的信息(1：寄件人姓名，2：寄件人电话，3：寄件人地址)" << endl;
		break;
	case 2:
		cout << "请输入你要修改的信息(1：收件人电话)" << endl;
		break;
	case 3:
		cout << "请输入你要修改的信息(1：快递公司，2：快递重量，3：邮编)" << endl;
		break;
	default:
		break;
	}
	cin >> choice;
	cout << "请输入修改后的信息" << endl;
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
		cout << "修改成功" << endl;
	}
	else {
		cout << "修改失败" << endl;
	}
	staffFunction();
}


void System::staffDelete()
{
	string trickingNumber;
	cout << "输入你要删除的快递单号" << endl;
	cin >> trickingNumber;
	if (this->staff->dropPackage(trickingNumber)) {
		cout << "删除成功" << endl;
	}
	else {
		cout << "删除失败" << endl;
	}
}

void System::SetAddressee(Addressee& addreessee)
{
	this->addressee = &addreessee;
}

void System::staffLogin()
{
	cout << "请输入密码，输错自动返回" << endl;
	cout << "提示：员工初始密码为123" << endl;
	string password;
	cin >> password;
	if (staff->identify_staff(password)){
		staffFunction();
	}
	else {
		initInterface();
	}
}
