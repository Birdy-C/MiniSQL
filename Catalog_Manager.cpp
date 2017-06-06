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
			if (CreateDirectory(fpath + "\\catalog", NULL) && CreateDirectory(fpath + "\\table", NULL) && CreateDirectory(fpath + "\\index", NULL)) 
			{
				if (CreateDirectory(fpath + "\\catalog\\table_num", NULL) && CreateDirectory(fpath + "\\catalog\\table_attr", NULL) && CreateDirectory(fpath + "\\catalog\\table_info", NULL)) 
				{
					ofstream table_num;//����table_num�ļ�
					table_num.open(fpath + "\\catalog\\table_num\\table_num.txt");
					table_num << 0;//��ʼ����Ϊ0
					table_num.close();

					ofstream table_attr;
					table_attr.open(fpath + "\\catalog\\table_attr\\table_attr.txt");
					table_attr.close();
					cout << "create database successfully!" << endl;
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
//attr���ڲ��������
//attr_name��������������еĻ�������Ҫ���⴫��ȥ
void Create_Table(CString Table_Name, CString Attr, CString DB_Name, CString & Attr_Name)
{
	CFile table_file;
	CString fpath = "..\\" + DB_Name;
	if (PathIsDirectory(fpath))//���ݿ����
	{
		CString file_name = "..\\" + DB_Name + "\\catalog\\table_info\\" + Table_Name + ".txt";
		//table_file.Open(file_name,CFile::modeRead);
		if (!table_file.Open(file_name, CFile::modeRead))//û�иñ��Ӧ���ļ����򴴽��ļ���������Ϣ
		{
			table_file.Abort();

			CFile fpath;
			fpath.Open(file_name, CFile::modeCreate | CFile::modeWrite);
			//�Ѹ��������������͵���Ϣ����catalog��table_info��
			
			CString temp;
			CString line = "\r\n";
			int start = 0, end, primary_key_end;
			int count = 0;//��¼������
			
			while ((end = Attr.Find(',', start)) != -1)//�ҵ�����
			{
				count++;
				temp = Attr.Mid(start, end - start);
				fpath.Write((unsigned char *)(temp.GetBuffer()),temp.GetLength());//�浽table_info����
				fpath.Write(line.GetBuffer(), line.GetLength());
				if (temp.Find('#') != -1)//����������
				{
					count--;
					primary_key_end = temp.Find(' ');
					Attr_Name = temp.Mid(0, primary_key_end);
				}
				start = end + 1;
			}
			fpath.Close();
			
			//����database��
			/*CString database_file_name = "..\\" + DB_Name + "\\catalog\\database_info.txt";
			CFile database_info;
			database_info.Open(database_file_name,CFile::modeReadWrite);
			int num_table;
			database_info >> num_table;
			cout << "current table num: "<<num_table<<endl;
			//Ҫ��database_info�ļ��еĵ�һ�и���
			num_table++;*/

			try //table_num�б�������1
			{
				CString database_file_name = "..\\" + DB_Name + "\\catalog\\table_num\\table_num.txt";
				CStdioFile database_info;
				database_info.Open(database_file_name, CFile::modeReadWrite);

				CString strLine(_T(""));
				int table_num;
				if (database_info.ReadString(strLine))
				{
					table_num = _ttoi(strLine);
					table_num++;
				}
				CString strNewLine;
				strNewLine.Format("%d", table_num);
				database_info.SetLength(0);
				database_info.WriteString(strNewLine);
				database_info.Close();
			}
			catch (CException* e)
			{
				e->ReportError();
				e->Delete();
			}
			//�޸�table_attr.txt����
			CFile table_attr;
			CString attr_path = "..\\" + DB_Name + "\\catalog\\table_attr\\table_attr.txt";
			table_attr.Open(attr_path, CFile::modeWrite);
			table_attr.SeekToEnd();
			
			CString attrToWrite;
			CString attr_count;
			attr_count.Format("%d", count);
			attrToWrite = Table_Name + " " + attr_count;
			table_attr.Write(attrToWrite.GetBuffer(), attrToWrite.GetLength());
			table_attr.Write(line.GetBuffer(), line.GetLength());//����

			table_attr.Close();

			cout << "create table successfully!" << endl;
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

//��������
//�Ȳ��ұ����Բ���
//Ȼ���������Բ���
//Ȼ���ǲ���unique
void Create_Index(CString Index_Name, CString Table_Name, CString Attr_Name, CString DB_Name, int & length, int & offset, int & type)
{
	CStdioFile table_file;
	CString fpath = "..\\" + DB_Name;
	if (PathIsDirectory(fpath))//���ݿ����
	{
		CString file_name = "..\\" + DB_Name + "\\catalog\\table_info\\" + Table_Name + ".txt";
		if (table_file.Open(file_name, CFile::modeReadWrite))//���ڸñ�
		{
			try //��ȡtable_info�ж�Ӧ���ÿһ�У��ҵ�Ҫ���������Ǹ�����
			{

				CString strLine(_T(""));
				CString temp_attr;
				int index;
				int flag = 0;
				while(table_file.ReadString(strLine))
				{
					index = strLine.Find(' ');
					temp_attr = strLine.Left(index);//��ȡÿһ�е�����
					if (temp_attr == Attr_Name)
					{
						flag = 1;//�ҵ��˶�Ӧ����
						break;
					}
					strLine = _T("");
				}
				if (flag)//����������
				{
					CString whether_unique;
					whether_unique = strLine.Right(1);//��ѽ�����е㲻�ԡ������һ������ֻ�ܽ�һ�������Ļ�����Ҫ����һ�������Ƿ���ڣ��������ҵ�table_info��ʽ��Ҫ���˰�������������˵
					if (whether_unique == '1')//����Ϊunique
					{
						CString attr_type;
						int type_end;
						type_end = strLine.Find(' ', index);
						attr_type = strLine.Mid(index + 1, type_end - index - 1);//��ȡ��ʾ�������͵ķ��� +:int -:float ����:char
						if (attr_type == '+')//int
						{
							type = 0;
							length = 5;
						}
						else if (attr_type == '-')//float
						{
							type = 1;
							length = 10;
						}
						else//char
						{
							type = 2;
							length = _ttoi(attr_type);
						}

						//��Ҫ�����Ӧ��catalog�ļ���Ϣ
					}
					else//���Է�unique
					{
						cout << "Can not create index on this attribute!" << endl;
					}
				}
				else//����������
				{
					cout << "No such an attribute!" << endl;
				}
	
				table_file.Close();
			}
			catch (CException* e)
			{
				e->ReportError();
				e->Delete();
			}
		}
		else//��������
		{
			cout << "No such a table!" << endl;
		}
	}
	else
	{
		cout << "No such a database!" << endl;
	}
}
/*

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

//ת��������delete
bool Get_Condition(CString DB_Name, CString Table_Name, CString Condition, condition_info conds[32], int & count, char & cond, index_info nodes[32], int & num)
{

}

//��ȡ��ʾ��¼��ʽ
void Get_Attr_Info(CString DB_Name, CString Table_Name, attr_info print[32], int & count, CString Attr)
{

}

//��ȡһ�ű���������Ե���Ϣ
void Get_Attr_Info_All(CString DB_Name, CString Table_Name, attr_info print[32], int & count)
{

}

//��ȡ�������е�������Ϣ
void Get_All_Index(CFile & file, long header, int attrs, index_info nodes[32], int & num)
{

}

//ʹ�����ݿ�use
CString Use_Database(CString DB_Name)
{

}

//ִ��ָ���ļ�execfile
void Exect_File(CString File)
{

}
*/
