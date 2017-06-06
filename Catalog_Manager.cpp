#include"Catalog_Manager.h"
#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")
#include<fstream>


//创建数据库
//先判断数据库是否已经存在，如果存在返回错误信息
//不存在则创建数据库文件夹,表文件夹，索引文件夹和数据字典文件
void Create_Database(CString DB_Name)
{
	CString fpath = "..\\" + DB_Name;//当前目录下建立文件夹
	if (!PathIsDirectory(fpath))
	{
		bool flag = CreateDirectory(fpath, NULL);//数据库文件夹
		if (flag) {
			if (CreateDirectory(fpath + "\\catalog", NULL) && CreateDirectory(fpath + "\\table", NULL) && CreateDirectory(fpath + "\\index", NULL)) 
			{
				if (CreateDirectory(fpath + "\\catalog\\table_num", NULL) && CreateDirectory(fpath + "\\catalog\\table_attr", NULL) && CreateDirectory(fpath + "\\catalog\\table_info", NULL)) 
				{
					ofstream table_num;//创建table_num文件
					table_num.open(fpath + "\\catalog\\table_num\\table_num.txt");
					table_num << 0;//初始表数为0
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
	else//数据库已存在
	{
		cout << "Database already exists!" << endl;
	}
}

//创建表
//先验证有没有这个数据库……
//然后判断表有无重名，有则失败，返回错误信息
//没有则创建该表对应的文件，更新catalog中的相应信息
//如果有主键定义还需要建立索引……？
//attr是内部解析语句
//attr_name是主键名（如果有的话），需要从这传回去
void Create_Table(CString Table_Name, CString Attr, CString DB_Name, CString & Attr_Name)
{
	CFile table_file;
	CString fpath = "..\\" + DB_Name;
	if (PathIsDirectory(fpath))//数据库存在
	{
		CString file_name = "..\\" + DB_Name + "\\catalog\\table_info\\" + Table_Name + ".txt";
		//table_file.Open(file_name,CFile::modeRead);
		if (!table_file.Open(file_name, CFile::modeRead))//没有该表对应的文件，则创建文件，输入信息
		{
			table_file.Abort();

			CFile fpath;
			fpath.Open(file_name, CFile::modeCreate | CFile::modeWrite);
			//把各个属性名和类型等信息存入catalog的table_info表
			
			CString temp;
			CString line = "\r\n";
			int start = 0, end, primary_key_end;
			int count = 0;//记录属性数
			
			while ((end = Attr.Find(',', start)) != -1)//找到逗号
			{
				count++;
				temp = Attr.Mid(start, end - start);
				fpath.Write((unsigned char *)(temp.GetBuffer()),temp.GetLength());//存到table_info表中
				fpath.Write(line.GetBuffer(), line.GetLength());
				if (temp.Find('#') != -1)//有主键定义
				{
					count--;
					primary_key_end = temp.Find(' ');
					Attr_Name = temp.Mid(0, primary_key_end);
				}
				start = end + 1;
			}
			fpath.Close();
			
			//更新database表
			/*CString database_file_name = "..\\" + DB_Name + "\\catalog\\database_info.txt";
			CFile database_info;
			database_info.Open(database_file_name,CFile::modeReadWrite);
			int num_table;
			database_info >> num_table;
			cout << "current table num: "<<num_table<<endl;
			//要把database_info文件中的第一行更新
			num_table++;*/

			try //table_num中表数增加1
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
			//修改table_attr.txt内容
			CFile table_attr;
			CString attr_path = "..\\" + DB_Name + "\\catalog\\table_attr\\table_attr.txt";
			table_attr.Open(attr_path, CFile::modeWrite);
			table_attr.SeekToEnd();
			
			CString attrToWrite;
			CString attr_count;
			attr_count.Format("%d", count);
			attrToWrite = Table_Name + " " + attr_count;
			table_attr.Write(attrToWrite.GetBuffer(), attrToWrite.GetLength());
			table_attr.Write(line.GetBuffer(), line.GetLength());//换行

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

//创建索引
//先查找表名对不对
//然后看属性名对不对
//然后看是不是unique
void Create_Index(CString Index_Name, CString Table_Name, CString Attr_Name, CString DB_Name, int & length, int & offset, int & type)
{
	CStdioFile table_file;
	CString fpath = "..\\" + DB_Name;
	if (PathIsDirectory(fpath))//数据库存在
	{
		CString file_name = "..\\" + DB_Name + "\\catalog\\table_info\\" + Table_Name + ".txt";
		if (table_file.Open(file_name, CFile::modeReadWrite))//存在该表
		{
			try //读取table_info中对应表的每一行，找到要建索引的那个属性
			{

				CString strLine(_T(""));
				CString temp_attr;
				int index;
				int flag = 0;
				while(table_file.ReadString(strLine))
				{
					index = strLine.Find(' ');
					temp_attr = strLine.Left(index);//获取每一行的属性
					if (temp_attr == Attr_Name)
					{
						flag = 1;//找到了对应属性
						break;
					}
					strLine = _T("");
				}
				if (flag)//属性名存在
				{
					CString whether_unique;
					whether_unique = strLine.Right(1);//嗨呀好像有点不对……如果一个属性只能建一次索引的话，还要考虑一下索引是否存在，那这样我的table_info格式就要改了癌癌癌明天起床再说
					if (whether_unique == '1')//属性为unique
					{
						CString attr_type;
						int type_end;
						type_end = strLine.Find(' ', index);
						attr_type = strLine.Mid(index + 1, type_end - index - 1);//获取表示属性类型的符号 +:int -:float 数字:char
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

						//还要添加相应的catalog文件信息
					}
					else//属性非unique
					{
						cout << "Can not create index on this attribute!" << endl;
					}
				}
				else//属性名错误
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
		else//表名错误
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

//向字符串增加信息
CString add_info(CString info, int length, CString attr)
{

}

//验证属性组名是否有效
bool verify_attrs(CString attrs[32], int count)
{

}
*/

void DeleteDirectory(CString directory_path)   //删除一个文件夹下的所有内容  
{
	CFileFind finder;
	CString path = directory_path;
	path += _T("\\*.*");
	bool bWorking = finder.FindFile(path);
	while (bWorking) {
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots()) {//处理文件夹  
			DeleteDirectory(finder.GetFilePath()); //递归删除文件夹  
			RemoveDirectory(finder.GetFilePath());
		}
		else {//处理文件  
			DeleteFile(finder.GetFilePath());
		}
	}
}

//删除数据库
//先查找有无该数据库对应的文件夹，若有则删除整个文件夹，若无返回错误信息
void Drop_Database(CString DB_Name)
{
	CString fpath = "..\\" + DB_Name;//当前目录下的数据库文件夹
	if (PathIsDirectory(fpath))
	{
		DeleteDirectory(fpath);		//遍历文件夹，删除其中所有文件

		bool flag = RemoveDirectory(fpath);//删除文件夹
		if (flag)
		{
			cout << "success!" << endl;
		}
		else
		{
			cout << "failed!" << endl;
		}
	}
	else//数据库不存在
	{
		cout << "No such a database!" << endl;
	}
}
/*
//删除表
void Drop_Table(CString Table_Name, CString DB_Name, CString index_name[32], int & count)
{

}

//删除索引
void Drop_Index(CString Index_Name, CString DB_Name)
{

}

//验证并增加属性值
bool verify_add_attr(CString & attr, CString & temp, int type, int length)
{

}

//判断是否为表的属性名
bool verify_attr_name(CString attr_name, CFile & file, long header, int attrs, long & offset, int & length, char & type, CString & Index_Name)
{

}

//验证属性值组的有效性及表中所有索引
void Verify_Attr(CString DB_Name, CString Table_Name, CString & Attr, index_info nodes[32], int & count)
{

}

//删除表内所有元组
void Empty_Table(CString DB_Name, CString Table_Name, CString index_name[32], int & count)
{

}

//转换条件组delete
bool Get_Condition(CString DB_Name, CString Table_Name, CString Condition, condition_info conds[32], int & count, char & cond, index_info nodes[32], int & num)
{

}

//获取显示记录格式
void Get_Attr_Info(CString DB_Name, CString Table_Name, attr_info print[32], int & count, CString Attr)
{

}

//获取一张表的所有属性的信息
void Get_Attr_Info_All(CString DB_Name, CString Table_Name, attr_info print[32], int & count)
{

}

//获取表中所有的索引信息
void Get_All_Index(CFile & file, long header, int attrs, index_info nodes[32], int & num)
{

}

//使用数据库use
CString Use_Database(CString DB_Name)
{

}

//执行指定文件execfile
void Exect_File(CString File)
{

}
*/
