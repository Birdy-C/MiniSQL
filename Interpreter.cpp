//////////////////////////////////////////////////////////
///----------------------------------------------------///
///       Module: Interpreter_Module                           ///
///       Produced by: ZZZ                          ///
///       Description: Produced to deal with SQL parse ///
///       date: 2004/12/25                             ///
///----------------------------------------------------///
//////////////////////////////////////////////////////////

#include"Interpreter.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//读取用户输入
CString read_input()
{
	CString SQL;
	CString temp;
	char str[100];
	bool finish = false;
	while (!finish)
	{
		cin >> str;//空格为分界
		temp = str;
		SQL = SQL + " " + temp;
		if (SQL.GetAt(SQL.GetLength() - 1) == ';')//GetAt返回特定位置的字符（第一位是0） 此处判断SQL最后一位是不是分号
		{
			SQL.SetAt(SQL.GetLength() - 1, ' ');//如果是分号就把分号先变成空格，然后再加一个分号？
			SQL += ";";
			finish = true;
		}
	}
	//将输入大写转化为小写
	SQL.MakeLower();
	//返回用户输入
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create database语句是否有效
CString create_database(CString SQL, int start)
{
	CString temp;
	int index, end;
	//获取第三个单词
	while (SQL.GetAt(start) == ' ')
		start++;
	index = start;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.IsEmpty())
	{
		cout << "error: database name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.GetAt(start) == ' ')
			start++;
		//若为非法信息，打印出错信息
		if (SQL.GetAt(start) != ';' || start != SQL.GetLength() - 1)
		{
			wcout << "error12:" << SQL.Mid(index, SQL.GetLength() - index - 2).GetString() << "---is not a valid database name!" << endl;
			SQL = "99";
		}
		//返回create database语句的内部形式
		else
			SQL = "00" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//获得属性
CString get_attribute(CString temp, CString sql)
{
	int start = 0, end, index;
	CString T, C;
	temp += " ";//为了方便一会找数据类型什么时候结束
				//获得属性名
	end = temp.Find(' ', start);
	T = temp.Mid(start, end - start);
	start = end + 1;
	sql += T + " ";
	//获得数据类型
	while (temp.GetAt(start) == ' ')
		start++;
	end = temp.Find(' ', start);
	T = temp.Mid(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (T.IsEmpty())
	{
		cout << "error : error in create table statement!" << endl;
		sql = "99";
		return sql;
	}
	//若为int 
	else if (T == "int")
		sql += "+";
	//若为float
	else if (T == "float")
		sql += "-";
	//其他 按大程要求，其他就是char()了……
	else
	{
		index = T.Find('(');//左括号
		C = T.Left(index);//正好应该是char这个字符串
		index++;
		//若有误，打印出错信息
		if (C.IsEmpty())
		{
			cout << "error: " << T << "---is not a valid data type definition!" << endl;
			sql = "99";
			return sql;
		}
		//若为char
		else if (C == "char")
		{
			C = T.Mid(index, T.GetLength() - index - 1);//char的长度
			if (C.IsEmpty())
			{
				cout << "error: the length of the data type char has not been specified!" << endl;
				sql = "99";
				return sql;
			}
			else
				sql += C;
		}
		//若为非法信息，打印出错信息
		else
		{
			cout << "error: " << C << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
	}
	//是否有附加信息 有的话也就一个unique
	while (start<temp.GetLength() && temp.GetAt(start) == ' ')
		start++;
	if (start<temp.GetLength())
	{
		//若为unique定义，保存信息
		end = temp.Find(' ', start);
		T = temp.Mid(start, end - start);
		if (T == "unique")
		{
			sql += " 1,";
		}
		//若为非法信息，打印出错信息
		else
		{
			cout << "error: " << temp << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
	}
	//若无附加信息
	else
		sql += " 0,";
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create table语句是否有效
CString create_table(CString SQL, int start)
{
	CString temp, sql, T;
	int index, end, length;
	//获取表名
	while (SQL.GetAt(start) == ' ')//找到第一个不是空格的位置
		start++;
	index = start;//这个index干嘛的
	if ((end = SQL.Find('(', start)) == -1)//end是左括号的位置，end==-1表示没找到左括号
	{
		cout << "error: missing ( in the statement!" << endl;
		SQL = "99";
		return SQL;
	}
	temp = SQL.Mid(start, end - start);//空格和左括号中间的字符串
	start = end + 1;//更新start
	if (!temp.IsEmpty())//处理表名和左括号之间的空格
	{
		while (SQL.GetAt(start) == ' ')
			start++;
		length = temp.GetLength() - 1;//减一变成下标……因为Index是从0开始的
		while (temp.GetAt(length) == ' ')//length指向表名的最后一位
			length--;
		temp = temp.Left(length + 1);
	}
	//若无，打印出错信息
	if (temp.IsEmpty())//表名为空
	{
		cout << "error: error in create table statement!" << endl;
		SQL = "99";
		return SQL;
	}
	//若为非法信息，打印出错信息
	else if (temp.Find(' ') != -1)//这应该是在表名中间有空格
	{
		wcout << "error: " << temp.GetString() << "---is not a valid table name!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		sql = temp + ",";
		//获取每个属性
		while ((end = SQL.Find(',', start)) != -1)
		{
			temp = SQL.Mid(start, end - start);
			start = end + 1;
			//若有空属性，打印出错信息 俩逗号连一起了才能出这种情况吧……
			if (temp.IsEmpty())
			{
				cout << "error: error in create table statement!" << endl;
				SQL = "99";
				return SQL;
			}
			//保存属性
			else
			{
				sql = get_attribute(temp, sql);
				if (sql == "99")
					return sql;
			}
			while (SQL.GetAt(start) == ' ')
				start++;
		}
		//获得最后属性
		temp = SQL.Mid(start, SQL.GetLength() - start - 1);//最后一位没包括进来……？嗷，最后一位是分号？在read_input函数里设定过，最后一位肯定是分号…… 咦那从文件里读来的怎么办……
		length = temp.GetLength() - 1;//又要把这个length当index用了
		while (temp.GetAt(length) != ')'&&length >= 0)//有些调皮的用户会在括号后边输空格……
			length--;
		//若无，打印出错信息
		if (length<1)
		{
			cout << "error: error in create table statement!" << endl;
			SQL = "99";
			return SQL;
		}
		//存储属性
		else
		{
			temp = temp.Left(length);
			end = SQL.Find(' ', start);
			T = SQL.Mid(start, end - start);
			start = end + 1;
			//若为主键定义 嗯最后一个属性可能并不是属性而是主键定义……
			if (T == "primary")
			{
				//判断是否有关键字key
				temp += ")";//这加个括号是干啥……
				while (SQL.GetAt(start) == ' ')
					start++;
				end = SQL.Find('(', start);
				T = SQL.Mid(start, end - start);
				start = end + 1;
				length = T.GetLength() - 1;
				while (T.GetAt(length) == ' ')
					length--;
				T = T.Left(length + 1);
				//若为空，打印出错信息
				if (T.IsEmpty())
				{
					cout << "syntax error: syntax error in create table statement!" << endl;
					cout << "\t missing key word key!" << endl;
					SQL = "99";
					return SQL;
				}
				//若有，继续验证
				else if (T == "key")
				{
					//获取主键属性名
					while (SQL.GetAt(start) == ' ')
						start++;
					end = SQL.Find(')', start);
					T = SQL.Mid(start, end - start);
					length = T.GetLength() - 1;
					while (T.GetAt(length) == ' ')
						length--;
					T = T.Left(length + 1);
					//若无，打印出错信息
					if (T.IsEmpty())
					{
						cout << "error : missing primary key attribute name!" << endl;
						SQL = "99";
						return SQL;
					}
					//若为非法信息，打印出错信息
					else if (T.Find(' ') != -1)
					{
						cout << "error : " << T << "---is not a valid primary key attribute name!" << endl;
						SQL = "99";
						return SQL;
					}
					//保存主键
					else
					{
						sql += T + " #,";
						SQL = "01" + sql;
					}
				}
				//若为非法信息，打印出错信息
				else
				{
					cout << "error : " << T << "---is not a valid key word!" << endl;
					SQL = "99";
					return SQL;
				}
			}
			//若为一般属性
			else
			{
				sql = get_attribute(temp, sql);
				if (sql == "99")
				{
					SQL = "99";
					return SQL;
				}
				else
					SQL = "01" + sql;
			}
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create index on语句是否有效
CString create_index_on(CString SQL, int start, CString sql)
{
	CString temp;
	int end, length;
	//获取表名
	while (SQL.GetAt(start) == ' ')
		start++;
	end = SQL.Find('(', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.IsEmpty())
	{
		cout << "syntax error: syntax error for create index statement!" << endl;
		cout << "\t missing ( !" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		//检验是否为有效文件名
		length = temp.GetLength() - 1;
		while (temp.GetAt(length) == ' ')
			length--;
		temp = temp.Left(length + 1);
		//有效
		if (temp.Find(' ') == -1)
		{
			sql += " " + temp;
			//获取属性名
			while (SQL.GetAt(start) == ' ')
				start++;
			end = SQL.Find(')', start);
			temp = SQL.Mid(start, end - start);
			start = end + 1;
			//若无，打印出错信息
			if (temp.IsEmpty())
			{
				cout << "syntax error: syntax error for create index statement!" << endl;
				cout << "\t missing ) !" << endl;
				SQL = "99";
				return SQL;
			}
			else
			{
				//检验是否为有效属性名
				length = temp.GetLength() - 1;
				while (temp.GetAt(length) == ' ')
					length--;
				temp = temp.Left(length + 1);
				//有效
				if (temp.Find(' ') == -1)
				{
					sql += " " + temp;
					while (SQL.GetAt(start) == ' ')
						start++;
					if (SQL.GetAt(start) != ';' || start != SQL.GetLength() - 1)
					{
						cout << "syntax error: syntax error for quit!" << endl;
						SQL = "99";
						return SQL;
					}
					//返回create index语句的内部形式
					else
						SQL = "02" + sql;
				}
				//无效,打印出错信息
				else
				{
					wcout << "error:" << " " << temp.GetString() << "---is not a valid attribute name!" << endl;
					SQL = "99";
					return SQL;
				}
			}
		}
		//无效,打印出错信息
		else
		{
			cout << "error:" << " " << temp << "---is not a valid table name!" << endl;
			SQL = "99";
			return SQL;
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create index语句是否有效
CString create_index(CString SQL, int start)
{
	CString temp, sql;
	int end;
	//获取第三个单词
	while (SQL.GetAt(start) == ' ')
		start++;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.IsEmpty())
	{
		cout << "syntax error: syntax error for create index statement!" << endl;
		SQL = "99";
	}
	else
	{
		sql = temp;
		//获取第四个单词
		while (SQL.GetAt(start) == ' ')
			start++;
		end = SQL.Find(' ', start);
		temp = SQL.Mid(start, end - start);
		start = end + 1;
		//若无，打印出错信息
		if (temp.IsEmpty())
		{
			cout << "syntax error: syntax error for create index statement!" << endl;
			SQL = "99";
		}
		//若为on,继续验证
		else if (temp == "on")
			SQL = create_index_on(SQL, start, sql);
		//若为非法信息，打印非法信息
		else
		{
			wcout << "syntax error:" << " " << temp.GetString() << "---is not a valid key word!" << endl;
			SQL = "99";
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create语句是否有效
CString create_clause(CString SQL, int start)
{
	CString temp;
	int end;
	//获取第二个单词
	while (SQL.GetAt(start) == ' ')
		start++;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.IsEmpty())
	{
		cout << "syntax error: syntax error for create statement!" << endl;
		SQL = "99";
	}
	//若为database,继续验证
	else if (temp == "database")
		SQL = create_database(SQL, start);
	//若为table,继续验证
	else if (temp == "table")
		SQL = create_table(SQL, start);
	//若为index,继续验证
	else if (temp == "index")
		SQL = create_index(SQL, start);
	//若为错误信息，打印出错信息
	else
	{
		cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//返回create语句的内部形式
	return SQL;
}

CString drop_clause(CString SQL, int start)
{
	int end;
	CString temp;
	//获取第二个单词
	while (SQL.GetAt(start) == ' ')
		start++;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	if (temp.IsEmpty())//第二个单词为空，打印错误信息
	{
		cout << "syntax error: syntax error for drop statement!" << endl;
		SQL = "99";
	}
	else
	{
		if (temp == "database")
		{
			SQL = drop_database(SQL, start);
		}
		else if (temp == "table")
		{
			SQL = drop_table(SQL, start);
		}
		else if (temp == "index")
		{
			SQL = drop_index(SQL, start);
		}
		else
		{
			cout << "syntax error: syntax error for drop statement!" << endl;
			SQL = "99";
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证drop database语句是否有效
CString drop_database(CString SQL, int start)
{
	CString temp;
	int index, end;
	//获取第三个单词
	while (SQL.GetAt(start) == ' ')
		start++;
	index = start;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.IsEmpty())
	{
		cout << "error: database name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.GetAt(start) == ' ')
			start++;
		//若为非法信息，打印出错信息
		if (SQL.GetAt(start) != ';' || start != SQL.GetLength() - 1)//最后一位不是分号，或者后边还写了多余的东西
		{
			wcout << "error:" << SQL.Mid(index, SQL.GetLength() - index - 2).GetString() << "---is not a valid database name!" << endl;//减2是减去分号和空格
			SQL = "99";
		}
		//返回drop database语句的内部形式
		else
			SQL = "10" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证drop table语句是否有效
CString drop_table(CString SQL, int start)
{
	CString temp;
	int index, end;
	//获取第三个单词
	while (SQL.GetAt(start) == ' ')
		start++;
	index = start;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.IsEmpty())
	{
		cout << "error: table name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.GetAt(start) == ' ')
			start++;
		//若为非法信息，打印出错信息
		if (SQL.GetAt(start) != ';' || start != SQL.GetLength() - 1)//最后一位不是分号，或者后边还写了多余的东西
		{
			wcout << "error:" << SQL.Mid(index, SQL.GetLength() - index - 2).GetString() << "---is not a valid table name!" << endl;//减2是减去分号和空格
			SQL = "99";
		}
		//返回drop table语句的内部形式
		else
			SQL = "11" + temp;
	}

	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证drop index语句是否有效
CString drop_index(CString SQL, int start)
{
	CString temp;
	int index, end;
	//获取第三个单词
	while (SQL.GetAt(start) == ' ')
		start++;
	index = start;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//打印错误信息
	if (temp.IsEmpty())
	{
		cout << "error: index name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.GetAt(start) == ' ')
			start++;
		//若为非法信息，打印出错信息
		if (SQL.GetAt(start) != ';' || start != SQL.GetLength() - 1)//最后一位不是分号，或者后边还写了多余的东西
		{
			wcout << "error:" << SQL.Mid(index, SQL.GetLength() - index - 2).GetString() << "---is not a valid index name!" << endl;//减2是减去分号和空格
			SQL = "99";
		}
		//返回drop index语句的内部形式
		else
			SQL = "12" + temp;
	}
	return SQL;
}

//验证select 语句是否有效
CString select_clause(CString SQL, int start)//第二个词可能是*或一连串的列名
{
	CString temp, sql, temp_sql;
	int end, length;
	CString from;
	from = "from";
	//获取select和from之间的内容
	while (SQL.GetAt(start) == ' ')
		start++;
	end = SQL.Find(from, start);
	if (end == -1)//语句里没有from...
	{
		SQL = "99";
		cout << "syntax error: syntax error for select statement!" << endl;
		return SQL;
	}
	else//有from
	{
		temp = SQL.Mid(start, end - start);
		start = end + 1;
		while (SQL.GetAt(start) != ' ')
			start++;
		while (SQL.GetAt(start) == ' ')
			start++;
		//删一下temp和from中间的空格
		length = temp.GetLength() - 1;
		while (temp.GetAt(length) == ' ')
			length--;
		temp = temp.Left(length + 1);

		//若无，打印出错信息
		if (temp.IsEmpty())
		{
			cout << "syntax error: syntax error for select statement!" << endl;
			sql = "99";
		}
		else if (temp == "*")//select后第二个词是* 万一select和*是连着的怎么办……还有万一select *然后换行了呢……
		{
			sql = "20.";
			temp_sql = select_from(SQL, start);
			if (temp_sql == "99")
			{
				sql = "99";
				return sql;
			}
			else
			{
				sql += temp_sql;
			}
		}
		else//不是*，理论上应该是一堆逗号分隔的属性名，但是也可能是错误输入……
		{
			temp_sql = get_part(temp);
			if (temp_sql == "99")
			{
				sql = "99";
				return sql;
			}
			else
			{
				sql = "21," + temp_sql;
			}
			//扔进catalog检查有没有这些属性
			temp_sql = select_from(SQL, start);
			if (temp_sql == "99")
			{
				sql = "99";
				return sql;
			}
			else
			{
				sql += temp_sql;
			}
		}
		SQL = sql;
		return SQL;
	}
}
//获得属性组或文件组的每一项,逗号为分隔
CString get_part(CString SQL)
{
	int start = 0, end, length;
	CString temp, sql;
	while ((end = SQL.Find(',', start)) != -1)//逗号为分界
	{
		temp = SQL.Mid(start, end - start);
		start = end + 1;

		length = temp.GetLength() - 1;//删掉temp的结尾字符和逗号之间的空格
		while (temp.GetAt(length) == ' ')
			length--;
		temp = temp.Left(length + 1);

		//若有空属性，打印出错信息
		if (temp.IsEmpty())
		{
			cout << "error: error in select statement!" << endl;
			SQL = "99";
			return SQL;
		}
		else
		{
			//验证temp是否合法
			if (temp.Find(' ') != -1)
			{
				sql = "99";
				cout << "error: error in select statement!" << endl;
			}
			else
			{
				sql = sql + temp + ",";
			}
		}
		while (SQL.GetAt(start) == ' ')
			start++;
	}
	//获取最后一个属性
	temp = SQL.Mid(start, SQL.GetLength() - start);
	length = temp.GetLength() - 1;//删掉temp的结尾字符和逗号之间的空格
	while (temp.GetAt(length) == ' ')
		length--;
	temp = temp.Left(length + 1);

	if (temp.IsEmpty())
	{
		cout << "error: error in create table statement!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		if (temp.Find(' ') != -1)
		{
			sql = "99";
			cout << "error: error in select statement!" << endl;
		}
		else
		{
			sql = sql + temp + ".";
		}
	}
	return sql;
}
//处理select from语句
CString select_from(CString SQL, int start)
{
	CString sql, temp, temp_sql;
	int end, length;
	CString where_;
	where_ = "where";
	end = SQL.Find(where_, start);
	if (end == -1)//语句里没有where,再看有没有分号
	{
		end = SQL.Find(';', start);
		if (end == -1)//没有分号……
		{
			SQL = "99";
			cout << "syntax error: syntax error for select statement!" << endl;
			return SQL;
		}
		else//有分号,得验证一下表名是否合法
		{
			temp = SQL.Mid(start, end - start);
			while (SQL.GetAt(start) != ' ')
				start++;
			while (SQL.GetAt(start) == ' ')
				start++;

			//验证表名，先删空格
			length = temp.GetLength() - 1;
			while (temp.GetAt(length) == ' ')
				length--;
			temp = temp.Left(length + 1);

			//若无，打印出错信息
			if (temp.IsEmpty())
			{
				cout << "syntax error: syntax error for insert statement!" << endl;
				SQL = "99";
				return SQL;
			}
			else
			{
				if (temp.Find(' ') != -1)//验证表名是否有效 感觉不应该这么检查，应该丢进catalog里检查……嘛先放着
				{
					wcout << "error: " << temp.GetString() << "---is not a valid table name!" << endl;
					SQL = "99";
				}
				else
				{
					sql = temp + ".";//表名
				}
				/*temp_sql = get_part(temp);
				if (temp_sql == "99")
				{
				sql = "99";
				}
				else
				{
				sql = get_part(temp) + '#';
				}*/
			}
		}
	}
	else//有where
	{
		temp = SQL.Mid(start, end - start);
		start = end + 1;
		while (SQL.GetAt(start) != ' ')
			start++;
		while (SQL.GetAt(start) == ' ')
			start++;
		//		sql = get_part(temp);
		//验证表名，先删空格
		length = temp.GetLength() - 1;
		while (temp.GetAt(length) == ' ')
			length--;
		temp = temp.Left(length + 1);

		//若无，打印出错信息
		if (temp.IsEmpty())
		{
			cout << "syntax error: syntax error for insert statement!" << endl;
			SQL = "99";
			return SQL;
		}
		else
		{
			if (temp.Find(' ') != -1)//验证表名是否有效 感觉不应该这么检查，应该丢进catalog里检查……嘛先放着
			{
				wcout << "error: " << temp.GetString() << "---is not a valid table name!" << endl;
				SQL = "99";
			}
			else
			{
				sql = temp + ".";//表名
			}
			/*temp_sql = get_part(temp);
			if (temp_sql == "99")
			{
			sql = "99";
			}
			else
			{
			sql = get_part(temp) + '#';
			}*/
		}

		//接下来记录where条件
		end = SQL.Find(';', start);//找到分号
		if (end == -1)//没有分号……
		{
			SQL = "99";
			cout << "syntax error: syntax error for select statement!" << endl;
			return SQL;
		}
		else//找到了分号
		{
			temp = SQL.Mid(start, end - start);
			while (SQL.GetAt(start) != ' ')
				start++;
			while (SQL.GetAt(start) == ' ')
				start++;

			temp_sql = get_where(temp);
			if (temp_sql == "99")
			{
				sql = "99";
			}
			else
			{
				sql = sql + temp_sql;
			}
		}
	}

	return sql;
}
//处理where语句
CString get_where(CString SQL)
{
	int start = 0, end, length;
	CString temp, sql, temp_sql;
	//and为分界……万一某个字符串里边有and可咋办呢……
	CString and;
	and = "and";

	while ((end = SQL.Find(and, start)) != -1)//and为分界，循环 糟糕啊，这破语句好像找的不是and而是a。。。
	{
		temp = SQL.Mid(start, end - start);
		start = end + 1;
		while (SQL.GetAt(start) != ' ')
			start++;
		while (SQL.GetAt(start) == ' ')
			start++;

		length = temp.GetLength() - 1;//删掉temp的结尾字符和and之间的空格
		while (temp.GetAt(length) == ' ')
			length--;
		temp = temp.Left(length + 1);
		//然后还应该丢进一个函数里边去判断where的每一项对不对……
		sql += where_clause(temp);
		sql += '&';

	}
	//最后一个属性
	temp = SQL.Mid(start, SQL.GetLength() - start);
	if (temp.IsEmpty())
	{
		cout << "error: error in where statement!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		temp_sql = where_clause(temp);
		if (temp_sql == "99")
		{
			sql = "99";
		}
		else
		{
			sql = sql + temp_sql + ".";
		}
	}
	return sql;

}
//处理where语句的每个子句
CString where_clause(CString SQL)
{
	int start = 0, end, length;
	CString temp, op, sql;
	int op_start = start, op_end;
	length = SQL.GetLength();
	//找到运算符的位置
	while (SQL.GetAt(op_start) != '='&&SQL.GetAt(op_start) != '<'&&SQL.GetAt(op_start) != '>')
	{
		if (op_start == length - 1)
			break;
		op_start++;
	}

	if (op_start == length - 1)//没找到运算符
	{
		SQL = "99";
		cout << "error: no operator in where statement!" << endl;
		return SQL;
	}
	else//找到了运算符，此时op_start是运算符第一位的下标
	{
		//判断一下运算符是什么
		op_end = op_start + 1;
		if (SQL.GetAt(op_start) == '<' && (SQL.GetAt(op_end) == '=' || SQL.GetAt(op_end) == '>'))//如果第一位是小于号，看看下一位是不是等号或大于号
		{
			op_end++;
		}
		if (SQL.GetAt(op_start) == '>'&&SQL.GetAt(op_end) == '=')//如果第一位是大于号，看下一位是不是等号
		{
			op_end++;
		}
		op = SQL.Mid(op_start, op_end - op_start);//获取运算符
		temp = SQL.Mid(start, op_start - start);//获取运算符左边属性

		length = temp.GetLength() - 1;//删掉temp的结尾字符和运算符之间的空格
		while (temp.GetAt(length) == ' ')
			length--;
		temp = temp.Left(length + 1);

		if (temp.IsEmpty())
		{
			cout << "error: error in where statement!" << endl;
			SQL = "99";
			return SQL;
		}
		else {
			//然后应该去catalog里边验证一下是否合法
			sql = temp;
			//运算符的解析符……
			if (op_end - op_start == 2)//两位的运算符直接存
			{
				sql += op;
			}
			else if (op_end - op_start == 1)//一位的存成一个空格加上运算符
			{
				sql = sql + ' ' + op;
			}

			//看下最后的值吧
			//感觉这个还是应该丢到catalog里边看看它应该是什么属性
			//先不管了……
			start = op_end;
			while (SQL.GetAt(start) == ' ')
				start++;
			end = SQL.GetLength();
			temp = SQL.Mid(start, end - start);


			//然后又是分三种情况了……


			if (temp.IsEmpty())//空的
			{
				cout << "error: error in where statement!" << endl;
				SQL = "99";
				return SQL;
			}
			else//不空，要判断是啥……
			{
				length = temp.GetLength() - 1;//删掉空格
				while (temp.GetAt(length) == ' ')
					length--;
				temp = temp.Left(length + 1);

				if (temp.GetAt(0) == '\'')//char类型，需验证开头结尾都是单引号
				{
					if (temp.GetAt(length) != '\'')
					{
						cout << "error: error in insert values!" << endl;
						SQL = "99";
						return SQL;
					}
					else
					{
						sql = sql + " /" + temp;
					}
				}
				else if (temp.Find('.') != -1)//float类型，需验证除了小数点之外的字符全是数字
				{
					for (int i = 0; i <= length; i++)
					{
						if (temp.GetAt(i) < '0' || temp.GetAt(i) > '9')
						{
							if (i != temp.Find('.'))
							{
								sql = "99";
								break;
							}
						}

					}
					if (sql != "99")
					{
						sql = sql + " -" + temp;
					}
				}
				else//int类型，需验证每一位都是数字...用验证首位不是0吗，好像不用吧。。
				{
					for (int i = 0; i <= length; i++)
					{
						if (temp.GetAt(i) < '0' || temp.GetAt(i) > '9')
						{
							sql = "99";
							break;
						}
					}
					if (sql != "99")
					{
						sql = sql + " +" + temp;
					}
				}
				if (sql == "99")
					return sql;
			}
		}

		return sql;
	}
}
//验证insert 语句是否有效（检查第二个单词是不是into,以及表名是否有效）
CString insert_clause(CString SQL, int start)
{
	CString temp, sql;
	int end;
	//获取第二个单词
	while (SQL.GetAt(start) == ' ')
		start++;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.IsEmpty())
	{
		cout << "syntax error: syntax error for insert statement!" << endl;
		SQL = "99";
	}
	else if (temp == "into")
	{
		//获取第三个单词
		while (SQL.GetAt(start) == ' ')
			start++;
		end = SQL.Find(' ', start);
		temp = SQL.Mid(start, end - start);
		start = end + 1;
		//若无，打印出错信息
		if (temp.IsEmpty())
		{
			cout << "syntax error: syntax error for insert statement!" << endl;
			SQL = "99";
		}
		else
		{
			if (temp.Find(' ') != -1)//验证表名是否有效
			{
				wcout << "error: " << temp.GetString() << "---is not a valid table name!" << endl;
				SQL = "99";
			}
			else
			{
				sql = temp + ",";//表名
				SQL = insert_into_values(SQL, start, sql);
			}
		}
	}
	else
	{
		cout << "syntax error: syntax error for insert statement!" << endl;
		SQL = "99";
	}
	return SQL;
}
//验证insert into values语句是否有效
CString insert_into_values(CString SQL, int start, CString sql)
{
	int end, length;
	CString temp;
	//先检查下一个单词是不是values
	while (SQL.GetAt(start) == ' ')
		start++;
	end = SQL.Find('(', start);
	if (end == -1)//没有左括号
	{
		cout << "error: missing ( in the statement!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		temp = SQL.Mid(start, end - start);
		start = end + 1;
		//先删掉values和左括号之间的空格
		length = temp.GetLength() - 1;
		while (temp.GetAt(length) == ' ')
			length--;
		temp = temp.Left(length + 1);

		if (temp == "values")
		{
			while (SQL.GetAt(start) == ' ')
				start++;
			//获取除最后一个属性外的每个属性
			while ((end = SQL.Find(',', start)) != -1)//逗号为分界
			{
				temp = SQL.Mid(start, end - start);
				start = end + 1;

				length = temp.GetLength() - 1;//删掉temp的结尾字符和逗号之间的空格
				while (temp.GetAt(length) == ' ')
					length--;
				temp = temp.Left(length + 1);

				//若有空属性，打印出错信息
				if (temp.IsEmpty())
				{
					cout << "error: error in create table statement!" << endl;
					SQL = "99";
					return SQL;
				}
				//保存属性
				else
				{
					if (temp.GetAt(0) == '\'')//char类型，需验证开头结尾都是单引号
					{
						if (temp.GetAt(length - 1) != '\'')
						{
							cout << "error: error in insert values!" << endl;
							SQL = "99";
							return SQL;
						}
						else
						{
							sql = sql + " /" + temp + ",";
						}
					}
					else if (temp.Find('.') != -1)//float类型，需验证除了小数点之外的字符全是数字
					{
						for (int i = 0; i <= length; i++)
						{
							if (temp.GetAt(i) < '0' || temp.GetAt(i) > '9')
							{
								if (i != temp.Find('.'))
								{
									sql = "99";
									break;
								}
							}

						}
						if (sql != "99")
						{
							sql = sql + " -" + temp + ",";
						}
					}
					else//int类型，需验证每一位都是数字...用验证首位不是0吗，好像不用吧。。
					{
						for (int i = 0; i <= length; i++)
						{
							if (temp.GetAt(i) < '0' || temp.GetAt(i) > '9')
							{
								sql = "99";
								break;
							}
						}
						if (sql != "99")
						{
							sql = sql + " +" + temp + ",";
						}
					}
					if (sql == "99")
						return sql;
				}

				while (SQL.GetAt(start) == ' ')
					start++;
			}
			//获取最后一个属性
			temp = SQL.Mid(start, SQL.GetLength() - start - 1);
			length = temp.GetLength() - 1;
			while (temp.GetAt(length) != ')'&&length >= 0)//删掉分号和右括号之间的空格
				length--;
			//若无，打印出错信息
			if (length<1)
			{
				cout << "error: error in insert statement!" << endl;
				SQL = "99";
				return SQL;
			}
			//验证属性
			else
			{
				temp = temp.Left(length);
				if (temp.GetAt(0) == '\'')//char类型，需验证开头结尾都是单引号
				{
					if (temp.GetAt(length - 1) != '\'')
					{
						cout << "error: error in insert values!" << endl;
						SQL = "99";
						return SQL;
					}
					else
					{
						sql = sql + " /" + temp + ",";
					}
				}
				else if (temp.Find('.') != -1)//float类型，需验证除了小数点之外的字符全是数字
				{
					for (int i = 0; i <= length; i++)
					{
						if (temp.GetAt(i) < '0' || temp.GetAt(i) > '9')
						{
							if (i != temp.Find('.'))
							{
								sql = "99";
								break;
							}
						}
					}
					if (sql != "99")
					{
						sql = sql + " -" + temp + ",";
					}

				}
				else//int类型，需验证每一位都是数字...用验证首位不是0吗，好像不用吧。。
				{
					for (int i = 0; i < length; i++)
					{
						if (temp.GetAt(i) < '0' || temp.GetAt(i) > '9')
						{
							sql = "99";
							break;
						}
						if (sql != "99")
						{
							sql = sql + " +" + temp + ",";
						}

					}
				}
				if (sql == "99")
					return sql;
			}
		}
		else//输入的词不是values
		{
			cout << "error: error in insert statement!" << endl;
			SQL = "99";
			return SQL;
		}
	}
	//应该是这里扔到catalog里边再检查一下
	SQL = "30" + sql;
	return SQL;
}


//验证delete语句是否有效
CString delete_clause(CString SQL, int start)
{
	//检查一下第二个词是不是from
	CString temp, sql, temp_sql;
	int end, length;
	//获取第二个单词
	while (SQL.GetAt(start) == ' ')
		start++;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.IsEmpty())
	{
		cout << "syntax error: syntax error for delete statement!" << endl;
		SQL = "99";
	}
	else if (temp == "from")//第二个词是from,接着检查第三个词是不是合法表名
	{
		//获取第三个单词
		while (SQL.GetAt(start) == ' ')
			start++;
		end = SQL.Find(' ', start);
		temp = SQL.Mid(start, end - start);
		start = end;
		//若无，打印出错信息
		if (temp.IsEmpty())
		{
			cout << "syntax error: syntax error for insert statement!" << endl;
			SQL = "99";
			return SQL;
		}
		else
		{
			if (temp.Find(' ') != -1)//验证表名是否有效 感觉不应该这么检查，应该丢进catalog里检查……嘛先放着
			{
				wcout << "error: " << temp.GetString() << "---is not a valid table name!" << endl;
				SQL = "99";
			}
			else
			{
				sql = temp + ",";//表名
								 //然后看下边是where还是分号
				CString where_;
				where_ = "where";
				end = SQL.Find(where_, start);

				if (end == -1)//没有where,那么直接验证下一个词是不是分号就好了……
				{
					while (SQL.GetAt(start) == ' ')
						start++;
					length = SQL.GetLength();
					temp = SQL.Mid(start, length - start);
					if (temp == ';')
					{
						sql += "#";
					}
					else
					{
						sql = "99";
						cout << "syntax error: syntax error for delete statement!" << endl;
						return sql;
					}
				}
				else //有where
				{
					start = end + 1;
					sql = delete_from_where(SQL, start, sql);
				}
			}

		}
	}
	else
	{
		cout << "syntax error: syntax error for insert statement!" << endl;
		SQL = "99";
	}
	SQL = "40" + sql;
	return SQL;
}
//验证 delete from where 语句是否有效
CString delete_from_where(CString SQL, int start, CString sql)
{
	int end;
	CString temp, temp_sql;
	//接下来记录where条件
	while (SQL.GetAt(start) != ' ')
		start++;
	while (SQL.GetAt(start) == ' ')
		start++;
	end = SQL.Find(';', start);//找到分号
	if (end == -1)//没有分号……
	{
		SQL = "99";
		cout << "syntax error: syntax error for delete statement!" << endl;
		return SQL;
	}
	else//找到了分号
	{
		temp = SQL.Mid(start, end - start);
		temp_sql = get_where(temp);
		if (temp_sql == "99")
		{
			sql = "99";
		}
		else
		{
			sql = sql + temp_sql;
		}
	}
	return sql;
}


CString use_clause(CString SQL, int start)
{
	CString temp;
	int index, end;
	//获取第二个单词
	while (SQL.GetAt(start) == ' ')
		start++;
	index = start;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.IsEmpty())
	{
		cout << "error: database name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.GetAt(start) == ' ')
			start++;
		//若为非法信息，打印出错信息
		if (SQL.GetAt(start) != ';' || start != SQL.GetLength() - 1)
		{
			wcout << "error12:" << SQL.Mid(index, SQL.GetLength() - index - 2).GetString() << "---is not a valid database name!" << endl;
			SQL = "99";
		}
		//返回use语句的内部形式
		else
			SQL = "50" + temp;
	}
	return SQL;
}

CString execfile_clause(CString SQL, int start)
{
	CString temp;
	int index, end;
	//获取第二个单词
	while (SQL.GetAt(start) == ' ')
		start++;
	index = start;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.IsEmpty())
	{
		cout << "error: file name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.GetAt(start) == ' ')
			start++;
		//若为非法信息，打印出错信息
		if (SQL.GetAt(start) != ';' || start != SQL.GetLength() - 1)
		{
			wcout << "error12:" << SQL.Mid(index, SQL.GetLength() - index - 2).GetString() << "---is not a valid file name!" << endl;
			SQL = "99";
		}
		//返回use语句的内部形式
		else
			SQL = "60" + temp;
	}
	return SQL;
}//此处存疑，文件名是否需要加引号……
CString quit_clause(CString SQL, int start)
{
	int end, index;

	while (SQL.GetAt(start) == ' ')
		start++;
	index = start;
	//若为非法信息，打印出错信息
	if (SQL.GetAt(start) != ';' || start != SQL.GetLength() - 1)//最后一位不是分号，或者后边还写了多余的东西
	{
		wcout << "error:" << SQL.Mid(index, SQL.GetLength() - index - 2).GetString() << "---is not a valid quit clause!" << endl;//减2是减去分号和空格
		SQL = "99";
	}
	//返回quit语句的内部形式
	else
		SQL = "70";

	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//获取用户输入，并对输入作有效性检查，若正确，返回语句的内部表示形式
CString Interpreter(CString statement)
{
	CString SQL;
	CString temp;
	CString sql;
	int start = 0, end;
	if (statement.IsEmpty())//如果statement是空的就让用户输入命令
							//获取用户输入
		SQL = read_input();
	else
		SQL = statement;
	//获取输入的第一个单词
	while (SQL.GetAt(start) == ' ')//找到第一个不是空格的字符
		start++;
	end = SQL.Find(' ', start);//从start开始找，第一个是空格的位置（如果start不为0则不包括start）
	temp = SQL.Mid(start, end - start);//刚好是第一个单词
	start = end + 1;//如果俩单词中间只有一个空格的话，现在的start应该是第二个单词的第一个字母
					//若无输入，打印出错信息
	if (temp.IsEmpty())
	{
		cout << "syntax error: empty statement!" << endl;
		SQL = "99";
	}
	//若为create语句
	else if (temp == "create")
		SQL = create_clause(SQL, start);
	//若为drop语句
	else if (temp == "drop")
		SQL = drop_clause(SQL, start);

	//若为select语句
	else if (temp == "select")
		SQL = select_clause(SQL, start);

	//若为insert语句
	else if (temp == "insert")
		SQL = insert_clause(SQL, start);
	//若为delete语句
	else if (temp == "delete")
		SQL = delete_clause(SQL, start);
	//若为use语句
	else if (temp == "use")
		SQL = use_clause(SQL, start);
	//若为execfile语句
	else if (temp == "execfile")
		SQL = execfile_clause(SQL, start);
	//若为quit语句
	else if (temp == "quit")
		SQL = quit_clause(SQL, start);
	//获取帮助
	else if (temp == "help")
		SQL = "80";
	//若为非法语句
	else
	{
		wcout << "syntax error:" << " " << temp.GetString() << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//返回输入语句的内部形式
	return SQL;
}
