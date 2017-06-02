#include"Catalog_Manager.h"
#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")
#include<fstream>


//�������ݿ�
//���ж����ݿ��Ƿ��Ѿ����ڣ�������ڷ��ش�����Ϣ
//�������򴴽����ݿ��ļ���,���ļ��У������ļ��к������ֵ��ļ�
void Create_Database(CString DB_Name)
{
	CString fpath = "..\\" + DB_Name;//��ǰĿ¼�½����ļ���
	if (!PathIsDirectory(fpath))
	{
		bool flag = CreateDirectory(fpath, NULL);//���ݿ��ļ���
		if (flag) {
			if (CreateDirectory(fpath + "\\catalog", NULL)&& CreateDirectory(fpath + "\\table", NULL)&& CreateDirectory(fpath + "\\index", NULL)) {
				ofstream database_info;//����database_info�ļ�
				database_info.open(fpath + "\\catalog\\database_info.txt");
				database_info << 0;//��ʼ����Ϊ0
				database_info.close();
				cout << "success!" << endl;
			}
			else
			{
				cout << "failed!" << endl;
			}
		}
		else
		{
			cout << "failed!" << endl;
		}
	}
	else//���ݿ��Ѵ���
	{
		cout << "Database already exists!" << endl;
	}
}

//������
//����֤��û��������ݿ⡭��
//Ȼ���жϱ���������������ʧ�ܣ����ش�����Ϣ
//û���򴴽��ñ��Ӧ���ļ�������catalog�е���Ӧ��Ϣ
//������������廹��Ҫ��������������
//�����һ�û�㶮���������������ɶ�������������Űɡ���������
void Create_Table(CString Table_Name, CString Attr, CString DB_Name)
{
	fstream table_file;
	CString fpath = "..\\" + DB_Name;
	if (PathIsDirectory(fpath))//���ݿ����
	{
		CString file_name = "..\\" + DB_Name + "\\table\\" + Table_Name + ".txt";
		table_file.open(file_name,iostream::in);
		if (!table_file)
		{
			ofstream fpath;
			fpath.open(file_name);
			fpath << (LPSTR)(LPCSTR)Attr;
			fpath.close();
			table_file.close();
			cout << "success!" << endl;
		}
		else
		{
			cout << "Table already exists!" << endl;
		}
	}
	else
	{
		cout << "No such a database!" << endl;
	}
}
/*
//��������
void Create_Index(CString Index_Name, CString Table_Name, CString Attr_Name, CString DB_Name, int & length, int & offset, int & type)
{

}


//���ַ���������Ϣ
CString add_info(CString info, int length, CString attr)
{

}

//��֤���������Ƿ���Ч
bool verify_attrs(CString attrs[32], int count)
{

}
*/

void DeleteDirectory(CString directory_path)   //ɾ��һ���ļ����µ���������  
{
	CFileFind finder;
	CString path = directory_path;
	path += _T("\\*.*");
	bool bWorking = finder.FindFile(path);
	while (bWorking) {
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots()) {//�����ļ���  
			DeleteDirectory(finder.GetFilePath()); //�ݹ�ɾ���ļ���  
			RemoveDirectory(finder.GetFilePath());
		}
		else {//�����ļ�  
			DeleteFile(finder.GetFilePath());
		}
	}
}

//ɾ�����ݿ�
//�Ȳ������޸����ݿ��Ӧ���ļ��У�������ɾ�������ļ��У����޷��ش�����Ϣ
void Drop_Database(CString DB_Name)
{
	CString fpath = "..\\" + DB_Name;//��ǰĿ¼�µ����ݿ��ļ���
	if (PathIsDirectory(fpath))
	{
		DeleteDirectory(fpath);		//�����ļ��У�ɾ�����������ļ�

		bool flag = RemoveDirectory(fpath);//ɾ���ļ���
		if (flag)
		{
			cout << "success!" << endl;
		}
		else
		{
			cout << "failed!" << endl;
		}
	}
	else//���ݿⲻ����
	{
		cout << "No such a database!" << endl;
	}
}
/*
//ɾ����
void Drop_Table(CString Table_Name, CString DB_Name, CString index_name[32], int & count)
{

}

//ɾ������
void Drop_Index(CString Index_Name, CString DB_Name)
{

}

//��֤����������ֵ
bool verify_add_attr(CString & attr, CString & temp, int type, int length)
{

}

//�ж��Ƿ�Ϊ���������
bool verify_attr_name(CString attr_name, CFile & file, long header, int attrs, long & offset, int & length, char & type, CString & Index_Name)
{

}

//��֤����ֵ�����Ч�Լ�������������
void Verify_Attr(CString DB_Name, CString Table_Name, CString & Attr, index_info nodes[32], int & count)
{

}

//ɾ����������Ԫ��
void Empty_Table(CString DB_Name, CString Table_Name, CString index_name[32], int & count)
{

}

//ת��������
bool Get_Condition(CString DB_Name, CString Table_Name, CString Condition, condition_info conds[32], int & count, char & cond, index_info nodes[32], int & num)
{

}

//��ȡ��ʾ��¼��ʽ
void Get_Attr_Info(CString DB_Name, CString Table_Name, attr_info print[32], int & count, CString Attr)
{

}

//��ȡ�������Ե���Ϣ
void Get_Attr_Info_All(CString DB_Name, CString Table_Name, attr_info print[32], int & count)
{

}

//��ȡ�������е�������Ϣ
void Get_All_Index(CFile & file, long header, int attrs, index_info nodes[32], int & num)
{

}

//ʹ�����ݿ�
CString Use_Database(CString DB_Name)
{

}

//ִ��ָ���ļ�
void Exect_File(CString File)
{

}
*/
