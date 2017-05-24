#include"API_Module.h"
#include"Catalog_Manager.h"
#include"Record_Manager.h"
#include"Index_Manager.h"

//�ͷ����ݿ���ڴ滺����
void Close_Database(CString DB_Name, bool closetype)
{
	printf("call Record_Manager->Close_Database(...);\n");
	printf("�ͷ����ݿ���ڴ滺����\n");
}

//�ͷű���������ڴ滺����
void Close_File(CString DB_Name, CString filename, int filetype, bool closetype)
{
	printf("call Record_Manager->Close_File(...);\n");
	printf("�ͷű���������ڴ滺����\n");
}

//����в���Ԫ��
void Insert_Item(CString DB_Name, CString Table_Name, CString Attr, int & record_Num)
{
	printf("call Record_Manager->Insert_Item(...);\n");
	printf("����в���Ԫ��\n");
}

//��ӡѡ������
void Print_Head(attr_info print[32], int count)
{
	printf("call Record_Manager->Print_Head(...);\n");
	printf("��ӡѡ������\n");
}

//��ӡ��ѡ����
void Print_To_Screen(CString record, attr_info print[32], int count)
{
	printf("call Record_Manager->Print_To_Screen(...);\n");
	printf("��ӡ��ѡ����\n");
}

//��where�����޿���������select���
void Select_Without_Useful_Cond(CString DB_Name, CString Table_Name, condition_info conds[10], int count, attr_info print[32], int Count, char cond)
{
	printf("call Record_Manager->Select_Without_Useful_Cond(...);\n");
	printf("��where�����޿���������select���\n");
}
//��where���п���������select ���
void Select_Without_Useful_Cond(CString DB_Name, CString Table_Name, condition_info conds[10], int count, attr_info print[32], int Count, char cond, int index)
{
	printf("call Record_Manager->Select_Without_Useful_Cond(...);\n");
	printf("��where���п���������select ���\n");
}

//�õ���������ѡ�����
void Select_With_Equal_Cond(CString DB_Name, CString Table_Name, condition_info conds[32], int count, attr_info print[32], int Count, char cond, index_info Index)
{
	printf("call Record_Manager->Select_With_Equal_Cond(...);\n");
	printf("�õ���������ѡ�����\n");
}

//�ô��ڻ���ڵ���������ѡ�����
void Select_With_Greater_Cond(CString DB_Name, CString Table_Name, condition_info conds[32], int count, attr_info print[32], int Count, char cond, index_info Index, int type)
{
	printf("call Record_Manager->Select_With_Greater_Cond(...);\n");
	printf("�ô��ڻ���ڵ���������ѡ�����\n");
}

//��С�ڻ�С�ڵ���������ѡ�����
void Select_With_Smaller_Cond(CString DB_Name, CString Table_Name, condition_info conds[32], int count, attr_info print[32], int Count, char cond, index_info Index, int type)
{
	printf("call Record_Manager->Select_With_Smaller_Cond(...);\n");
	printf("��С�ڻ�С�ڵ���������ѡ�����\n");
}

//�ж��Ƿ����where������
bool Confirm_To_Where(CString record, condition_info conds[10], int count, char cond)
{
	printf("call Record_Manager->Confirm_To_Where(...);\n");
	printf("�ж��Ƿ����where������\n");
}

//�ж��Ƿ����ĳ��ָ������
bool Confirm(CString record, condition_info condition)
{
	printf("call Record_Manager->Confirm(...);\n");
	printf("�ж��Ƿ����ĳ��ָ������\n");
}

//ѡ����䣨��where��
void Select_No_Where(CString DB_Name, CString Table_Name, attr_info print[32], int count)
{
	printf("call Record_Manager->Select_No_Where(...);\n");
	printf("ѡ����䣨��where��\n");
}

//ѡ����䣨��where��
void Select_With_Where(CString DB_Name, CString Table_Name, condition_info conds[10], int count, char cond, attr_info print[32], int Count)
{
	printf("call Record_Manager->Select_With_Where(...);\n");
	printf("ѡ����䣨��where��\n");
}

//ɾ����䣨��where��
void Delete_With_Where(CString DB_Name, CString Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond)
{
	printf("call Record_Manager->Delete_With_Where(...);\n");
	printf("ɾ����䣨��where��\n");
}

//��where, ���޿���������delete���
void Delete_Without_Useful_Cond(CString DB_Name, CString Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond)
{
	printf("call Record_Manager->Delete_Without_Useful_Cond(...);\n");
	printf("��where, ���޿���������delete���\n");
}

//��where���п���������delete ���
void Delete_With_Useful_Cond(CString DB_Name, CString Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond, int index)
{
	printf("call Record_Manager->Delete_With_Useful_Cond(...);\n");
	printf("��where���п���������delete ���\n");
}

//�õ���������ɾ�����
void Delete_With_Equal_Cond(CString DB_Name, CString Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond, index_info Index)
{
	printf("call Record_Manager->Delete_With_Equal_Cond(...);\n");
	printf("�õ���������ɾ�����\n");
}

//�ô��ڻ���ڵ���������ɾ�����
void Delete_With_Greater_Cond(CString DB_Name, CString Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond, index_info Index, int type)
{
	printf("call Record_Manager->Delete_With_Greater_Cond(...);\n");
	printf("�ô��ڻ���ڵ���������ɾ�����\n");
}

//��С�ڻ�С�ڵ���������ɾ�����
void Select_With_Smaller_Cond(CString DB_Name, CString Table_Name, condition_info conds[32], int count, index_info nodes[32], int num, char cond, index_info Index, int type)
{
	printf("call Record_Manager->Select_With_Smaller_Cond(...);\n");
	printf("��С�ڻ�С�ڵ���������ɾ�����\n");
}

//�������޿�������
bool Find_Useful_Cond(condition_info conds[10], int count, int & index)
{
	printf("call Record_Manager->Find_Useful_Cond(...);\n");
	printf("�������޿�������\n");
}

//�������������ڵ�
void Insert_Index_All(CString DB_Name, CString Table_Name, CString Index_Name, int length, int offset, int type)
{
	printf("call Record_Manager->Insert_Index_All(...);\n");
	printf("�������������ڵ�\n");
}

//��֤�ɷ����
bool Verify_Insertable(CString DB_Name, CString Table_Name, index_info nodes[32], int count, CString Attr)
{
	printf("call Record_Manager->Verify_Insertable(...);\n");
	printf("��֤�ɷ����\n");
}

//�˳�����
void Quit(CString DB_Name)
{
	printf("call Record_Manager->Quit(...);\n");
	printf("�˳�����\n");
}
