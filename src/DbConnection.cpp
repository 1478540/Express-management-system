#include"DbConnection.h"
#include"DbException.h"
#include<iostream>
using namespace std;

DbConnection::DbConnection()
{
    //��ȡ��Ӧ�����ݿ���Ϣ
    getDbInformation();
    //��ʼ��MySQL���Ӷ���
    mysql_init(&connection);
    //ͨ���趨���ݿ�����������ַhost���˿ں�3306���û���username������passowrd�����ݿ���database�ȣ�������MySQL���ݿ�����ӣ�����ʧ���׳��쳣
    if (NULL == mysql_real_connect(&connection, host, username, password, database, 3306, nullptr, 0))
        throw DbExcepiton(mysql_error(&connection));
    //���ò�ѯ����Ϊgbk����֧������
    mysql_query(&connection, "set names gbk");
}

void DbConnection::getDbInformation()
{
    ifstream DbFile;
    DbFile.open("Database_Information.txt");
    if (!DbFile) {
        cout << "�ļ���ʧ��" << endl;
        exit(0);
    }
    DbFile.getline(host, 100, '\n');
    DbFile.getline(username, 100, '\n');
    DbFile.getline(password, 100, '\n');
    DbFile.getline(database, 100, '\n');
    DbFile.close();
}

DbConnection::~DbConnection() {
    mysql_close(&connection);
}

MYSQL_RES*  DbConnection::executeQuery(const char* sql)
{
    //ִ��select����
    int res = mysql_query(&connection, sql);

    if (res) //ִ��ʧ��
        throw DbExcepiton(mysql_error(&connection));
    else
    {
        //��ȡ�����
        MYSQL_RES* res_ptr = mysql_store_result(&connection);
        //�����ȡ�ɹ�
        if (res_ptr)
            return res_ptr;
        else throw DbExcepiton(mysql_error(&connection));
    }

}

int DbConnection::executeSQL(const char* sql)
{
    //ִ��insert��update��delete������
    int res = mysql_query(&connection, sql);

    if (res) //ִ��ʧ��
        throw DbExcepiton(mysql_error(&connection));
    else //ִ�гɹ������ز���Ӱ��ļ�¼��
    {
        return mysql_affected_rows(&connection);
    }
}
