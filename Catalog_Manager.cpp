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
				if (CreateDirectory(fpath + "\\catalog\\table_num", NULL) && CreateDirectory(fpath + "\\catalog\\table_info", NULL) && CreateDirectory(fpath + "\\catalog\\index_info", NULL))
				{
					ofstream table_num;//创建table_num文件
					table_num.open(fpath + "\\catalog\\table_num\\table_num.txt");
					table_num << 0;//初始表数为0
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
				temp += " NULL";
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
		/*	CFile table_attr;
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

//创建索引
//先查找表名对不对
//然后看属性名对不对
//是否已经有索引
//然后看是不是unique
//修改table_info文件的索引名
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
				int flag = 0,line_offset = 0;
				while(table_file.ReadString(strLine))
				{
					index = strLine.Find(' ');
					temp_attr = strLine.Left(index);//获取每一行的属性
					if (temp_attr == Attr_Name)
					{
						flag = 1;//找到了对应属性
						break;
					}
					line_offset += strLine.GetLength();
					strLine = _T("");
				}
				if (flag)//属性名存在
				{
					CString whether_exist_index;
					int last_end;
					last_end = strLine.ReverseFind(' ');
					whether_exist_index = strLine.Mid(last_end + 1, strLine.GetLength() - last_end - 1);//最后一个空格后的词
					if (whether_exist_index == "NULL")//索引不存在
					{
						CString whether_unique;
						strLine = strLine.Left(last_end);
						whether_unique = strLine.Right(1);
						if (whether_unique == '1')//属性unique,折腾到这终于能建了
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
							//先改table_info,那一行后边把NULL改成索引名就行了……
							CString NewLines[33];//最多32个属性+一个主键定义
							CString line = "\r\n";
							NewLines[0] = strLine+' ' + Index_Name;//第一行是要改的那个
							//这接着先把后边的东西也全读出来
							int i = 1;
							while (table_file.ReadString(strLine))
							{
								NewLines[i++] = strLine;
							}

							
							i = 0;//再全写回去
							table_file.Seek(line_offset, CFile::begin);
							while (!NewLines[i].IsEmpty()) 
							{
								table_file.Write(NewLines[i].GetBuffer(), NewLines[i].GetLength());//不能这么写……这样会把原来的给覆盖掉……要么就把它之后的东西也都读出来，然后接着写回去
								table_file.Write(line.GetBuffer(), line.GetLength());//换行
								i++;
							}

							//然后再更新index_info的信息
							CString index_info = "..\\" + DB_Name + "\\catalog\\index_info\\" + Index_Name + ".txt";
							CFile index_info_file;
							index_info_file.Open(index_info, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
							CString temp = Table_Name+' '+Attr_Name;
							//index_info_file.SeekToEnd();
							index_info_file.Write(temp.GetBuffer(), temp.GetLength());
							index_info_file.Close();

							cout << "Create index successfully!" << endl;
						}
						else//属性非unique
						{
							cout << "Can't create index on a non-unique attribute!" << endl;
						}
					}
					else//该属性的索引已经存在
					{
						cout << "There is already an index on this attribute!" << endl;
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

//删除表
//先找有没有这个表
//然后把这个表上所有的索引删掉
//然后修改table_info table_attr table_num吧……？
void Drop_Table(CString Table_Name, CString DB_Name, CString index_name[32], int & count)
{
	CString table_info_path = "..\\" + DB_Name + "\\catalog\\table_info\\" + Table_Name + ".txt";
	CStdioFile table_info;
	try
	{
		if (table_info.Open(table_info_path, CFile::modeRead))//有这个表
		{
			CString strLine,temp;
			int index_end;//i是index_name数组的下标
			count = 0;
			CString index_info_path;
			CFile index_info_file;
			while (table_info.ReadString(strLine))
			{
				index_end = strLine.ReverseFind(' ');
				temp = strLine.Right(strLine.GetLength() - index_end-1);
				if (temp != "NULL")//该属性存在索引，去index_info文件夹里把对应文件删掉
				{
					index_name[count++] = temp;
					index_info_path = "..\\" + DB_Name + "\\catalog\\index_info\\" + temp + ".txt";
					index_info_file.Remove(index_info_path);

				}
				
			}
			table_info.Close();
			table_info.Remove(table_info_path);//删掉table_info里的文件

			//然后就是改table_attr和table_num
			//table_num比较容易，先来吧……
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
			
			//突然开始思考table_attr这个信息有用吗…………陷入沉思
			//先放着，以后用到再回来填这个坑

			wcout << "Drop table " << Table_Name.GetString() << " successfully!" << endl;
		}
		else//没有这个表
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

//删除索引
//先看索引是否存在
//然后到index_info文件夹里找到它在哪张表上的哪个属性上，顺便删除index_info文件
//然后到table_info文件夹的对应文件里更新一下
void Drop_Index(CString Index_Name, CString DB_Name)
{
	CString index_info_path = "..\\" + DB_Name + "\\catalog\\index_info\\" + Index_Name + ".txt";
	CStdioFile index_info;
	try {
		if (index_info.Open(index_info_path, CFile::modeRead))//有这个索引字典文件
		{
			CString temp;
			CString table_name, attr_name;
			int end;
			index_info.ReadString(temp);
			end = temp.Find(' ');
			table_name = temp.Left(end);
			attr_name = temp.Right(temp.GetLength() - end - 1);//获取表名和属性名
			index_info.Close();

			//查找有无表名
			CString file_name = "..\\" + DB_Name + "\\catalog\\table_info\\" + table_name + ".txt";
			CStdioFile table_file;
			if (table_file.Open(file_name, CFile::modeReadWrite))//存在该表
			{
				CString strLine, temp_name;
				int attr_name_index;
				int line_offset = 0;//记录偏移量
				while (table_file.ReadString(strLine))
				{
					attr_name_index = strLine.Find(' ');
					temp_name = strLine.Left(attr_name_index);

					if (temp_name == attr_name)//找到对应的属性名
					{
						break;
					}
					line_offset += strLine.GetLength();
				}

				end = strLine.ReverseFind(' ');
				strLine = strLine.Left(end);
				//修改该属性对应的那行的索引信息
				CString NewLines[33];//最多32个属性+一个主键定义
				CString line = "\r\n";
				NewLines[0] = strLine + ' ' + "NULL";//第一行是要改的那个
														 //这接着先把后边的东西也全读出来
				int i = 1;
				while (table_file.ReadString(strLine))
				{
					NewLines[i++] = strLine;
				}

				i = 0;//再全写回去
				table_file.Seek(line_offset, CFile::begin);
				while (!NewLines[i].IsEmpty())
				{
					table_file.Write(NewLines[i].GetBuffer(), NewLines[i].GetLength());
					table_file.Write(line.GetBuffer(), line.GetLength());//换行
					i++;
				}

				cout << "Drop index successfully!" << endl;
				table_file.Close();
				index_info.Remove(index_info_path);//删除索引成功之后删除字典文件
			}
			else
			{

				cout << "something wrong!" << endl;//应该是不会发生这个的……
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
*/
void Get_each_condition(CString one_cond, condition_info conds[32], int& count)
{
	CString temp_attr, op, right_type, right_value;
	int op_start, op_end;

	op_end = one_cond.ReverseFind(' ');
	op_start = op_end - 2;
	op = one_cond.Mid(op_start, 2);
	temp_attr = one_cond.Left(op_start);//提取属性名

	right_type = one_cond.GetAt(op_end + 1);//获取右侧值的数据类型
	right_value = one_cond.Right(one_cond.GetLength() - op_end - 2);//获取右侧值
	//wcout << temp_attr.GetString() << " " << op.GetString() << " " << right_type.GetString() << " " << right_value.GetString() << endl;//测试提取是否成功用
	conds[count].left_attr = temp_attr;//先把这些信息都存起来，对不对再说……
	conds[count].condition = op;
	conds[count].const_data = right_value;
	conds[count].right_type = right_type;
	count++;
}
//转换where条件组
//第一件事还是看表名对不对
//然后是不是要验证那些属性是不是这个表里的
//都对的话应该就可以存起来了吧
//想必还要存一下索引信息留给后边用....?不知道……
//1表示验证成功，0表示验证失败？
bool Get_Condition(CString DB_Name, CString Table_Name, CString Condition, condition_info conds[32], int & count, char & cond, index_info nodes[32], int & num)
{
	CString table_info_path = "..\\" + DB_Name + "\\catalog\\table_info\\" + Table_Name + ".txt";
	CStdioFile table_info;
	try
	{
		if (table_info.Open(table_info_path, CFile::modeRead))//有这张表
		{
			table_info.Close();
			//以&和.为界分隔Condition
	        int start = 0,end;
			CString one_cond;
			/*
			CString temp_attr,op,right_type,right_value;
			int op_start,op_end;*/
			count = 0;
			while ((end = Condition.Find('&', start)) != -1)//找得到&
			{
				one_cond = Condition.Mid(start, end - start);//获取单独的一个condition
				start = end + 1;
				Get_each_condition(one_cond, conds, count);
			}
			//然后要获取最后一个属性的信息
			end = Condition.GetLength() - 1;//要忽略掉最后那个. 干嘛要在最后加个点呢，啧……

			one_cond = Condition.Mid(start, end - start);//获取单独的一个condition
			start = end + 1;
			Get_each_condition(one_cond, conds, count);
		  
			//然后应该验证1.左侧属性是否存在 2.左右属性类型是否一致
			attr_info attrs[32];
			int attr_count;
			Get_Attr_Info_All(DB_Name, Table_Name, attrs, attr_count);//把table_info里的信息全都获取出来
			
			int i,j;
			int flag;
	
			for (i = 0; i < count; i++)//遍历conds,逐个验证……
			{
				flag = 0;
				for (j = 0; j < attr_count; j++)
				{
					//wcout << "conds[i]: " << conds[i].left_attr.GetString() << conds[i].right_type << endl;
					//wcout << "attrs[j]: " << attrs[j].attr_name.GetString() << attrs[j].type << endl;
					if (conds[i].left_attr == attrs[j].attr_name)//找到了对应的属性
					{
						conds[i].left_type = attrs[j].type;
						flag = 1;
						if (conds[i].left_type == conds[i].right_type)
						{
							//验证成功了
							//接下来是不是应该完善一下conds,看看那些属性有没有索引什么的
							conds[i].left_index_name = attrs[j].index_name;
							//先空着
							//cout << "验证成功了" << endl;
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

			//获取索引信息?


			return true;
		}
		else//没有这个表
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


//验证insert语句是否正确
//先看表名
//再验证：1.属性个数是否一致 2.各个属性的类型是否一致
bool verify_insert(CString DB_Name, CString Table_Name, CString Attr)
{
	CString table_info_path = "..\\" + DB_Name + "\\catalog\\table_info\\" + Table_Name + ".txt";
	CStdioFile table_info;
	try
	{
		if (table_info.Open(table_info_path, CFile::modeRead))//有这张表
		{
			table_info.Close();
			attr_info print[32];
			int table_count;
			Get_Attr_Info_All(DB_Name, Table_Name, print, table_count);//先获取catalog中表属性的信息
			//然后读取输入信息，逐个比较
			CString temp;
			CString input_type;
			int input_count = 0;
			//int flag = 0;
			int start = 0, end;
			int length = Attr.GetLength();
			while ((start != length)&&((end = Attr.Find(',', start)) != -1))//逗号为分界
			{
			
				temp = Attr.Mid(start, end - start);
				input_type = temp.GetAt(1);//获取输入的数据类型

				if (input_type != print[input_count].type)//数据类型不一致 //不知道能不能这么比较
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
		else//没有这个表
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
//获取显示记录格式
void Get_Attr_Info(CString DB_Name, CString Table_Name, attr_info print[32], int & count, CString Attr)
{

}
*/
//获取一张表的所有属性的信息
//先看有没有这张表
//有的话就开始遍历~
void Get_Attr_Info_All(CString DB_Name, CString Table_Name, attr_info print[32], int & count)
{
	CString table_info_path = "..\\" + DB_Name + "\\catalog\\table_info\\" + Table_Name + ".txt";
	CStdioFile table_info;
	try
	{
		if (table_info.Open(table_info_path, CFile::modeRead))//有这张表
		{
			CString strLine;
			CString temp_type,temp_index_name;
			int attr_end,type_end,index_end;
			int length = 0;
			count = 0;
			while (table_info.ReadString(strLine))
			{
				if (strLine.Find('#') == -1)//主键定义的那行不算
				{
					attr_end = strLine.Find(' ');
					print[count].attr_name = strLine.Left(attr_end);
					type_end = strLine.Find(' ',attr_end+1);
					temp_type = strLine.Mid(attr_end + 1, type_end - attr_end-1);

					index_end = strLine.ReverseFind(' ');//记录索引名
					temp_index_name = strLine.Right(strLine.GetLength() - index_end-1);
					print[count].index_name = temp_index_name;
					
					if (temp_type == '+')//属性类型 int:+ float:- char:/
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

				//	print[count].offset = length;//这样offset是这行记录在table_info文件中的位置，length是它这一行的长度
				//	print[count].length = strLine.GetLength();
				//	length += strLine.GetLength();//不知道这样对不对，暂时先这样放着

					count++;
				}
			}
			table_info.Close();
		}
		else//没有这个表
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
//获取表中所有的索引信息
void Get_All_Index(CFile & file, long header, int attrs, index_info nodes[32], int & num)
{

}

//使用数据库use
//查找一下有无该数据库就好了……
CString Use_Database(CString DB_Name)
{

}

//执行指定文件execfile
void Exect_File(CString File)
{

}
*/
