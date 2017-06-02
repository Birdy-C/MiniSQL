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
			if (CreateDirectory(fpath + "\\catalog", NULL)&& CreateDirectory(fpath + "\\table", NULL)&& CreateDirectory(fpath + "\\index", NULL)) {
				ofstream database_info;//创建database_info文件
				database_info.open(fpath + "\\catalog\\database_info.txt");
				database_info << 0;//初始表数为0
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
//现在我还没搞懂输入的那俩参数是啥……先这样放着吧……癌……
void Create_Table(CString Table_Name, CString Attr, CString DB_Name)
{
	fstream table_file;
	CString fpath = "..\\" + DB_Name;
	if (PathIsDirectory(fpath))//数据库存在
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
//创建索引
void Create_Index(CString Index_Name, CString Table_Name, CString Attr_Name, CString DB_Name, int & length, int & offset, int & type)
{

}


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

//转换条件组
bool Get_Condition(CString DB_Name, CString Table_Name, CString Condition, condition_info conds[32], int & count, char & cond, index_info nodes[32], int & num)
{

}

//获取显示记录格式
void Get_Attr_Info(CString DB_Name, CString Table_Name, attr_info print[32], int & count, CString Attr)
{

}

//获取所有属性的信息
void Get_Attr_Info_All(CString DB_Name, CString Table_Name, attr_info print[32], int & count)
{

}

//获取表中所有的索引信息
void Get_All_Index(CFile & file, long header, int attrs, index_info nodes[32], int & num)
{

}

//使用数据库
CString Use_Database(CString DB_Name)
{

}

//执行指定文件
void Exect_File(CString File)
{

}
*/
