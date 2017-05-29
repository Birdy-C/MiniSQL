#include"API_Module.h"
#include"Catalog_Manager.h"
#include"Record_Manager.h"
#include"Index_Manager.h"

using namespace std;

CString DB_Name;
/*
����Interpreter��������ɵ������ڲ���ʽ������ŷֱ����Catalog Manager�ṩ�Ľӿڽ�������������֤��
�����ݽ������Record Manager,Index Manager�ṩ�Ľӿ�ִ�и���䣬
ͬʱҲΪRecord Manager ģ�����Index Managerģ��Ĺ����ṩ�ӿڡ�
�Ŷ���֮��API��Interpreterģ�飬Record Managerģ�鼰Catalog Managerģ����й�ͨ����Ŧ��
����֮��Ľ���һ�㶼����APIģ��������ת�Ӳ��ܽ����໥֮��Ĺ��ܵ��á�
��ˣ���ȻAPIģ����ʵ�ֵĹ���ʮ�����ޣ����Ǹ�ģ��ȴ������ϵͳ�ĺ��ġ�
*/
/*
ͨ��APIģ��Ĺ��ܣ�ʹ��Interpreterģ�飬Catalog Managerģ�飬Record Managerģ���Index Managerģ��֮��ʵ�����໥ͨ�ţ�
Ϊģ��֮���Э�������ṩ����Ч�Ľӿڡ�
*/
/*
CString Left( int nCount ) const;
����ȡ�ִ�

����
csStr="abcdef";
cout<<csStr.Left(3);
//abc
//��nCount����0ʱ�����ؿա�
//��nCountΪ����ʱ�����ؿա�
//��nCount���ڶ��󳤶�ʱ������ֵ�������ͬ��


CString Right( int nCount ) const;
����ȡ�ִ�

����
csStr="abcdef";
cout<<csStr.Right(3);
//def
//��nCount����0ʱ�����ؿա�
//��nCountΪ����ʱ�����ؿա�
//��nCount���ڶ��󳤶�ʱ������ֵ�������ͬ��
*/
void API_Module(CString SQL)
{
	CString Type, Attr, Index_Name, Table_Name, Attr_Name, Condition, index_name[32], File;
	int index, end, length, offset, type, count, num, record_Num, Count;
	//ʮ����֪�������count��ʲô���ã�ԭʼ�����������õ�������ֱ���õģ�
	//Ҳ�������Եĸ�����������һ��Count��������where�����ĸ������˴������
	//�����һ����������û�н��й���ʼ����
	//�˴������
	index_info nodes[32];
	condition_info conds[10];
	attr_info print[32];
	char cond;
	bool ok;
	Type = SQL.Left(2);//Type�Ǻ���İ���
	SQL = SQL.Mid(2, SQL.GetLength() - 2);//�����￪ʼSQL������������

										  /////////////////////////////////////////////////////////////////////////////////////////
										  //�������ݿ�
	if (Type == "00")
		Create_Database(SQL);//�˴��ǵ���Catalog_Manager���Create_Database()����

							 /////////////////////////////////////////////////////////////////////////////////////////
							 //�������ݱ�Create_Table
	else if (Type == "01")
	{
		if (DB_Name.IsEmpty())
			cout << "error: you have not specified the database to be used!" << endl;
		else
		{
			index = SQL.Find(',');
			Table_Name = SQL.Left(index);
			Attr = SQL.Right(SQL.GetLength() - index - 1);//�˴���Attr��Ȼ����һ������������Ϣ
			Attr_Name.Empty();
			//�������ݱ�
			Create_Table(Table_Name, Attr, DB_Name, Attr_Name);//�˴��ǵ���Catalog_Manager���Create_Table()����
															   //�ж��Ƿ񴴽���������
			if (!Attr_Name.IsEmpty())
				Create_Index(Table_Name, Table_Name, Attr_Name, DB_Name, length, offset, type); //�˴��ǵ���Catalog_Manager���Create_Index()����
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//��������Create_Index
	else if (Type == "02")
	{
		if (DB_Name.IsEmpty())
			cout << "error: you have not specified the database to be used!" << endl;
		else
		{
			index = SQL.Find(' ');
			//��ȡ������
			Index_Name = SQL.Left(index);
			index++;
			end = SQL.Find(' ', index);
			//��ȡ����
			Table_Name = SQL.Mid(index, end - index);
			//��ȡ������
			Attr = SQL.Right(SQL.GetLength() - end - 1);
			Create_Index(Index_Name, Table_Name, Attr, DB_Name, length, offset, type);//�˴��ǵ���Catalog_Manager���Create_Index()����
																					  //�������������ڵ�
			if (length != -1)
				Insert_Index_All(DB_Name, Table_Name, Index_Name, length, offset, type);//�˴��ǵ���Catalog_Manager���Insert_Index_All()����
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//ɾ�����ݿ�
	else if (Type == "10")
	{
		if (SQL == DB_Name)
		{
			//�ͷ����ݿ���ڴ�
			Close_Database(DB_Name, false);//�˴��ǵ���Record_Manager���Close_Database()����
										   //Buffer_Manager������ƺ�����Сд����ʽ
			DB_Name.Empty();
		}
		Drop_Database(SQL);//�˴��ǵ���Catalog_Manager���Drop_Database()����
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//ɾ�����ݱ�
	else if (Type == "11")
	{
		if (DB_Name.IsEmpty())
			cout << "error: you have not specified the database to be used!" << endl;
		else
		{
			Table_Name = SQL;
			//�ͷű���ڴ�
			Close_File(DB_Name, Table_Name, 0, false);//�˴��ǵ���Record_Manager���Close_File()����
													  //ɾ�����ļ�
			Drop_Table(Table_Name, DB_Name, index_name, count);//�˴��ǵ���Catalog_Manager���Drop_Table()����
															   //�ͷű��������������ڴ�
			for (index = 0; index < count; index++)
				Close_File(DB_Name, index_name[index], 1, false);//�˴��ǵ���Record_Manager���Close_File()����
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//ɾ������
	else if (Type == "12")
	{
		if (DB_Name.IsEmpty())
			cout << "error: you have not specified the database to be used!" << endl;
		else
		{
			Index_Name = SQL;
			//ɾ���������ڴ�
			Close_File(DB_Name, Index_Name, 1, false);//�˴��ǵ���Record_Manager���Close_File()����
			Drop_Index(Index_Name, DB_Name);//�˴��ǵ���Catalog_Manager���Drop_Index()����
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//ѡ�����(��where�Ӿ�)
	//���������ԭʼ������ģ�����ԭʼ����ʱ��������whereϸ�ֵ�Type����
	//���������е�InterpreterӦ�ò�����������
	//����TypeΪ20����select * ���
	//��������where��������ͨ���жϽ�β�ǲ���.#������Ӧ��Type�������ϸ��
	//Ŀǰ���뵽���ж��ַ�����������SQL.Right(2)=='.#'?
	//����������������������where������պ�Ҳ����'.#'��β�Ŀ�զ���� �˴������

	/////////////////////////////////////////////////////////////////////////////////////////
	/*
	select * :"20." + "����1,����2,...,����n." + "#��where����"
	select(������) : "21,������1��������2��...,������n." + "����1,����2,...,����n." + "#��where����"
	#��ʾ��select����в���where���
	where������
	&��ʾand �����������
	��λ�������(>= <= <>)�������Ǳ���һλ���������< > = ����һ���ո�������Ǳ���
	*/
	/*
	ѡ�����
	SELECT * FROM ������   SELECT ��1����2����.��n FROM ������
	��WHERE����������Ա��ļ����б���������ÿ����¼������ѡ���������飬��ӡ����Ӧ������ֵ��

	SELECT * FROM ���� WHERE ������ SELECT ��1����2������n FROM ���� WHERE ����
	���޿��õ�����������Ա��ļ����б���������ÿ����¼������WHERE ����ƥ�䣬�����ϣ����ӡ��Ӧ���Ե�ֵ��
	���п��õ���������ɵ���Index Manaterģ��Ĺ��ܣ����ҷ���WHERE �����ļ�¼������ӡ��Ӧ���Ե�ֵ��������ӡ����ѡ��ļ�¼��������
	*/


	//ѡ����䣨select * ��ʽ��
	else if (Type == "20")
	{
		if (DB_Name.IsEmpty())
			cout << "error: you have not specified the database to be used!" << endl;
		else if (SQL.Right(2) == '.#')//ѡ����䣨select * ��ʽ����where�Ӿ䣩
		{
			/*�˴�Ϊԭʼ�����еĴ��룬���ο�
			//ѡ�����(��where�Ӿ�)
			//��ȡѡ��������
			index = SQL.Find(',');
			Attr = SQL.Left(index);
			index++;
			//��ȡ����
			Table_Name = SQL.Right(SQL.GetLength() - index);
			Table_Name = Table_Name.Left(Table_Name.GetLength() - 1);
			if (Table_Name.Find(' ') != -1)
			cout << "error: can not select from more than one table!" << endl;
			else
			{
			//��ȡ��ʾ��¼��ʽ
			if (Attr == "*")
			Get_Attr_Info_All(DB_Name, Table_Name, print, count);
			else
			Get_Attr_Info(DB_Name, Table_Name, print, count, Attr);//select����
			if (count != 0)
			Select_No_Where(DB_Name, Table_Name, print, count);
			}
			*/
			index = SQL.Find('.');//����������
			Table_Name = SQL.Left(index);//������һ������������Ҫȫ���ѿ����������𡣡����˴������
										 //�õĲ��õģ���Ϊ��Ҫ�������ѽ��
			if (Table_Name.Find(',') != -1)
				cout << "erroe: can not select from more than one table!" << endl;
			else
			{
				//�Ա��ļ����б���������ÿ����¼������ѡ���������飬��ӡ����Ӧ������ֵ��
				Get_Attr_Info_All(DB_Name, Table_Name, print, count);//�˴��ǵ���Catalog_Manager���Get_Attr_Info_All()����
				if (count != 0)
					Select_No_Where(DB_Name, Table_Name, print, count);
			}
		}
		else if (SQL.Right(2) != '.#')//ѡ����䣨select * ��ʽ����where�Ӿ䣩
		{
			//��ȡ����
			SQL = SQL.Mid(1, SQL.GetLength() - 1);//����SQL����Ϊ20���и�'.'
			index = SQL.Find('.');//����������
			Table_Name = SQL.Left(index);
			//�����������¼where������Conditionû�����κκ����г��֣�Ӧ����Ҫ��ôת��һ�°ɣ��˴������
			//����Ϊ�ѽ������Ҫ����Catalog_Manager�е�Get_Condition()����
			index = index + 1;
			Condition = SQL.Right(SQL.GetLength() - index);//where����
														   //ת��������
			Get_Condition(DB_Name, Table_Name, Condition, conds, count, cond, nodes, num);//�˴��ǵ���Catalog_Manager�е�Get_Condition()����

			if (Table_Name.Find(',') != -1)
				cout << "erroe: can not select from more than one table!" << endl;
			else//����һϵ�к���ʵ��select * where
			{
				//�������޿�������
				if (Find_Useful_Cond(conds, count, index) == 0)//�޿�������
				{
					//�Ա��ļ����б���
					Get_Attr_Info_All(DB_Name, Table_Name, print, count);//�˴��ǵ���Catalog_Manager���Get_Attr_Info_All()����
																		 //��ÿ����¼������WHERE ����ƥ�䣬�����ϣ����ӡ��Ӧ���Ե�ֵ��
					if (count != 0)//���ﲻ֪����ʲô��˼����֪���ò��üӣ��˴������
						Select_With_Where(DB_Name, Table_Name, conds, count, cond, print, Count);//�˴��ǵ���Record_Manager���Select_With_Where()����
				}
				else//�п�������
				{
					//����Index Managerģ��Ĺ���
					//���ҷ���WHERE �����ļ�¼������ӡ��Ӧ���Ե�ֵ��������ӡ����ѡ��ļ�¼��������
					//���ݲ�ͬ��where������������ĺ��������ﻹ��Ҫϸ��һ�£��˴������
					//�о�����Ӧ���Ƿ�һ��Record_Manager��������Select_With_Where�����ĺ�����
					//����ĵ�����������Select_With_Where�ڲ��������г��ĸ��ֺ�����ɡ�
					//����֮���ٸģ����������

					//�õ���������ѡ�����
					Select_With_Equal_Cond(DB_Name, Table_Name, conds, count, print, Count, cond, Index);
					//�ô��ڻ���ڵ���������ѡ�����
					Select_With_Greater_Cond(DB_Name, Table_Name, conds, count, print, Count, cond, Index, type);
					//��С�ڻ�С�ڵ���������ѡ�����
					Select_With_Smaller_Cond(DB_Name, Table_Name, conds, count, print, Count, cond, Index, type);

					if (count != 0)
						Select_Without_Useful_Cond(DB_Name, Table_Name, conds, count, print, Count, cond, index);//�˴��ǵ���Record_Manager���Select_Without_Useful_Cond()����
				}

			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//ѡ����䣨select(������) ��ʽ��
	else if (Type == "21")
	{
		if (DB_Name.IsEmpty())
			cout << "error: you have not specified the database to be used!" << endl;
		else if (SQL.Right(2) == '.#')//ѡ����䣨select (����) ��ʽ����where�Ӿ䣩
		{
			//��ȡѡ��������
			SQL = SQL.Mid(1, SQL.GetLength() - 1);//����SQL����Ϊ21���и�','
			index = SQL.Find('.');//���Խ�����
			Attr = SQL.Left(index);
			index++;
			//��ȡ����
			Table_Name = SQL.Right(SQL.GetLength() - index);
			Table_Name = Table_Name.Left(Table_Name.GetLength() - 2);//ȥ��'.#'
			if (Table_Name.Find(',') != -1)
				cout << "error: can not select from more than one table!" << endl;
			else
			{
				//�Ա��ļ����б���
				Get_Attr_Info(DB_Name, Table_Name, print, count, Attr);//�˴��ǵ���Catalog_Manager���Get_Attr_Info()����
				if (count != 0)
					Select_No_Where(DB_Name, Table_Name, print, count);
			}
		}
		else if (SQL.Right(2) != '.#')//ѡ����䣨select (����) ��ʽ����where�Ӿ䣩
		{
			//��ȡ������
			SQL = SQL.Mid(1, SQL.GetLength() - 1);//����SQL����Ϊ21���и�','
			index = SQL.Find('.');
			Attr = SQL.Left(index);
			//��ȡ����
			SQL = SQL.Mid(index + 1, SQL.GetLength() - index - 1);
			index = SQL.Find('.');
			Table_Name = SQL.Left(index);
			//��ȡwhere����
			SQL = SQL.Mid(index + 1, SQL.GetLength() - index - 1);
			Condition = SQL;
			//ת��������
			Get_Condition(DB_Name, Table_Name, Condition, conds, count, cond, nodes, num);//�˴��ǵ���Catalog_Manager�е�Get_Condition()����

			if (Table_Name.Find(',') != -1)
				cout << "error: can not select from more than one table!" << endl;
			else//����һϵ�к���ʵ��select (����) where
			{
				//�������޿�������
				if (Find_Useful_Cond(conds, count, index) == 0)//�޿�������
				{
					//�Ա��ļ����б���
					Get_Attr_Info(DB_Name, Table_Name, print, count, Attr);//�˴��ǵ���Catalog_Manager���Get_Attr_Info()����
																		   //��ÿ����¼������WHERE ����ƥ�䣬�����ϣ����ӡ��Ӧ���Ե�ֵ��
					if (count != 0)//���ﲻ֪����ʲô��˼����֪���ò��üӣ��˴������
						Select_Without_Useful_Cond(DB_Name, Table_Name, conds, count, print, Count, cond);//�˴��ǵ���Record_Manager���Select_Without_Useful_Cond()����
				}
				else//�п�������
				{
					//�õ���������ѡ�����
					Select_With_Equal_Cond(DB_Name, Table_Name, conds, count, print, Count, cond, Index);
					//�ô��ڻ���ڵ���������ѡ�����
					Select_With_Greater_Cond(DB_Name, Table_Name, conds, count, print, Count, cond, Index, type);
					//��С�ڻ�С�ڵ���������ѡ�����
					Select_With_Smaller_Cond(DB_Name, Table_Name, conds, count, print, Count, cond, Index, type);

					if (count != 0)
						Select_Without_Useful_Cond(DB_Name, Table_Name, conds, count, print, Count, cond, index);//�˴��ǵ���Record_Manager���Select_Without_Useful_Cond()����
				}

			}
		}
	}

	//�������ɾ���ˡ�

	/////////////////////////////////////////////////////////////////////////////////////////
	//�������
	//insert:"30" + "table_name" + "��������" + "��������," + ... + "��������,"
	//�������� : +: int - : float / : char
	/*
	����Catalog Manager�������ɵĳ����ڲ�������ʽ����ȡ��������¼��
	������Ϣ��������¼�Ŀ�ţ�������Buffer Manager �Ĺ��ܣ�
	��ȡָ�����ڴ�飬������¼���뵽�ڴ��У�ͬʱ�޸���λ������Ϣ���ɡ�
	*/
	else if (Type == "30") {
		//��ȡ����
		//������ҪInterpreter����tablename�����һ����.��
		index = SQL.Find('.');
		Table_Name = SQL.Left(index);
		//��ȡ��������
		Attr = SQL.Mid(index + 1, SQL.GetLength() - index - 1);
		//����в���Ԫ��
		Insert_Item(DB_Name, Table_Name, Attr, record_Num);//�˴���Record_Manager�е�Insert_Item()����
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//ɾ�����
	//delete: "40"+"����,"+"where��������ʾ����ͬselect��"���еĻ��� ��û�еĻ���#��β��
	else if (Type == "40") {
		//���ｫ��ͬ��select�Ĵ��룬ֱ��������ķ�ʽ
		if (DB_Name.IsEmpty())
			cout << "error: you have not specified the database to be used!" << endl;
		else if (SQL.Right(2) == ',#')//ɾ����䣨��where�Ӿ䣩
		{
			//��ȡ����
			index = SQL.Find(',');
			Table_Name = SQL.Left(index);

			if (Table_Name.Find(',') != -1)
				cout << "error: can not select from more than one table!" << endl;
			else
				//ɾ����������Ԫ��
				Empty_Table(DB_Name, Table_Name, index_name, count);//�����ǵ���Catalog_Manager�����е�Empty_Table()����
		}
		else//ɾ����䣨��where�Ӿ䣩
		{
			//��ȡ����
			index = SQL.Find(',');
			Table_Name = SQL.Left(index);
			//��ȡwhere����
			SQL = SQL.Mid(index + 1, SQL.GetLength() - index - 1);
			Condition = SQL;
			//ת��������
			Get_Condition(DB_Name, Table_Name, Condition, conds, count, cond, nodes, num);//�˴��ǵ���Catalog_Manager�е�Get_Condition()����

			if (Table_Name.Find(',') != -1)
				cout << "error: can not select from more than one table!" << endl;
			else
				//ɾ����䣨��where��
				Delete_With_Where(DB_Name, Table_Name, conds, count, nodes, num, cond);//�����ǵ���Record_Manager���Delete_With_Where����
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//use ���
	//use:"50"+"database_name"
	else if (Type == "50") {
		DB_Name = SQL;
		Use_Database(DB_Name);//�˴��ǵ���Catalog_Manager�����е�Use_Database()����
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//ִ���ļ����
	//	execfile:"60"+"file_name" //���ɣ���֪���ļ����Ƿ�Ӧ�ü�����
	else if (Type == "60") {
		File = SQL;
		Exect_File(File);//�˴��ǵ���Catalog_Manager�����е�Exect_File()����
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//�˳�
	else if (Type == "70") {
		DB_Name = SQL;
		Quit(DB_Name);//�˴��ǵ���Record_Manager�е�Quit()����
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//����
	else if (Type == "80") {
		Help();//�˴��ǵ���API_Module�е�Help()����
	}

}

//��ʾ������Ϣ
void Help()
{
	printf("call API_Module->Help();\n");
	printf("��ʾ������Ϣ\n");
}

//���������ڵ�
void Insert_Index(CString DB_Name, CString Table_Name, index_info & index)
{
	printf("call API_Module->Insert_Index(...);\n");
	printf("���������ڵ�\n");
}
//ɾ�������ڵ�
void Delete_Index(CString DB_Name, CString Table_Name, index_info & index)
{
	printf("call API_Module->Delete_Index(...);\n");
	printf("ɾ�������ڵ�\n");
}
//���������ڵ�
void Find_Index(CString DB_Name, CString Table_Name, index_info & index)
{
	printf("call API_Module->Find_Index(...);\n");
	printf("���������ڵ�\n");
}
//��ȡ������������Ϣ
void Get_Index(CString DB_Name, CString Table_Name, int & start, int & end, int type, index_info & index)
{
	printf("call API_Module->Get_Index(...);\n");
	printf("��ȡ������������Ϣ\n");
}
