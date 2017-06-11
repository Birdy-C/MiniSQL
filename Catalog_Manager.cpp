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
				if (CreateDirectory(fpath + "\\catalog\\table_num", NULL) && CreateDirectory(fpath + "\\catalog\\table_info", NULL) && CreateDirectory(fpath + "\\catalog\\index_info", NULL))
				{
					ofstream table_num;//����table_num�ļ�
					table_num.open(fpath + "\\catalog\\table_num\\table_num.txt");
					table_num << 0;//��ʼ����Ϊ0
					table_num.close();

					/*ofstream table_attr;
					table_attr.open(fpath + "\\catalog\\table_attr\\table_attr.txt");
					table_attr.close();*/
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
				temp += " NULL";
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
		/*	CFile table_attr;
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
*/
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
//�Ƿ��Ѿ�������
//Ȼ���ǲ���unique
//�޸�table_info�ļ���������
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
				int flag = 0,line_offset = 0;
				while(table_file.ReadString(strLine))
				{
					index = strLine.Find(' ');
					temp_attr = strLine.Left(index);//��ȡÿһ�е�����
					if (temp_attr == Attr_Name)
					{
						flag = 1;//�ҵ��˶�Ӧ����
						break;
					}
					line_offset += strLine.GetLength();
					strLine = _T("");
				}
				if (flag)//����������
				{
					CString whether_exist_index;
					int last_end;
					last_end = strLine.ReverseFind(' ');
					whether_exist_index = strLine.Mid(last_end + 1, strLine.GetLength() - last_end - 1);//���һ���ո��Ĵ�
					if (whether_exist_index == "NULL")//����������
					{
						CString whether_unique;
						strLine = strLine.Left(last_end);
						whether_unique = strLine.Right(1);
						if (whether_unique == '1')//����unique,���ڵ��������ܽ���
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
							//�ȸ�table_info,��һ�к�߰�NULL�ĳ������������ˡ���
							CString NewLines[33];//���32������+һ����������
							CString line = "\r\n";
							NewLines[0] = strLine+' ' + Index_Name;//��һ����Ҫ�ĵ��Ǹ�
							//������ȰѺ�ߵĶ���Ҳȫ������
							int i = 1;
							while (table_file.ReadString(strLine))
							{
								NewLines[i++] = strLine;
							}

							
							i = 0;//��ȫд��ȥ
							table_file.Seek(line_offset, CFile::begin);
							while (!NewLines[i].IsEmpty()) 
							{
								table_file.Write(NewLines[i].GetBuffer(), NewLines[i].GetLength());//������ôд�����������ԭ���ĸ����ǵ�����Ҫô�Ͱ���֮��Ķ���Ҳ����������Ȼ�����д��ȥ
								table_file.Write(line.GetBuffer(), line.GetLength());//����
								i++;
							}

							//Ȼ���ٸ���index_info����Ϣ
							CString index_info = "..\\" + DB_Name + "\\catalog\\index_info\\" + Index_Name + ".txt";
							CFile index_info_file;
							index_info_file.Open(index_info, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
							CString temp = Table_Name+' '+Attr_Name;
							//index_info_file.SeekToEnd();
							index_info_file.Write(temp.GetBuffer(), temp.GetLength());
							index_info_file.Close();

							cout << "Create index successfully!" << endl;
						}
						else//���Է�unique
						{
							cout << "Can't create index on a non-unique attribute!" << endl;
						}
					}
					else//�����Ե������Ѿ�����
					{
						cout << "There is already an index on this attribute!" << endl;
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

//ɾ����
//������û�������
//Ȼ�������������е�����ɾ��
//Ȼ���޸�table_info table_attr table_num�ɡ�����
void Drop_Table(CString Table_Name, CString DB_Name, CString index_name[32], int & count)
{
	CString table_info_path = "..\\" + DB_Name + "\\catalog\\table_info\\" + Table_Name + ".txt";
	CStdioFile table_info;
	try
	{
		if (table_info.Open(table_info_path, CFile::modeRead))//�������
		{
			CString strLine,temp;
			int index_end;//i��index_name������±�
			count = 0;
			CString index_info_path;
			CFile index_info_file;
			while (table_info.ReadString(strLine))
			{
				index_end = strLine.ReverseFind(' ');
				temp = strLine.Right(strLine.GetLength() - index_end-1);
				if (temp != "NULL")//�����Դ���������ȥindex_info�ļ�����Ѷ�Ӧ�ļ�ɾ��
				{
					index_name[count++] = temp;
					index_info_path = "..\\" + DB_Name + "\\catalog\\index_info\\" + temp + ".txt";
					index_info_file.Remove(index_info_path);

				}
				
			}
			table_info.Close();
			table_info.Remove(table_info_path);//ɾ��table_info����ļ�

			//Ȼ����Ǹ�table_attr��table_num
			//table_num�Ƚ����ף������ɡ���
			CString table_num_path = "..\\" + DB_Name + "\\catalog\\table_num\\table_num.txt";
			CStdioFile table_num_file;
			table_num_file.Open(table_num_path, CFile::modeReadWrite);

			int table_num;
			if (table_num_file.ReadString(strLine))
			{
				table_num = _ttoi(strLine);
				table_num--;
			}
			CString strNewLine;
			strNewLine.Format("%d", table_num);
			table_num_file.SetLength(0);
			table_num_file.WriteString(strNewLine);
			table_num_file.Close();
			
			//ͻȻ��ʼ˼��table_attr�����Ϣ�����𡭡����������˼
			//�ȷ��ţ��Ժ��õ��ٻ����������

			wcout << "Drop table " << Table_Name.GetString() << " successfully!" << endl;
		}
		else//û�������
		{
			wcout << "No such a table named " <<Table_Name.GetString() << endl;
		}
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
}

//ɾ������
//�ȿ������Ƿ����
//Ȼ��index_info�ļ������ҵ��������ű��ϵ��ĸ������ϣ�˳��ɾ��index_info�ļ�
//Ȼ��table_info�ļ��еĶ�Ӧ�ļ������һ��
void Drop_Index(CString Index_Name, CString DB_Name)
{
	CString index_info_path = "..\\" + DB_Name + "\\catalog\\index_info\\" + Index_Name + ".txt";
	CStdioFile index_info;
	try {
		if (index_info.Open(index_info_path, CFile::modeRead))//����������ֵ��ļ�
		{
			CString temp;
			CString table_name, attr_name;
			int end;
			index_info.ReadString(temp);
			end = temp.Find(' ');
			table_name = temp.Left(end);
			attr_name = temp.Right(temp.GetLength() - end - 1);//��ȡ������������
			index_info.Close();

			//�������ޱ���
			CString file_name = "..\\" + DB_Name + "\\catalog\\table_info\\" + table_name + ".txt";
			CStdioFile table_file;
			if (table_file.Open(file_name, CFile::modeReadWrite))//���ڸñ�
			{
				CString strLine, temp_name;
				int attr_name_index;
				int line_offset = 0;//��¼ƫ����
				while (table_file.ReadString(strLine))
				{
					attr_name_index = strLine.Find(' ');
					temp_name = strLine.Left(attr_name_index);

					if (temp_name == attr_name)//�ҵ���Ӧ��������
					{
						break;
					}
					line_offset += strLine.GetLength();
				}

				end = strLine.ReverseFind(' ');
				strLine = strLine.Left(end);
				//�޸ĸ����Զ�Ӧ�����е�������Ϣ
				CString NewLines[33];//���32������+һ����������
				CString line = "\r\n";
				NewLines[0] = strLine + ' ' + "NULL";//��һ����Ҫ�ĵ��Ǹ�
														 //������ȰѺ�ߵĶ���Ҳȫ������
				int i = 1;
				while (table_file.ReadString(strLine))
				{
					NewLines[i++] = strLine;
				}

				i = 0;//��ȫд��ȥ
				table_file.Seek(line_offset, CFile::begin);
				while (!NewLines[i].IsEmpty())
				{
					table_file.Write(NewLines[i].GetBuffer(), NewLines[i].GetLength());
					table_file.Write(line.GetBuffer(), line.GetLength());//����
					i++;
				}

				cout << "Drop index successfully!" << endl;
				table_file.Close();
				index_info.Remove(index_info_path);//ɾ�������ɹ�֮��ɾ���ֵ��ļ�
			}
			else
			{

				cout << "something wrong!" << endl;//Ӧ���ǲ��ᷢ������ġ���
			}


		}
		else
		{
			cout << "No such an index!" << endl;
		}
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}

}
/*
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
*/
void Get_each_condition(CString one_cond, condition_info conds[32], int& count)
{
	CString temp_attr, op, right_type, right_value;
	int op_start, op_end;

	op_end = one_cond.ReverseFind(' ');
	op_start = op_end - 2;
	op = one_cond.Mid(op_start, 2);
	temp_attr = one_cond.Left(op_start);//��ȡ������

	right_type = one_cond.GetAt(op_end + 1);//��ȡ�Ҳ�ֵ����������
	right_value = one_cond.Right(one_cond.GetLength() - op_end - 2);//��ȡ�Ҳ�ֵ
	//wcout << temp_attr.GetString() << " " << op.GetString() << " " << right_type.GetString() << " " << right_value.GetString() << endl;//������ȡ�Ƿ�ɹ���
	conds[count].left_attr = temp_attr;//�Ȱ���Щ��Ϣ�����������Բ�����˵����
	conds[count].condition = op;
	conds[count].const_data = right_value;
	conds[count].right_type = right_type;
	count++;
}
//ת��where������
//��һ���»��ǿ������Բ���
//Ȼ���ǲ���Ҫ��֤��Щ�����ǲ�����������
//���ԵĻ�Ӧ�þͿ��Դ������˰�
//��ػ�Ҫ��һ��������Ϣ���������....?��֪������
//1��ʾ��֤�ɹ���0��ʾ��֤ʧ�ܣ�
bool Get_Condition(CString DB_Name, CString Table_Name, CString Condition, condition_info conds[32], int & count, char & cond, index_info nodes[32], int & num)
{
	CString table_info_path = "..\\" + DB_Name + "\\catalog\\table_info\\" + Table_Name + ".txt";
	CStdioFile table_info;
	try
	{
		if (table_info.Open(table_info_path, CFile::modeRead))//�����ű�
		{
			table_info.Close();
			//��&��.Ϊ��ָ�Condition
	        int start = 0,end;
			CString one_cond;
			/*
			CString temp_attr,op,right_type,right_value;
			int op_start,op_end;*/
			count = 0;
			while ((end = Condition.Find('&', start)) != -1)//�ҵõ�&
			{
				one_cond = Condition.Mid(start, end - start);//��ȡ������һ��condition
				start = end + 1;
				Get_each_condition(one_cond, conds, count);
			}
			//Ȼ��Ҫ��ȡ���һ�����Ե���Ϣ
			end = Condition.GetLength() - 1;//Ҫ���Ե�����Ǹ�. ����Ҫ�����Ӹ����أ�������

			one_cond = Condition.Mid(start, end - start);//��ȡ������һ��condition
			start = end + 1;
			Get_each_condition(one_cond, conds, count);
		  
			//Ȼ��Ӧ����֤1.��������Ƿ���� 2.�������������Ƿ�һ��
			attr_info attrs[32];
			int attr_count;
			Get_Attr_Info_All(DB_Name, Table_Name, attrs, attr_count);//��table_info�����Ϣȫ����ȡ����
			
			int i,j;
			int flag;
	
			for (i = 0; i < count; i++)//����conds,�����֤����
			{
				flag = 0;
				for (j = 0; j < attr_count; j++)
				{
					//wcout << "conds[i]: " << conds[i].left_attr.GetString() << conds[i].right_type << endl;
					//wcout << "attrs[j]: " << attrs[j].attr_name.GetString() << attrs[j].type << endl;
					if (conds[i].left_attr == attrs[j].attr_name)//�ҵ��˶�Ӧ������
					{
						conds[i].left_type = attrs[j].type;
						flag = 1;
						if (conds[i].left_type == conds[i].right_type)
						{
							//��֤�ɹ���
							//�������ǲ���Ӧ������һ��conds,������Щ������û������ʲô��
							conds[i].left_index_name = attrs[j].index_name;
							//�ȿ���
							//cout << "��֤�ɹ���" << endl;
							flag = 2;
							break;

						}
						else
						{
							break;
						}
			
					}
					
				}
				if (flag == 0)
				{
					wcout << "There is no such an attribute named " << conds[i].left_attr.GetString() << " in the table!" << endl;
					break;
				}
				else if (flag == 1)
				{
					wcout << "The type of " << conds[i].const_data.GetString() << " is wrong!" << endl;
					break;
				}

			}

			//��ȡ������Ϣ?


			return true;
		}
		else//û�������
		{
			wcout << "No such a table named " << Table_Name.GetString() << endl;
			return false;
		}
		}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
}


//��֤insert����Ƿ���ȷ
//�ȿ�����
//����֤��1.���Ը����Ƿ�һ�� 2.�������Ե������Ƿ�һ��
bool verify_insert(CString DB_Name, CString Table_Name, CString Attr)
{
	CString table_info_path = "..\\" + DB_Name + "\\catalog\\table_info\\" + Table_Name + ".txt";
	CStdioFile table_info;
	try
	{
		if (table_info.Open(table_info_path, CFile::modeRead))//�����ű�
		{
			table_info.Close();
			attr_info print[32];
			int table_count;
			Get_Attr_Info_All(DB_Name, Table_Name, print, table_count);//�Ȼ�ȡcatalog�б����Ե���Ϣ
			//Ȼ���ȡ������Ϣ������Ƚ�
			CString temp;
			CString input_type;
			int input_count = 0;
			//int flag = 0;
			int start = 0, end;
			int length = Attr.GetLength();
			while ((start != length)&&((end = Attr.Find(',', start)) != -1))//����Ϊ�ֽ�
			{
			
				temp = Attr.Mid(start, end - start);
				input_type = temp.GetAt(1);//��ȡ�������������

				if (input_type != print[input_count].type)//�������Ͳ�һ�� //��֪���ܲ�����ô�Ƚ�
				{
					wcout << "The input data doesn't match the type of attribute " << print[input_count].attr_name.GetString() << " !" << endl;
					return false;
					//break;
				}


				start = end + 1;
				input_count++;
			}
			if (input_count != table_count)
			{
				cout << "The number of input data doesn't match!" << endl;
				return false;
			}
			else
			{
				return true;
			}

		}
		else//û�������
		{
			wcout << "No such a table named " << Table_Name.GetString() << endl;
			return false;
		}
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}
}

/*
//��ȡ��ʾ��¼��ʽ
void Get_Attr_Info(CString DB_Name, CString Table_Name, attr_info print[32], int & count, CString Attr)
{

}
*/
//��ȡһ�ű���������Ե���Ϣ
//�ȿ���û�����ű�
//�еĻ��Ϳ�ʼ����~
void Get_Attr_Info_All(CString DB_Name, CString Table_Name, attr_info print[32], int & count)
{
	CString table_info_path = "..\\" + DB_Name + "\\catalog\\table_info\\" + Table_Name + ".txt";
	CStdioFile table_info;
	try
	{
		if (table_info.Open(table_info_path, CFile::modeRead))//�����ű�
		{
			CString strLine;
			CString temp_type,temp_index_name;
			int attr_end,type_end,index_end;
			int length = 0;
			count = 0;
			while (table_info.ReadString(strLine))
			{
				if (strLine.Find('#') == -1)//������������в���
				{
					attr_end = strLine.Find(' ');
					print[count].attr_name = strLine.Left(attr_end);
					type_end = strLine.Find(' ',attr_end+1);
					temp_type = strLine.Mid(attr_end + 1, type_end - attr_end-1);

					index_end = strLine.ReverseFind(' ');//��¼������
					temp_index_name = strLine.Right(strLine.GetLength() - index_end-1);
					print[count].index_name = temp_index_name;
					
					if (temp_type == '+')//�������� int:+ float:- char:/
					{
						print[count].type = '+';
					}
					else if (temp_type == '-')
					{
						print[count].type = '-';
					}
					else
					{
						print[count].type = '/';
					}

				//	print[count].offset = length;//����offset�����м�¼��table_info�ļ��е�λ�ã�length������һ�еĳ���
				//	print[count].length = strLine.GetLength();
				//	length += strLine.GetLength();//��֪�������Բ��ԣ���ʱ����������

					count++;
				}
			}
			table_info.Close();
		}
		else//û�������
		{
			wcout << "No such a table named " << Table_Name.GetString() << endl;
		}
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}

}
/*
//��ȡ�������е�������Ϣ
void Get_All_Index(CFile & file, long header, int attrs, index_info nodes[32], int & num)
{

}

//ʹ�����ݿ�use
//����һ�����޸����ݿ�ͺ��ˡ���
CString Use_Database(CString DB_Name)
{

}

//ִ��ָ���ļ�execfile
void Exect_File(CString File)
{

}
*/
