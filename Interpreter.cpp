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
//��ȡ�û�����
CString read_input()
{
	CString SQL;
	CString temp;
	char str[100];
	bool finish = false;
	while (!finish)
	{
		cin >> str;//�ո�Ϊ�ֽ�
		temp = str;
		SQL = SQL + " " + temp;
		if (SQL.GetAt(SQL.GetLength() - 1) == ';')//GetAt�����ض�λ�õ��ַ�����һλ��0�� �˴��ж�SQL���һλ�ǲ��Ƿֺ�
		{
			SQL.SetAt(SQL.GetLength() - 1, ' ');//����ǷֺžͰѷֺ��ȱ�ɿո�Ȼ���ټ�һ���ֺţ�
			SQL += ";";
			finish = true;
		}
	}
	//�������дת��ΪСд
	SQL.MakeLower();
	//�����û�����
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create database����Ƿ���Ч
CString create_database(CString SQL, int start)
{
	CString temp;
	int index, end;
	//��ȡ����������
	while (SQL.GetAt(start) == ' ')
		start++;
	index = start;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.IsEmpty())
	{
		cout << "error: database name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.GetAt(start) == ' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL.GetAt(start) != ';' || start != SQL.GetLength() - 1)
		{
			wcout << "error12:" << SQL.Mid(index, SQL.GetLength() - index - 2).GetString() << "---is not a valid database name!" << endl;
			SQL = "99";
		}
		//����create database�����ڲ���ʽ
		else
			SQL = "00" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//�������
CString get_attribute(CString temp, CString sql)
{
	int start = 0, end, index;
	CString T, C;
	temp += " ";//Ϊ�˷���һ������������ʲôʱ�����
				//���������
	end = temp.Find(' ', start);
	T = temp.Mid(start, end - start);
	start = end + 1;
	sql += T + " ";
	//�����������
	while (temp.GetAt(start) == ' ')
		start++;
	end = temp.Find(' ', start);
	T = temp.Mid(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (T.IsEmpty())
	{
		cout << "error : error in create table statement!" << endl;
		sql = "99";
		return sql;
	}
	//��Ϊint 
	else if (T == "int")
		sql += "+";
	//��Ϊfloat
	else if (T == "float")
		sql += "-";
	//���� �����Ҫ����������char()�ˡ���
	else
	{
		index = T.Find('(');//������
		C = T.Left(index);//����Ӧ����char����ַ���
		index++;
		//�����󣬴�ӡ������Ϣ
		if (C.IsEmpty())
		{
			cout << "error: " << T << "---is not a valid data type definition!" << endl;
			sql = "99";
			return sql;
		}
		//��Ϊchar
		else if (C == "char")
		{
			C = T.Mid(index, T.GetLength() - index - 1);//char�ĳ���
			if (C.IsEmpty())
			{
				cout << "error: the length of the data type char has not been specified!" << endl;
				sql = "99";
				return sql;
			}
			else
				sql += C;
		}
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		else
		{
			cout << "error: " << C << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
	}
	//�Ƿ��и�����Ϣ �еĻ�Ҳ��һ��unique
	while (start<temp.GetLength() && temp.GetAt(start) == ' ')
		start++;
	if (start<temp.GetLength())
	{
		//��Ϊunique���壬������Ϣ
		end = temp.Find(' ', start);
		T = temp.Mid(start, end - start);
		if (T == "unique")
		{
			sql += " 1,";
		}
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		else
		{
			cout << "error: " << temp << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
	}
	//���޸�����Ϣ
	else
		sql += " 0,";
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create table����Ƿ���Ч
CString create_table(CString SQL, int start)
{
	CString temp, sql, T;
	int index, end, length;
	//��ȡ����
	while (SQL.GetAt(start) == ' ')//�ҵ���һ�����ǿո��λ��
		start++;
	index = start;//���index�����
	if ((end = SQL.Find('(', start)) == -1)//end�������ŵ�λ�ã�end==-1��ʾû�ҵ�������
	{
		cout << "error: missing ( in the statement!" << endl;
		SQL = "99";
		return SQL;
	}
	temp = SQL.Mid(start, end - start);//�ո���������м���ַ���
	start = end + 1;//����start
	if (!temp.IsEmpty())//���������������֮��Ŀո�
	{
		while (SQL.GetAt(start) == ' ')
			start++;
		length = temp.GetLength() - 1;//��һ����±ꡭ����ΪIndex�Ǵ�0��ʼ��
		while (temp.GetAt(length) == ' ')//lengthָ����������һλ
			length--;
		temp = temp.Left(length + 1);
	}
	//���ޣ���ӡ������Ϣ
	if (temp.IsEmpty())//����Ϊ��
	{
		cout << "error: error in create table statement!" << endl;
		SQL = "99";
		return SQL;
	}
	//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
	else if (temp.Find(' ') != -1)//��Ӧ�����ڱ����м��пո�
	{
		wcout << "error: " << temp.GetString() << "---is not a valid table name!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		sql = temp + ",";
		//��ȡÿ������
		while ((end = SQL.Find(',', start)) != -1)
		{
			temp = SQL.Mid(start, end - start);
			start = end + 1;
			//���п����ԣ���ӡ������Ϣ ��������һ���˲��ܳ���������ɡ���
			if (temp.IsEmpty())
			{
				cout << "error: error in create table statement!" << endl;
				SQL = "99";
				return SQL;
			}
			//��������
			else
			{
				sql = get_attribute(temp, sql);
				if (sql == "99")
					return sql;
			}
			while (SQL.GetAt(start) == ' ')
				start++;
		}
		//����������
		temp = SQL.Mid(start, SQL.GetLength() - start - 1);//���һλû��������������໣����һλ�Ƿֺţ���read_input�������趨�������һλ�϶��Ƿֺš��� ���Ǵ��ļ����������ô�졭��
		length = temp.GetLength() - 1;//��Ҫ�����length��index����
		while (temp.GetAt(length) != ')'&&length >= 0)//��Щ��Ƥ���û��������ź����ո񡭡�
			length--;
		//���ޣ���ӡ������Ϣ
		if (length<1)
		{
			cout << "error: error in create table statement!" << endl;
			SQL = "99";
			return SQL;
		}
		//�洢����
		else
		{
			temp = temp.Left(length);
			end = SQL.Find(' ', start);
			T = SQL.Mid(start, end - start);
			start = end + 1;
			//��Ϊ�������� �����һ�����Կ��ܲ��������Զ����������塭��
			if (T == "primary")
			{
				//�ж��Ƿ��йؼ���key
				temp += ")";//��Ӹ������Ǹ�ɶ����
				while (SQL.GetAt(start) == ' ')
					start++;
				end = SQL.Find('(', start);
				T = SQL.Mid(start, end - start);
				start = end + 1;
				length = T.GetLength() - 1;
				while (T.GetAt(length) == ' ')
					length--;
				T = T.Left(length + 1);
				//��Ϊ�գ���ӡ������Ϣ
				if (T.IsEmpty())
				{
					cout << "syntax error: syntax error in create table statement!" << endl;
					cout << "\t missing key word key!" << endl;
					SQL = "99";
					return SQL;
				}
				//���У�������֤
				else if (T == "key")
				{
					//��ȡ����������
					while (SQL.GetAt(start) == ' ')
						start++;
					end = SQL.Find(')', start);
					T = SQL.Mid(start, end - start);
					length = T.GetLength() - 1;
					while (T.GetAt(length) == ' ')
						length--;
					T = T.Left(length + 1);
					//���ޣ���ӡ������Ϣ
					if (T.IsEmpty())
					{
						cout << "error : missing primary key attribute name!" << endl;
						SQL = "99";
						return SQL;
					}
					//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
					else if (T.Find(' ') != -1)
					{
						cout << "error : " << T << "---is not a valid primary key attribute name!" << endl;
						SQL = "99";
						return SQL;
					}
					//��������
					else
					{
						sql += T + " #,";
						SQL = "01" + sql;
					}
				}
				//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
				else
				{
					cout << "error : " << T << "---is not a valid key word!" << endl;
					SQL = "99";
					return SQL;
				}
			}
			//��Ϊһ������
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
//��֤create index on����Ƿ���Ч
CString create_index_on(CString SQL, int start, CString sql)
{
	CString temp;
	int end, length;
	//��ȡ����
	while (SQL.GetAt(start) == ' ')
		start++;
	end = SQL.Find('(', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.IsEmpty())
	{
		cout << "syntax error: syntax error for create index statement!" << endl;
		cout << "\t missing ( !" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		//�����Ƿ�Ϊ��Ч�ļ���
		length = temp.GetLength() - 1;
		while (temp.GetAt(length) == ' ')
			length--;
		temp = temp.Left(length + 1);
		//��Ч
		if (temp.Find(' ') == -1)
		{
			sql += " " + temp;
			//��ȡ������
			while (SQL.GetAt(start) == ' ')
				start++;
			end = SQL.Find(')', start);
			temp = SQL.Mid(start, end - start);
			start = end + 1;
			//���ޣ���ӡ������Ϣ
			if (temp.IsEmpty())
			{
				cout << "syntax error: syntax error for create index statement!" << endl;
				cout << "\t missing ) !" << endl;
				SQL = "99";
				return SQL;
			}
			else
			{
				//�����Ƿ�Ϊ��Ч������
				length = temp.GetLength() - 1;
				while (temp.GetAt(length) == ' ')
					length--;
				temp = temp.Left(length + 1);
				//��Ч
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
					//����create index�����ڲ���ʽ
					else
						SQL = "02" + sql;
				}
				//��Ч,��ӡ������Ϣ
				else
				{
					wcout << "error:" << " " << temp.GetString() << "---is not a valid attribute name!" << endl;
					SQL = "99";
					return SQL;
				}
			}
		}
		//��Ч,��ӡ������Ϣ
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
//��֤create index����Ƿ���Ч
CString create_index(CString SQL, int start)
{
	CString temp, sql;
	int end;
	//��ȡ����������
	while (SQL.GetAt(start) == ' ')
		start++;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.IsEmpty())
	{
		cout << "syntax error: syntax error for create index statement!" << endl;
		SQL = "99";
	}
	else
	{
		sql = temp;
		//��ȡ���ĸ�����
		while (SQL.GetAt(start) == ' ')
			start++;
		end = SQL.Find(' ', start);
		temp = SQL.Mid(start, end - start);
		start = end + 1;
		//���ޣ���ӡ������Ϣ
		if (temp.IsEmpty())
		{
			cout << "syntax error: syntax error for create index statement!" << endl;
			SQL = "99";
		}
		//��Ϊon,������֤
		else if (temp == "on")
			SQL = create_index_on(SQL, start, sql);
		//��Ϊ�Ƿ���Ϣ����ӡ�Ƿ���Ϣ
		else
		{
			wcout << "syntax error:" << " " << temp.GetString() << "---is not a valid key word!" << endl;
			SQL = "99";
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create����Ƿ���Ч
CString create_clause(CString SQL, int start)
{
	CString temp;
	int end;
	//��ȡ�ڶ�������
	while (SQL.GetAt(start) == ' ')
		start++;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.IsEmpty())
	{
		cout << "syntax error: syntax error for create statement!" << endl;
		SQL = "99";
	}
	//��Ϊdatabase,������֤
	else if (temp == "database")
		SQL = create_database(SQL, start);
	//��Ϊtable,������֤
	else if (temp == "table")
		SQL = create_table(SQL, start);
	//��Ϊindex,������֤
	else if (temp == "index")
		SQL = create_index(SQL, start);
	//��Ϊ������Ϣ����ӡ������Ϣ
	else
	{
		cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//����create�����ڲ���ʽ
	return SQL;
}

CString drop_clause(CString SQL, int start)
{
	int end;
	CString temp;
	//��ȡ�ڶ�������
	while (SQL.GetAt(start) == ' ')
		start++;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	if (temp.IsEmpty())//�ڶ�������Ϊ�գ���ӡ������Ϣ
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
//��֤drop database����Ƿ���Ч
CString drop_database(CString SQL, int start)
{
	CString temp;
	int index, end;
	//��ȡ����������
	while (SQL.GetAt(start) == ' ')
		start++;
	index = start;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.IsEmpty())
	{
		cout << "error: database name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.GetAt(start) == ' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL.GetAt(start) != ';' || start != SQL.GetLength() - 1)//���һλ���Ƿֺţ����ߺ�߻�д�˶���Ķ���
		{
			wcout << "error:" << SQL.Mid(index, SQL.GetLength() - index - 2).GetString() << "---is not a valid database name!" << endl;//��2�Ǽ�ȥ�ֺźͿո�
			SQL = "99";
		}
		//����drop database�����ڲ���ʽ
		else
			SQL = "10" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop table����Ƿ���Ч
CString drop_table(CString SQL, int start)
{
	CString temp;
	int index, end;
	//��ȡ����������
	while (SQL.GetAt(start) == ' ')
		start++;
	index = start;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.IsEmpty())
	{
		cout << "error: table name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.GetAt(start) == ' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL.GetAt(start) != ';' || start != SQL.GetLength() - 1)//���һλ���Ƿֺţ����ߺ�߻�д�˶���Ķ���
		{
			wcout << "error:" << SQL.Mid(index, SQL.GetLength() - index - 2).GetString() << "---is not a valid table name!" << endl;//��2�Ǽ�ȥ�ֺźͿո�
			SQL = "99";
		}
		//����drop table�����ڲ���ʽ
		else
			SQL = "11" + temp;
	}

	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop index����Ƿ���Ч
CString drop_index(CString SQL, int start)
{
	CString temp;
	int index, end;
	//��ȡ����������
	while (SQL.GetAt(start) == ' ')
		start++;
	index = start;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//��ӡ������Ϣ
	if (temp.IsEmpty())
	{
		cout << "error: index name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.GetAt(start) == ' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL.GetAt(start) != ';' || start != SQL.GetLength() - 1)//���һλ���Ƿֺţ����ߺ�߻�д�˶���Ķ���
		{
			wcout << "error:" << SQL.Mid(index, SQL.GetLength() - index - 2).GetString() << "---is not a valid index name!" << endl;//��2�Ǽ�ȥ�ֺźͿո�
			SQL = "99";
		}
		//����drop index�����ڲ���ʽ
		else
			SQL = "12" + temp;
	}
	return SQL;
}

//��֤select ����Ƿ���Ч
CString select_clause(CString SQL, int start)//�ڶ����ʿ�����*��һ����������
{
	CString temp, sql, temp_sql;
	int end, length;
	CString from;
	from = "from";
	//��ȡselect��from֮�������
	while (SQL.GetAt(start) == ' ')
		start++;
	end = SQL.Find(from, start);
	if (end == -1)//�����û��from...
	{
		SQL = "99";
		cout << "syntax error: syntax error for select statement!" << endl;
		return SQL;
	}
	else//��from
	{
		temp = SQL.Mid(start, end - start);
		start = end + 1;
		while (SQL.GetAt(start) != ' ')
			start++;
		while (SQL.GetAt(start) == ' ')
			start++;
		//ɾһ��temp��from�м�Ŀո�
		length = temp.GetLength() - 1;
		while (temp.GetAt(length) == ' ')
			length--;
		temp = temp.Left(length + 1);

		//���ޣ���ӡ������Ϣ
		if (temp.IsEmpty())
		{
			cout << "syntax error: syntax error for select statement!" << endl;
			sql = "99";
		}
		else if (temp == "*")//select��ڶ�������* ��һselect��*�����ŵ���ô�졭��������һselect *Ȼ�������ء���
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
		else//����*��������Ӧ����һ�Ѷ��ŷָ���������������Ҳ�����Ǵ������롭��
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
			//�ӽ�catalog�����û����Щ����
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
//�����������ļ����ÿһ��,����Ϊ�ָ�
CString get_part(CString SQL)
{
	int start = 0, end, length;
	CString temp, sql;
	while ((end = SQL.Find(',', start)) != -1)//����Ϊ�ֽ�
	{
		temp = SQL.Mid(start, end - start);
		start = end + 1;

		length = temp.GetLength() - 1;//ɾ��temp�Ľ�β�ַ��Ͷ���֮��Ŀո�
		while (temp.GetAt(length) == ' ')
			length--;
		temp = temp.Left(length + 1);

		//���п����ԣ���ӡ������Ϣ
		if (temp.IsEmpty())
		{
			cout << "error: error in select statement!" << endl;
			SQL = "99";
			return SQL;
		}
		else
		{
			//��֤temp�Ƿ�Ϸ�
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
	//��ȡ���һ������
	temp = SQL.Mid(start, SQL.GetLength() - start);
	length = temp.GetLength() - 1;//ɾ��temp�Ľ�β�ַ��Ͷ���֮��Ŀո�
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
//����select from���
CString select_from(CString SQL, int start)
{
	CString sql, temp, temp_sql;
	int end, length;
	CString where_;
	where_ = "where";
	end = SQL.Find(where_, start);
	if (end == -1)//�����û��where,�ٿ���û�зֺ�
	{
		end = SQL.Find(';', start);
		if (end == -1)//û�зֺš���
		{
			SQL = "99";
			cout << "syntax error: syntax error for select statement!" << endl;
			return SQL;
		}
		else//�зֺ�,����֤һ�±����Ƿ�Ϸ�
		{
			temp = SQL.Mid(start, end - start);
			while (SQL.GetAt(start) != ' ')
				start++;
			while (SQL.GetAt(start) == ' ')
				start++;

			//��֤��������ɾ�ո�
			length = temp.GetLength() - 1;
			while (temp.GetAt(length) == ' ')
				length--;
			temp = temp.Left(length + 1);

			//���ޣ���ӡ������Ϣ
			if (temp.IsEmpty())
			{
				cout << "syntax error: syntax error for insert statement!" << endl;
				SQL = "99";
				return SQL;
			}
			else
			{
				if (temp.Find(' ') != -1)//��֤�����Ƿ���Ч �о���Ӧ����ô��飬Ӧ�ö���catalog���顭�����ȷ���
				{
					wcout << "error: " << temp.GetString() << "---is not a valid table name!" << endl;
					SQL = "99";
				}
				else
				{
					sql = temp + ".";//����
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
	else//��where
	{
		temp = SQL.Mid(start, end - start);
		start = end + 1;
		while (SQL.GetAt(start) != ' ')
			start++;
		while (SQL.GetAt(start) == ' ')
			start++;
		//		sql = get_part(temp);
		//��֤��������ɾ�ո�
		length = temp.GetLength() - 1;
		while (temp.GetAt(length) == ' ')
			length--;
		temp = temp.Left(length + 1);

		//���ޣ���ӡ������Ϣ
		if (temp.IsEmpty())
		{
			cout << "syntax error: syntax error for insert statement!" << endl;
			SQL = "99";
			return SQL;
		}
		else
		{
			if (temp.Find(' ') != -1)//��֤�����Ƿ���Ч �о���Ӧ����ô��飬Ӧ�ö���catalog���顭�����ȷ���
			{
				wcout << "error: " << temp.GetString() << "---is not a valid table name!" << endl;
				SQL = "99";
			}
			else
			{
				sql = temp + ".";//����
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

		//��������¼where����
		end = SQL.Find(';', start);//�ҵ��ֺ�
		if (end == -1)//û�зֺš���
		{
			SQL = "99";
			cout << "syntax error: syntax error for select statement!" << endl;
			return SQL;
		}
		else//�ҵ��˷ֺ�
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
//����where���
CString get_where(CString SQL)
{
	int start = 0, end, length;
	CString temp, sql, temp_sql;
	//andΪ�ֽ硭����һĳ���ַ��������and��զ���ء���
	CString and;
	and = "and";

	while ((end = SQL.Find(and, start)) != -1)//andΪ�ֽ磬ѭ�� ��Ⱑ�������������ҵĲ���and����a������
	{
		temp = SQL.Mid(start, end - start);
		start = end + 1;
		while (SQL.GetAt(start) != ' ')
			start++;
		while (SQL.GetAt(start) == ' ')
			start++;

		length = temp.GetLength() - 1;//ɾ��temp�Ľ�β�ַ���and֮��Ŀո�
		while (temp.GetAt(length) == ' ')
			length--;
		temp = temp.Left(length + 1);
		//Ȼ��Ӧ�ö���һ���������ȥ�ж�where��ÿһ��Բ��ԡ���
		sql += where_clause(temp);
		sql += '&';

	}
	//���һ������
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
//����where����ÿ���Ӿ�
CString where_clause(CString SQL)
{
	int start = 0, end, length;
	CString temp, op, sql;
	int op_start = start, op_end;
	length = SQL.GetLength();
	//�ҵ��������λ��
	while (SQL.GetAt(op_start) != '='&&SQL.GetAt(op_start) != '<'&&SQL.GetAt(op_start) != '>')
	{
		if (op_start == length - 1)
			break;
		op_start++;
	}

	if (op_start == length - 1)//û�ҵ������
	{
		SQL = "99";
		cout << "error: no operator in where statement!" << endl;
		return SQL;
	}
	else//�ҵ������������ʱop_start���������һλ���±�
	{
		//�ж�һ���������ʲô
		op_end = op_start + 1;
		if (SQL.GetAt(op_start) == '<' && (SQL.GetAt(op_end) == '=' || SQL.GetAt(op_end) == '>'))//�����һλ��С�ںţ�������һλ�ǲ��ǵȺŻ���ں�
		{
			op_end++;
		}
		if (SQL.GetAt(op_start) == '>'&&SQL.GetAt(op_end) == '=')//�����һλ�Ǵ��ںţ�����һλ�ǲ��ǵȺ�
		{
			op_end++;
		}
		op = SQL.Mid(op_start, op_end - op_start);//��ȡ�����
		temp = SQL.Mid(start, op_start - start);//��ȡ������������

		length = temp.GetLength() - 1;//ɾ��temp�Ľ�β�ַ��������֮��Ŀո�
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
			//Ȼ��Ӧ��ȥcatalog�����֤һ���Ƿ�Ϸ�
			sql = temp;
			//������Ľ���������
			if (op_end - op_start == 2)//��λ�������ֱ�Ӵ�
			{
				sql += op;
			}
			else if (op_end - op_start == 1)//һλ�Ĵ��һ���ո���������
			{
				sql = sql + ' ' + op;
			}

			//��������ֵ��
			//�о��������Ӧ�ö���catalog��߿�����Ӧ����ʲô����
			//�Ȳ����ˡ���
			start = op_end;
			while (SQL.GetAt(start) == ' ')
				start++;
			end = SQL.GetLength();
			temp = SQL.Mid(start, end - start);


			//Ȼ�����Ƿ���������ˡ���


			if (temp.IsEmpty())//�յ�
			{
				cout << "error: error in where statement!" << endl;
				SQL = "99";
				return SQL;
			}
			else//���գ�Ҫ�ж���ɶ����
			{
				length = temp.GetLength() - 1;//ɾ���ո�
				while (temp.GetAt(length) == ' ')
					length--;
				temp = temp.Left(length + 1);

				if (temp.GetAt(0) == '\'')//char���ͣ�����֤��ͷ��β���ǵ�����
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
				else if (temp.Find('.') != -1)//float���ͣ�����֤����С����֮����ַ�ȫ������
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
				else//int���ͣ�����֤ÿһλ��������...����֤��λ����0�𣬺����ðɡ���
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
//��֤insert ����Ƿ���Ч�����ڶ��������ǲ���into,�Լ������Ƿ���Ч��
CString insert_clause(CString SQL, int start)
{
	CString temp, sql;
	int end;
	//��ȡ�ڶ�������
	while (SQL.GetAt(start) == ' ')
		start++;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.IsEmpty())
	{
		cout << "syntax error: syntax error for insert statement!" << endl;
		SQL = "99";
	}
	else if (temp == "into")
	{
		//��ȡ����������
		while (SQL.GetAt(start) == ' ')
			start++;
		end = SQL.Find(' ', start);
		temp = SQL.Mid(start, end - start);
		start = end + 1;
		//���ޣ���ӡ������Ϣ
		if (temp.IsEmpty())
		{
			cout << "syntax error: syntax error for insert statement!" << endl;
			SQL = "99";
		}
		else
		{
			if (temp.Find(' ') != -1)//��֤�����Ƿ���Ч
			{
				wcout << "error: " << temp.GetString() << "---is not a valid table name!" << endl;
				SQL = "99";
			}
			else
			{
				sql = temp + ",";//����
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
//��֤insert into values����Ƿ���Ч
CString insert_into_values(CString SQL, int start, CString sql)
{
	int end, length;
	CString temp;
	//�ȼ����һ�������ǲ���values
	while (SQL.GetAt(start) == ' ')
		start++;
	end = SQL.Find('(', start);
	if (end == -1)//û��������
	{
		cout << "error: missing ( in the statement!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		temp = SQL.Mid(start, end - start);
		start = end + 1;
		//��ɾ��values��������֮��Ŀո�
		length = temp.GetLength() - 1;
		while (temp.GetAt(length) == ' ')
			length--;
		temp = temp.Left(length + 1);

		if (temp == "values")
		{
			while (SQL.GetAt(start) == ' ')
				start++;
			//��ȡ�����һ���������ÿ������
			while ((end = SQL.Find(',', start)) != -1)//����Ϊ�ֽ�
			{
				temp = SQL.Mid(start, end - start);
				start = end + 1;

				length = temp.GetLength() - 1;//ɾ��temp�Ľ�β�ַ��Ͷ���֮��Ŀո�
				while (temp.GetAt(length) == ' ')
					length--;
				temp = temp.Left(length + 1);

				//���п����ԣ���ӡ������Ϣ
				if (temp.IsEmpty())
				{
					cout << "error: error in create table statement!" << endl;
					SQL = "99";
					return SQL;
				}
				//��������
				else
				{
					if (temp.GetAt(0) == '\'')//char���ͣ�����֤��ͷ��β���ǵ�����
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
					else if (temp.Find('.') != -1)//float���ͣ�����֤����С����֮����ַ�ȫ������
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
					else//int���ͣ�����֤ÿһλ��������...����֤��λ����0�𣬺����ðɡ���
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
			//��ȡ���һ������
			temp = SQL.Mid(start, SQL.GetLength() - start - 1);
			length = temp.GetLength() - 1;
			while (temp.GetAt(length) != ')'&&length >= 0)//ɾ���ֺź�������֮��Ŀո�
				length--;
			//���ޣ���ӡ������Ϣ
			if (length<1)
			{
				cout << "error: error in insert statement!" << endl;
				SQL = "99";
				return SQL;
			}
			//��֤����
			else
			{
				temp = temp.Left(length);
				if (temp.GetAt(0) == '\'')//char���ͣ�����֤��ͷ��β���ǵ�����
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
				else if (temp.Find('.') != -1)//float���ͣ�����֤����С����֮����ַ�ȫ������
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
				else//int���ͣ�����֤ÿһλ��������...����֤��λ����0�𣬺����ðɡ���
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
		else//����Ĵʲ���values
		{
			cout << "error: error in insert statement!" << endl;
			SQL = "99";
			return SQL;
		}
	}
	//Ӧ���������ӵ�catalog����ټ��һ��
	SQL = "30" + sql;
	return SQL;
}


//��֤delete����Ƿ���Ч
CString delete_clause(CString SQL, int start)
{
	//���һ�µڶ������ǲ���from
	CString temp, sql, temp_sql;
	int end, length;
	//��ȡ�ڶ�������
	while (SQL.GetAt(start) == ' ')
		start++;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.IsEmpty())
	{
		cout << "syntax error: syntax error for delete statement!" << endl;
		SQL = "99";
	}
	else if (temp == "from")//�ڶ�������from,���ż����������ǲ��ǺϷ�����
	{
		//��ȡ����������
		while (SQL.GetAt(start) == ' ')
			start++;
		end = SQL.Find(' ', start);
		temp = SQL.Mid(start, end - start);
		start = end;
		//���ޣ���ӡ������Ϣ
		if (temp.IsEmpty())
		{
			cout << "syntax error: syntax error for insert statement!" << endl;
			SQL = "99";
			return SQL;
		}
		else
		{
			if (temp.Find(' ') != -1)//��֤�����Ƿ���Ч �о���Ӧ����ô��飬Ӧ�ö���catalog���顭�����ȷ���
			{
				wcout << "error: " << temp.GetString() << "---is not a valid table name!" << endl;
				SQL = "99";
			}
			else
			{
				sql = temp + ",";//����
								 //Ȼ���±���where���Ƿֺ�
				CString where_;
				where_ = "where";
				end = SQL.Find(where_, start);

				if (end == -1)//û��where,��ôֱ����֤��һ�����ǲ��Ƿֺžͺ��ˡ���
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
				else //��where
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
//��֤ delete from where ����Ƿ���Ч
CString delete_from_where(CString SQL, int start, CString sql)
{
	int end;
	CString temp, temp_sql;
	//��������¼where����
	while (SQL.GetAt(start) != ' ')
		start++;
	while (SQL.GetAt(start) == ' ')
		start++;
	end = SQL.Find(';', start);//�ҵ��ֺ�
	if (end == -1)//û�зֺš���
	{
		SQL = "99";
		cout << "syntax error: syntax error for delete statement!" << endl;
		return SQL;
	}
	else//�ҵ��˷ֺ�
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
	//��ȡ�ڶ�������
	while (SQL.GetAt(start) == ' ')
		start++;
	index = start;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.IsEmpty())
	{
		cout << "error: database name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.GetAt(start) == ' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL.GetAt(start) != ';' || start != SQL.GetLength() - 1)
		{
			wcout << "error12:" << SQL.Mid(index, SQL.GetLength() - index - 2).GetString() << "---is not a valid database name!" << endl;
			SQL = "99";
		}
		//����use�����ڲ���ʽ
		else
			SQL = "50" + temp;
	}
	return SQL;
}

CString execfile_clause(CString SQL, int start)
{
	CString temp;
	int index, end;
	//��ȡ�ڶ�������
	while (SQL.GetAt(start) == ' ')
		start++;
	index = start;
	end = SQL.Find(' ', start);
	temp = SQL.Mid(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.IsEmpty())
	{
		cout << "error: file name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.GetAt(start) == ' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL.GetAt(start) != ';' || start != SQL.GetLength() - 1)
		{
			wcout << "error12:" << SQL.Mid(index, SQL.GetLength() - index - 2).GetString() << "---is not a valid file name!" << endl;
			SQL = "99";
		}
		//����use�����ڲ���ʽ
		else
			SQL = "60" + temp;
	}
	return SQL;
}//�˴����ɣ��ļ����Ƿ���Ҫ�����š���
CString quit_clause(CString SQL, int start)
{
	int end, index;

	while (SQL.GetAt(start) == ' ')
		start++;
	index = start;
	//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
	if (SQL.GetAt(start) != ';' || start != SQL.GetLength() - 1)//���һλ���Ƿֺţ����ߺ�߻�д�˶���Ķ���
	{
		wcout << "error:" << SQL.Mid(index, SQL.GetLength() - index - 2).GetString() << "---is not a valid quit clause!" << endl;//��2�Ǽ�ȥ�ֺźͿո�
		SQL = "99";
	}
	//����quit�����ڲ���ʽ
	else
		SQL = "70";

	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��ȡ�û����룬������������Ч�Լ�飬����ȷ�����������ڲ���ʾ��ʽ
CString Interpreter(CString statement)
{
	CString SQL;
	CString temp;
	CString sql;
	int start = 0, end;
	if (statement.IsEmpty())//���statement�ǿյľ����û���������
							//��ȡ�û�����
		SQL = read_input();
	else
		SQL = statement;
	//��ȡ����ĵ�һ������
	while (SQL.GetAt(start) == ' ')//�ҵ���һ�����ǿո���ַ�
		start++;
	end = SQL.Find(' ', start);//��start��ʼ�ң���һ���ǿո��λ�ã����start��Ϊ0�򲻰���start��
	temp = SQL.Mid(start, end - start);//�պ��ǵ�һ������
	start = end + 1;//����������м�ֻ��һ���ո�Ļ������ڵ�startӦ���ǵڶ������ʵĵ�һ����ĸ
					//�������룬��ӡ������Ϣ
	if (temp.IsEmpty())
	{
		cout << "syntax error: empty statement!" << endl;
		SQL = "99";
	}
	//��Ϊcreate���
	else if (temp == "create")
		SQL = create_clause(SQL, start);
	//��Ϊdrop���
	else if (temp == "drop")
		SQL = drop_clause(SQL, start);

	//��Ϊselect���
	else if (temp == "select")
		SQL = select_clause(SQL, start);

	//��Ϊinsert���
	else if (temp == "insert")
		SQL = insert_clause(SQL, start);
	//��Ϊdelete���
	else if (temp == "delete")
		SQL = delete_clause(SQL, start);
	//��Ϊuse���
	else if (temp == "use")
		SQL = use_clause(SQL, start);
	//��Ϊexecfile���
	else if (temp == "execfile")
		SQL = execfile_clause(SQL, start);
	//��Ϊquit���
	else if (temp == "quit")
		SQL = quit_clause(SQL, start);
	//��ȡ����
	else if (temp == "help")
		SQL = "80";
	//��Ϊ�Ƿ����
	else
	{
		wcout << "syntax error:" << " " << temp.GetString() << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//�������������ڲ���ʽ
	return SQL;
}
