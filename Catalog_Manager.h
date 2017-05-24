#ifndef CATALOG_MANAGER_H
#define CATALOG_MANAGER_H

#include"MiniSQL.h"


//�������ݿ�
void Create_Database(CString DB_Name);

//������
void Create_Table(CString Table_Name, CString Attr, CString DB_Name, CString & Attr_Name);

//��������
void Create_Index(CString Index_Name, CString Table_Name, CString Attr_Name, CString DB_Name, int & length, int & offset, int & type);

//���ַ���������Ϣ
CString add_info(CString info, int length, CString attr);

//��֤���������Ƿ���Ч
bool verify_attrs(CString attrs[32], int count);

//ɾ�����ݿ�
void Drop_Database(CString DB_Name);

//ɾ����
void Drop_Table(CString Table_Name, CString DB_Name, CString index_name[32], int & count);

//ɾ������
void Drop_Index(CString Index_Name, CString DB_Name);

//��֤����������ֵ
bool verify_add_attr(CString & attr, CString & temp, int type, int length);

//�ж��Ƿ�Ϊ���������
bool verify_attr_name(CString attr_name, CFile & file, long header, int attrs, long & offset, int & length, char & type, CString & Index_Name);

//��֤����ֵ�����Ч�Լ�������������
void Verify_Attr(CString DB_Name, CString Table_Name, CString & Attr, index_info nodes[32], int & count);

//ɾ����������Ԫ��
void Empty_Table(CString DB_Name, CString Table_Name, CString index_name[32], int & count);

//ת��������
bool Get_Condition(CString DB_Name, CString Table_Name, CString Condition, condition_info conds[32], int & count, char & cond, index_info nodes[32], int & num);

//��ȡ��ʾ��¼��ʽ
void Get_Attr_Info(CString DB_Name, CString Table_Name, attr_info print[32], int & count, CString Attr);

//��ȡ�������Ե���Ϣ
void Get_Attr_Info_All(CString DB_Name, CString Table_Name, attr_info print[32], int & count);

//��ȡ�������е�������Ϣ
void Get_All_Index(CFile & file, long header, int attrs, index_info nodes[32], int & num);

//ʹ�����ݿ�
CString Use_Database(CString DB_Name);

//ִ��ָ���ļ�
void Exect_File(CString File);

#endif // !CATALOG_MANAGER_H