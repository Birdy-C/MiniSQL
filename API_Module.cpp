#include"API_Module.h"
#include"Catalog_Manager.h"
#include"Record_Manager.h"
#include"Index_Manager.h"

using namespace std;

CString DB_Name;
/*
根据Interpreter层解释生成的命令内部形式的语句编号分别调用Catalog Manager提供的接口进行语句的语义验证，
并根据结果调用Record Manager,Index Manager提供的接口执行各语句，
同时也为Record Manager 模块调用Index Manager模块的功能提供接口。
概而言之，API是Interpreter模块，Record Manager模块及Catalog Manager模块进行沟通的枢纽，
三者之间的交互一般都需由API模块来进行转接才能进行相互之间的功能调用。
因此，虽然API模块所实现的功能十分有限，但是该模块却是整个系统的核心。
*/
/*
通过API模块的功能，使得Interpreter模块，Catalog Manager模块，Record Manager模块和Index Manager模块之间实现了相互通信，
为模块之间的协调工作提供了有效的接口。
*/
/*
CString Left( int nCount ) const;
从左取字串

例：
csStr="abcdef";
cout<<csStr.Left(3);
//abc
//当nCount等于0时，返回空。
//当nCount为负数时，返回空。
//当nCount大于对象长度时，返回值与对象相同。


CString Right( int nCount ) const;
从右取字串

例：
csStr="abcdef";
cout<<csStr.Right(3);
//def
//当nCount等于0时，返回空。
//当nCount为负数时，返回空。
//当nCount大于对象长度时，返回值与对象相同。
*/
void API_Module(CString SQL)
{
	CString Type, Attr, Index_Name, Table_Name, Attr_Name, Condition, index_name[32], File;
	int index, end, length, offset, type, count, num, record_Num, Count;
	//十分想知道这里的count是什么作用，原始程序里后面会用到但是是直接用的？
	//也许是属性的个数？但还有一个Count，可能是where条件的个数？此处待解决
	//这里的一大批变量都没有进行过初始化？
	//此处待解决
	index_info nodes[32];
	condition_info conds[10];
	attr_info print[32];
	char cond;
	bool ok;
	Type = SQL.Left(2);//Type是后面的暗号
	SQL = SQL.Mid(2, SQL.GetLength() - 2);//从这里开始SQL不包含暗号了

										  /////////////////////////////////////////////////////////////////////////////////////////
										  //创建数据库
	if (Type == "00")
		Create_Database(SQL);//此处是调用Catalog_Manager里的Create_Database()函数

							 /////////////////////////////////////////////////////////////////////////////////////////
							 //创建数据表Create_Table
	else if (Type == "01")
	{
		if (DB_Name.IsEmpty())
			cout << "error: you have not specified the database to be used!" << endl;
		else
		{
			index = SQL.Find(',');
			Table_Name = SQL.Left(index);
			Attr = SQL.Right(SQL.GetLength() - index - 1);//此处的Attr依然还是一长串的属性信息
			Attr_Name.Empty();
			//创建数据表
			Create_Table(Table_Name, Attr, DB_Name, Attr_Name);//此处是调用Catalog_Manager里的Create_Table()函数
															   //判断是否创建主键索引
			if (!Attr_Name.IsEmpty())
				Create_Index(Table_Name, Table_Name, Attr_Name, DB_Name, length, offset, type); //此处是调用Catalog_Manager里的Create_Index()函数
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//创建索引Create_Index
	else if (Type == "02")
	{
		if (DB_Name.IsEmpty())
			cout << "error: you have not specified the database to be used!" << endl;
		else
		{
			index = SQL.Find(' ');
			//获取索引名
			Index_Name = SQL.Left(index);
			index++;
			end = SQL.Find(' ', index);
			//获取表名
			Table_Name = SQL.Mid(index, end - index);
			//获取属性名
			Attr = SQL.Right(SQL.GetLength() - end - 1);
			Create_Index(Index_Name, Table_Name, Attr, DB_Name, length, offset, type);//此处是调用Catalog_Manager里的Create_Index()函数
																					  //插入所有索引节点
			if (length != -1)
				Insert_Index_All(DB_Name, Table_Name, Index_Name, length, offset, type);//此处是调用Catalog_Manager里的Insert_Index_All()函数
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//删除数据库
	else if (Type == "10")
	{
		if (SQL == DB_Name)
		{
			//释放数据库的内存
			Close_Database(DB_Name, false);//此处是调用Record_Manager里的Close_Database()函数
										   //Buffer_Manager里的类似函数是小写的形式
			DB_Name.Empty();
		}
		Drop_Database(SQL);//此处是调用Catalog_Manager里的Drop_Database()函数
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//删除数据表
	else if (Type == "11")
	{
		if (DB_Name.IsEmpty())
			cout << "error: you have not specified the database to be used!" << endl;
		else
		{
			Table_Name = SQL;
			//释放表的内存
			Close_File(DB_Name, Table_Name, 0, false);//此处是调用Record_Manager里的Close_File()函数
													  //删除表文件
			Drop_Table(Table_Name, DB_Name, index_name, count);//此处是调用Catalog_Manager里的Drop_Table()函数
															   //释放表中所有索引的内存
			for (index = 0; index < count; index++)
				Close_File(DB_Name, index_name[index], 1, false);//此处是调用Record_Manager里的Close_File()函数
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//删除索引
	else if (Type == "12")
	{
		if (DB_Name.IsEmpty())
			cout << "error: you have not specified the database to be used!" << endl;
		else
		{
			Index_Name = SQL;
			//删除索引的内存
			Close_File(DB_Name, Index_Name, 1, false);//此处是调用Record_Manager里的Close_File()函数
			Drop_Index(Index_Name, DB_Name);//此处是调用Catalog_Manager里的Drop_Index()函数
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//选择语句(无where子句)
	//上面这句是原始程序里的，可能原始程序时根据有无where细分的Type暗号
	//但根据已有的Interpreter应该不是这样来分
	//而是Type为20代表select * 命令，
	//其中有无where条件可以通过判断结尾是不是.#来在相应的Type情况下再细分
	//目前我想到的判断字符方法就是用SQL.Right(2)=='.#'?
	//但如果这个命令里所包含的where条件里刚好也是以'.#'结尾的可咋整？ 此处待解决

	/////////////////////////////////////////////////////////////////////////////////////////
	/*
	select * :"20." + "表名1,表名2,...,表名n." + "#或where条件"
	select(属性名) : "21,属性名1，属性名2，...,属性名n." + "表名1,表名2,...,表名n." + "#或where条件"
	#表示该select语句中不含where语句
	where条件：
	&表示and 六种运算符：
	两位的运算符(>= <= <>)就是它们本身，一位的运算符（< > = ）是一个空格加上它们本身
	*/
	/*
	选择语句
	SELECT * FROM 表名；   SELECT 列1，列2，….列n FROM 表名；
	无WHERE条件，则须对表文件进行遍历，并对每条记录，根据选择属性名组，打印出对应的属性值。

	SELECT * FROM 表名 WHERE 条件； SELECT 列1，列2，…列n FROM 表名 WHERE 条件
	若无可用的索引，则须对表文件进行遍历，并对每条记录，进行WHERE 条件匹配，若符合，则打印相应属性的值。
	若有可用的索引，则可调用Index Manater模块的功能，查找符合WHERE 条件的记录，并打印相应属性的值。在最后打印出被选择的记录的条数。
	*/


	//选择语句（select * 形式）
	else if (Type == "20")
	{
		if (DB_Name.IsEmpty())
			cout << "error: you have not specified the database to be used!" << endl;
		else if (SQL.Right(2) == '.#')//选择语句（select * 形式，无where子句）
		{
			/*此处为原始程序中的代码，供参考
			//选择语句(无where子句)
			//获取选择属性组
			index = SQL.Find(',');
			Attr = SQL.Left(index);
			index++;
			//获取表名
			Table_Name = SQL.Right(SQL.GetLength() - index);
			Table_Name = Table_Name.Left(Table_Name.GetLength() - 1);
			if (Table_Name.Find(' ') != -1)
			cout << "error: can not select from more than one table!" << endl;
			else
			{
			//获取显示记录格式
			if (Attr == "*")
			Get_Attr_Info_All(DB_Name, Table_Name, print, count);
			else
			Get_Attr_Info(DB_Name, Table_Name, print, count, Attr);//select条件
			if (count != 0)
			Select_No_Where(DB_Name, Table_Name, print, count);
			}
			*/
			index = SQL.Find('.');//表名结束了
			Table_Name = SQL.Left(index);//这里是一长串表名，是要全做笛卡尔积起来吗。。。此处待解决
										 //好的不用的，因为不要求多个表，已解决
			if (Table_Name.Find(',') != -1)
				cout << "erroe: can not select from more than one table!" << endl;
			else
			{
				//对表文件进行遍历，并对每条记录，根据选择属性名组，打印出对应的属性值。
				Get_Attr_Info_All(DB_Name, Table_Name, print, count);//此处是调用Catalog_Manager里的Get_Attr_Info_All()函数
				if (count != 0)
					Select_No_Where(DB_Name, Table_Name, print, count);
			}
		}
		else if (SQL.Right(2) != '.#')//选择语句（select * 形式，有where子句）
		{
			//获取表名
			SQL = SQL.Mid(1, SQL.GetLength() - 1);//规整SQL，因为20后有个'.'
			index = SQL.Find('.');//表名结束了
			Table_Name = SQL.Left(index);
			//下面两行想记录where条件但Condition没有在任何函数中出现，应该是要怎么转换一下吧，此处待解决
			//自认为已解决，需要调用Catalog_Manager中的Get_Condition()函数
			index = index + 1;
			Condition = SQL.Right(SQL.GetLength() - index);//where条件
														   //转换条件组
			Get_Condition(DB_Name, Table_Name, Condition, conds, count, cond, nodes, num);//此处是调用Catalog_Manager中的Get_Condition()函数

			if (Table_Name.Find(',') != -1)
				cout << "erroe: can not select from more than one table!" << endl;
			else//调用一系列函数实现select * where
			{
				//查找有无可用索引
				if (Find_Useful_Cond(conds, count, index) == 0)//无可用索引
				{
					//对表文件进行遍历
					Get_Attr_Info_All(DB_Name, Table_Name, print, count);//此处是调用Catalog_Manager里的Get_Attr_Info_All()函数
																		 //对每条记录，进行WHERE 条件匹配，若符合，则打印相应属性的值。
					if (count != 0)//这里不知道是什么意思，不知道该不该加，此处待解决
						Select_With_Where(DB_Name, Table_Name, conds, count, cond, print, Count);//此处是调用Record_Manager里的Select_With_Where()函数
				}
				else//有可用索引
				{
					//调用Index Manager模块的功能
					//查找符合WHERE 条件的记录，并打印相应属性的值。在最后打印出被选择的记录的条数。
					//根据不同的where条件调用下面的函数，这里还需要细分一下，此处待解决
					//感觉这里应该是放一个Record_Manager里类似于Select_With_Where这样的函数，
					//具体的调用索引是在Select_With_Where内部由下面列出的各种函数完成。
					//这里之后再改，下面的类似

					//用等于索引的选择语句
					Select_With_Equal_Cond(DB_Name, Table_Name, conds, count, print, Count, cond, Index);
					//用大于或大于等于索引的选择语句
					Select_With_Greater_Cond(DB_Name, Table_Name, conds, count, print, Count, cond, Index, type);
					//用小于或小于等于索引的选择语句
					Select_With_Smaller_Cond(DB_Name, Table_Name, conds, count, print, Count, cond, Index, type);

					if (count != 0)
						Select_Without_Useful_Cond(DB_Name, Table_Name, conds, count, print, Count, cond, index);//此处是调用Record_Manager里的Select_Without_Useful_Cond()函数
				}

			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//选择语句（select(属性名) 形式）
	else if (Type == "21")
	{
		if (DB_Name.IsEmpty())
			cout << "error: you have not specified the database to be used!" << endl;
		else if (SQL.Right(2) == '.#')//选择语句（select (属性) 形式，无where子句）
		{
			//获取选择属性组
			SQL = SQL.Mid(1, SQL.GetLength() - 1);//规整SQL，因为21后有个','
			index = SQL.Find('.');//属性结束了
			Attr = SQL.Left(index);
			index++;
			//获取表名
			Table_Name = SQL.Right(SQL.GetLength() - index);
			Table_Name = Table_Name.Left(Table_Name.GetLength() - 2);//去掉'.#'
			if (Table_Name.Find(',') != -1)
				cout << "error: can not select from more than one table!" << endl;
			else
			{
				//对表文件进行遍历
				Get_Attr_Info(DB_Name, Table_Name, print, count, Attr);//此处是调用Catalog_Manager里的Get_Attr_Info()函数
				if (count != 0)
					Select_No_Where(DB_Name, Table_Name, print, count);
			}
		}
		else if (SQL.Right(2) != '.#')//选择语句（select (属性) 形式，有where子句）
		{
			//获取属性组
			SQL = SQL.Mid(1, SQL.GetLength() - 1);//规整SQL，因为21后有个','
			index = SQL.Find('.');
			Attr = SQL.Left(index);
			//获取表名
			SQL = SQL.Mid(index + 1, SQL.GetLength() - index - 1);
			index = SQL.Find('.');
			Table_Name = SQL.Left(index);
			//获取where条件
			SQL = SQL.Mid(index + 1, SQL.GetLength() - index - 1);
			Condition = SQL;
			//转换条件组
			Get_Condition(DB_Name, Table_Name, Condition, conds, count, cond, nodes, num);//此处是调用Catalog_Manager中的Get_Condition()函数

			if (Table_Name.Find(',') != -1)
				cout << "error: can not select from more than one table!" << endl;
			else//调用一系列函数实现select (属性) where
			{
				//查找有无可用索引
				if (Find_Useful_Cond(conds, count, index) == 0)//无可用索引
				{
					//对表文件进行遍历
					Get_Attr_Info(DB_Name, Table_Name, print, count, Attr);//此处是调用Catalog_Manager里的Get_Attr_Info()函数
																		   //对每条记录，进行WHERE 条件匹配，若符合，则打印相应属性的值。
					if (count != 0)//这里不知道是什么意思，不知道该不该加，此处待解决
						Select_Without_Useful_Cond(DB_Name, Table_Name, conds, count, print, Count, cond);//此处是调用Record_Manager里的Select_Without_Useful_Cond()函数
				}
				else//有可用索引
				{
					//用等于索引的选择语句
					Select_With_Equal_Cond(DB_Name, Table_Name, conds, count, print, Count, cond, Index);
					//用大于或大于等于索引的选择语句
					Select_With_Greater_Cond(DB_Name, Table_Name, conds, count, print, Count, cond, Index, type);
					//用小于或小于等于索引的选择语句
					Select_With_Smaller_Cond(DB_Name, Table_Name, conds, count, print, Count, cond, Index, type);

					if (count != 0)
						Select_Without_Useful_Cond(DB_Name, Table_Name, conds, count, print, Count, cond, index);//此处是调用Record_Manager里的Select_Without_Useful_Cond()函数
				}

			}
		}
	}

	//后面程序删除了。

	/////////////////////////////////////////////////////////////////////////////////////////
	//插入语句
	//insert:"30" + "table_name" + "数据类型" + "数据内容," + ... + "数据内容,"
	//数据类型 : +: int - : float / : char
	/*
	根据Catalog Manager处理生成的初步内部数据形式，提取表名及记录，
	根据信息计算插入记录的块号，并调用Buffer Manager 的功能，
	获取指定的内存块，并将记录插入到内存中，同时修改脏位及表信息即可。
	*/
	else if (Type == "30") {
		//获取表名
		//这里需要Interpreter中在tablename后面加一个‘.’
		index = SQL.Find('.');
		Table_Name = SQL.Left(index);
		//获取插入属性
		Attr = SQL.Mid(index + 1, SQL.GetLength() - index - 1);
		//向表中插入元组
		Insert_Item(DB_Name, Table_Name, Attr, record_Num);//此处是Record_Manager中的Insert_Item()函数
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//删除语句
	//delete: "40"+"表名,"+"where条件（表示方法同select）"（有的话） （没有的话以#结尾）
	else if (Type == "40") {
		//这里将不同于select的代码，直接用最简洁的方式
		if (DB_Name.IsEmpty())
			cout << "error: you have not specified the database to be used!" << endl;
		else if (SQL.Right(2) == ',#')//删除语句（无where子句）
		{
			//获取表名
			index = SQL.Find(',');
			Table_Name = SQL.Left(index);

			if (Table_Name.Find(',') != -1)
				cout << "error: can not select from more than one table!" << endl;
			else
				//删除表内所有元组
				Empty_Table(DB_Name, Table_Name, index_name, count);//这里是调用Catalog_Manager函数中的Empty_Table()函数
		}
		else//删除语句（有where子句）
		{
			//获取表名
			index = SQL.Find(',');
			Table_Name = SQL.Left(index);
			//获取where条件
			SQL = SQL.Mid(index + 1, SQL.GetLength() - index - 1);
			Condition = SQL;
			//转换条件组
			Get_Condition(DB_Name, Table_Name, Condition, conds, count, cond, nodes, num);//此处是调用Catalog_Manager中的Get_Condition()函数

			if (Table_Name.Find(',') != -1)
				cout << "error: can not select from more than one table!" << endl;
			else
				//删除语句（有where）
				Delete_With_Where(DB_Name, Table_Name, conds, count, nodes, num, cond);//这里是调用Record_Manager里的Delete_With_Where函数
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//use 语句
	//use:"50"+"database_name"
	else if (Type == "50") {
		DB_Name = SQL;
		Use_Database(DB_Name);//此处是调用Catalog_Manager函数中的Use_Database()函数
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//执行文件语句
	//	execfile:"60"+"file_name" //存疑，不知道文件名是否应该加引号
	else if (Type == "60") {
		File = SQL;
		Exect_File(File);//此处是调用Catalog_Manager函数中的Exect_File()函数
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//退出
	else if (Type == "70") {
		DB_Name = SQL;
		Quit(DB_Name);//此处是调用Record_Manager中的Quit()函数
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//帮助
	else if (Type == "80") {
		Help();//此处是调用API_Module中的Help()函数
	}

}

//显示帮助信息
void Help()
{
	printf("call API_Module->Help();\n");
	printf("显示帮助信息\n");
}

//插入索引节点
void Insert_Index(CString DB_Name, CString Table_Name, index_info & index)
{
	printf("call API_Module->Insert_Index(...);\n");
	printf("插入索引节点\n");
}
//删除索引节点
void Delete_Index(CString DB_Name, CString Table_Name, index_info & index)
{
	printf("call API_Module->Delete_Index(...);\n");
	printf("删除索引节点\n");
}
//查找索引节点
void Find_Index(CString DB_Name, CString Table_Name, index_info & index)
{
	printf("call API_Module->Find_Index(...);\n");
	printf("查找索引节点\n");
}
//获取条件的索引信息
void Get_Index(CString DB_Name, CString Table_Name, int & start, int & end, int type, index_info & index)
{
	printf("call API_Module->Get_Index(...);\n");
	printf("获取条件的索引信息\n");
}
