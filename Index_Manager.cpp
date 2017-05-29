//////////////////////////////////////////////////////////
///----------------------------------------------------///
///       Module: Index_Manager.cpp												///
///       Produced by: Birdy & C													///
///       Description: produce index for the project create index for buffer	///
///       date:2017/5/22                             ///
///----------------------------------------------------///
///////////////////////////////////////////////////////////

#include "Index_Manager.h"
#include "Buffer_Manager.h"
using namespace std;
/*
//1������һ��ֵ����inform.value(informΪinfo�ṹ��)�ļ�¼��table���еļ�¼�ţ�

����˵����
database�����ݿ�����cstring�ͣ�
table_name������ļ�����cstring��
inform��info�ṹ�壬�����á�
������������ʱ����¼�ű�����inform.offset�У���û�У�inform.offset��0��
����ֵ��
- 7    inform.offset = 0    �����ļ�Ϊ��
- 8    inform.offset = 0    �����޼�¼
- 1    inform.offset = 0    ����ֵ�����ʹ���int��float��char(n)��
- 3    inform.offset = 0    �����쳣��
����  Ҷ�ӿ�ţ� ��inform.offset > 0����¼���ڱ��ļ��У���¼��Ϊinform.offset
����  Ҷ�ӿ�� ����inform.offset��0����¼���ڱ��ļ��У����ص�Ҷ�ӿ���ں���insert_one��Ҫ�ã���Ϊ��ֵ����Ŀ�͹��ˣ�
˼·������b������ԭ�������м��Ҷ�ӽڵ㣬ֱ���ҵ���ֵ�����ء�

*/
int search_one(CString database, CString table_name, struct index_info &inform) 
{

	inform.offset = 0;
	fileInfo * ptrDatabase;

	//pre handle
	ptrDatabase = get_file_info(database, table_name, int(inform.type));// check if the type is right^
																		// �����ļ�Ϊ��
	// check if �����޼�¼
	if (NULL == ptrDatabase) 
	{
		return -8;
	}

	int type_kind = inform.type;
	int type_size = inform.length;

	//check if ����ֵ�����ʹ���int��float��char(n)��
	if (type_kind > 2 || type_kind < 0)
	{
		return -1; 
	}
	else if (type_kind == 0 && type_size != 5)
	{
		type_size = 5;
		cout << "type int should have length of 5 bit"<<endl;
	}
	else if (type_kind == 1 && type_size != 10)
	{
		type_size = 10;
		cout << "type int should have length of 10 bit" << endl;
	}

	CString des_value = inform.value;
	blockInfo *Block_temp = ptrDatabase->firstBlock;

	//�����ļ�Ϊ��
	if (NULL == Block_temp)
	{
		return -8;
	}

	CString node_value = Block_temp->cBlock;

	while (1) 
	{
		if ('!' == node_value.GetAt(0))			// leaf node
		{
			int num = _ttoi(node_value.Mid(1, 4));//the whole number of record

			int left, right, middle;
			left = 0;
			right = num - 1;
			//the following loop is a Binary Search
			while (left <= right)
			{
				middle = (left + right) / 2;
				if (node_value.Mid(10 + (5 + type_size)* middle, type_size) > des_value)
				{
					right = middle - 1;
				}
				else if (node_value.Mid(10 + (5 + type_size)* middle, type_size) < des_value)
				{
					left = middle + 1;
				}
				else
				{
					inform.offset = _ttoi(node_value.Mid(5 + (5 + type_size)* middle, 5));//find the value
					return Block_temp->blockNum;
				}
			}

			return Block_temp->blockNum;
		}
		else if ('?' == node_value.GetAt(0))			// normal node
		{
			int num = _ttoi(node_value.Mid(1, 4));     //the whole number of node
			int left, right, middle;
			left = 0;
			right = num;
			//the following loop is a Binary Search
			while (left < right - 1)
			{
				middle = (left + right) / 2;
				if (node_value.Mid(8 + (3 + type_size)* middle, type_size) > des_value)
				{
					right = middle + 1;
				}
				else if (node_value.Mid(8 + (3+ type_size)* middle, type_size) < des_value)
				{
					left = middle;
				}
				else 
				{
					left = middle;
					right = middle + 1;
					break;
				}
			}

			blockInfo *ptr_temp = get_file_block(database, table_name, type_kind,
				_ttoi(node_value.Mid(5 + (3 + type_size)* left, 3)));
			node_value = ptr_temp->cBlock;
		}	
		else
		{
			// �����쳣��
			return -3;
		}
	}
	//�����쳣�� on normal case will not reach there?
	cout << "function search_one has undefined state"<<endl;
	return -3;
}

//2������һ��>, >= , <, <= inform.value(informΪinfo�ṹ��)�ļ�¼��table���еļ�¼�ţ�
/*
����˵����
database�����ݿ�����cstring�ͣ�
table_name������ļ�����cstring��
inform��info�ṹ�壬�����ã�
type�� 3 �� �ҳ����� < inform.value �ļ�¼;
4 �� �ҳ����� <= inform.value �ļ�¼;
1 �� �ҳ����� > inform.value; �ļ�¼��
2 �� �ҳ����� >= inform.value �ļ�¼;
��������ʱ��
��û�У�start��0��
���� start�������ļ��������¼���ڵ�Ҷ�ӽڵ����ʼ���
end�������ļ��������¼���ڵ�Ҷ�ӽڵ����ֹ���
inform.offset �������ļ�start��ʼ���еĵ�һ������ֵ��ƫ������>, >= ��
����
�������ļ�end ��ֹ���е����һ������ֵ��ƫ������<, <= ��
˼·�������࿼�ǣ�����b������ԭ�������м��Ҷ�ӽڵ㣬ֱ���ҵ���ʼֵ����ֵֹ���ڵĿ�ʱ�����ء�

*/
void search_many(CString database, CString table_name, int& start, int& end, int type,
	struct index_info& inform) 
{
	int block_num = search_one(database, table_name, inform);
	if (block_num < 0)
		return;

	blockInfo *ptr_temp = get_file_block(database, table_name, type, block_num);

	start = 0;
	switch(type)
	{
	case 1:										//		> inform.value
		CString &temp = inform.value;
		temp.SetAt(temp.GetLength(), temp.GetAt(temp.GetLength()) + 1);
	case 2:			//		>= inform.value
		start = search_one(database, table_name, inform);
		end = find_right_child(database, table_name, inform);
		break;
	case 3:			//
		CString &temp = inform.value;
		temp.SetAt(temp.GetLength(), temp.GetAt(temp.GetLength()) - 1);
	case 4:			//		<= inform.value �ļ�¼;
		start = find_left_child(database, table_name, inform);
		end = search_one(database, table_name, inform);
		break;
	default:
		cout << "wrong type" << endl;
	}
	return;

}

/*
	3������һ��ֵ�ĺ�����ֵ�������ڲ���inform�ṹ���У�
	����˵����
	database�����ݿ�����cstring�ͣ�
	table_name������ļ�����cstring��
	inform��info�ṹ�壬�����á�
	˼·���ȵ���search_one�������ҵ�Ҫ�����Ҷ�ӿ��ţ����ж��Ƿ�ֵ�Ѿ����ڣ��������ڣ���Ҷ��δ��ʱ�����ҵ�λ�Ӳ��룬���򣬵���insert_divide�����ָ�ٲ��롣
*/

void insert_one(CString database, CString table_name, struct index_info & inform)
{
	int leaf_node = search_one(database, table_name, inform);
	int type_kind = inform.type;
	int type_size = inform.length;
	CString des_value = inform.value;

	blockInfo *pleaf = get_file_block(database, table_name, inform.length, leaf_node);
	CString value = pleaf->cBlock;
	int num = _ttoi(value.Mid(1, 4));//the whole number of record
	if (_ttoi(value.Mid(1, 4)) < 9999) 
	{

		int left, right, middle;
		left = 0;
		right = num - 1;
		//the following loop is a Binary Search
		while (left < right)
		{
			middle = (left + right) / 2;
			if (value.Mid(10 + (5 + type_size)* middle, type_size) > des_value)
			{
				right = middle - 1;
			}
			else if (value.Mid(10 + (5 + type_size)* middle, type_size) < des_value)
			{
				left = middle + 1;
			}
			else
			{
				return;
			}
		}
		CString record = int_to_str(inform.offset) + inform.value;
		value.Insert(5 + (5 + type_size)* left, record);

		//change the total number of record
		num++;
		CString num2 = int_to_str(num);
		value.Delete(1, 4);
		value.Insert(1, num2);
		pleaf->cBlock = (LPTSTR)(LPCTSTR)value;	
	}
	else 
	{
		int new_leaf_node = get_new_freeblocknum(database, table_name, inform);
		blockInfo *pleaf_new = get_file_block(database, table_name, inform.length, new_leaf_node);
		CString value1, value2, temp;
		int num1 = num / 2;
		value1 = value.Mid(5, num1*(5 + type_size));
		value2 = value.Mid(5+ num1*(5 + type_size),(num - num1) * (5 + type_size));
		value1.Insert(0, int_to_str(num1).Right(4));
		temp = value.Mid(5, type_size);
		value2.Insert(0, int_to_str(num - num1).Right(4));
		value1.Insert(0, '!');
		value2.Insert(0, '!');
		value1.Insert(value1.GetLength(), int_to_str(new_leaf_node).Right(3));

		if(value.Right(1)=="#")
			value2.Insert(value2.GetLength(), '#');
		else
			value2.Insert(value2.GetLength(), value.Right(3));

		pleaf->cBlock = (LPTSTR)(LPCTSTR)value1;

		pleaf_new->cBlock = (LPTSTR)(LPCTSTR)value2;

		insert_divide(database, table_name, inform, leaf_node, pleaf_new->blockNum, (LPTSTR)(LPCTSTR)temp);

	}

}

/*
2����Ҷ�ӿ���ʱ���ú�����
����˵����
database�����ݿ�����cstring�ͣ�
table_name������ļ�����cstring��
inform��info�ṹ�壬�����á�
leaf1��ԭ������Ҷ�ӿ�Ŀ�ţ��˿��Ѿ��ָ��
leaf2���µĿ�ţ��˿鱣����ԭ������Ҷ�ӿ�leaf1���Ұ벿��
leafpoint2��ָ���¿�leaf2��ָ��

˼·�����ڵ���ʱ����insert_one��������insert_divide���������ҳ�����Ҫ�ָ�Ľڵ�ĸ��ڵ㣬���϶��½��зָȻ����insert_divide�����ٵݹ����insert_one�������в��루��ʱ�϶��ܲ��룩��
*/
void insert_divide(CString database, CString table_name, struct index_info& inform,
	int leaf1, int leaf2, char* leafpoint2)
{
	int type_kind = inform.type;
	int type_size = inform.length;
	int block1 = leaf1;
	int block2 = leaf2;

	int father;
	CString temp;
	blockInfo *ptr_temp;
	CString value;
	temp = leafpoint2;

	while (1)
	{
		father = find_father(database, table_name, inform, block1);
		ptr_temp = get_file_block(database, table_name, type_kind, father);
		value = ptr_temp->cBlock;
		if ('?' == value.GetAt(0))												// normal node
		{
			int num = _ttoi(value.Mid(1, 4));									//the whole number of node
			int left, right, middle;
			left = 0;
			right = num - 1;
			//the following loop is a Binary Search
			while (left < right)
			{
				middle = (left + right) / 2;
				if (value.Mid(10 + (3 + type_size)* middle, type_size) > temp)
				{
					right = middle - 1;
				}
				else if (value.Mid(10 + (3 + type_size)* middle, type_size) < temp)
				{
					left = middle + 1;
				}
				else
				{
					std::cout << " The number already exist in the table" << endl;
					return;
				}
			}

			num++;
			if (num < 10000)			// if the node is not full
			{
				CString num2 = int_to_str(num);
				value.Delete(1, 4);
				value.Insert(1, num2);

				value.Insert(10 + (3 + type_size)* left, int_to_str(block2).Right(3));
				value.Insert(10 + (3 + type_size)* left, temp);

				ptr_temp->cBlock = (LPTSTR)(LPCTSTR)value;
				return;

			}
			else															
			{							// if the node is full	
				int new_node = get_new_freeblocknum(database, table_name, inform);
				blockInfo *pleaf_new = get_file_block(database, table_name, inform.length, new_node);
				CString value1, value2;
				int num1 = num / 2;
				value1 = value.Mid(5, num1*(3 + type_size));
				value2 = value.Mid(5 + type_size + num1*(3 + type_size), (num - num1) * (3 + type_size));
				value1.Insert(0, int_to_str(num1).Right(4));
				value2.Insert(0, int_to_str(num - num1).Right(4));
				value1.Insert(0, '?');
				value2.Insert(0, '?');

				temp = value.Mid(5 + type_size + num1*(3 + type_size), type_size);
			}
		}


	}

}

/*
4��int ����ֵת����5�ֽڵ�string���� ��
����˵����value�� int��
����ֵ��  cstring��
*/
CString int_to_str(int value) 
{
	CString temp;
	for (int i = 0; i < 5; i++)
	{
		temp.Insert(0, char(value & 0xFF + '0'));
		value >> 4;
	}
	return temp;
}

/*
5���ҳ�����Ҷ�ӿ��ǰһ���ֵ�Ҷ�ӿ飺
����˵����
database�����ݿ�����cstring�ͣ�
table_name������ļ�����cstring��
inform��info�ṹ�壬�������ã�
nodenum������Ҷ�ӿ���
����ֵ��
- 1      �����ж�������飻
0       û��ǰһ���ֵ�Ҷ�ӿ飨���˿�Ϊ�����ӣ�
>0      ǰһ���ֵ�Ҷ�ӿ�Ŀ��
˼·������find_left_child�������ҳ������ӣ����ж��Ƿ���nodenum��ȣ�����ȵĻ���
�������к��ӣ��ҵ�nodenum��ʱ����������ǰ��ڵ㣬���򣬷���0��
*/

int find_prev_leaf_sibling(CString database, CString table_name, struct index_info inform,
	int nodenum) 
{
	int most_left_child = find_left_child(database, table_name, inform);
	if (most_left_child == nodenum) return 0;

	blockInfo *ptr_temp = get_file_block(database, table_name, inform.length, nodenum);
	CString node_value = ptr_temp->cBlock;
	int blocknum_rem = ptr_temp->blockNum;

	while (node_value.Right(1) != "#")
	{
		ptr_temp = get_file_block(database, table_name, inform.length, _ttoi(node_value.Right(3)));
		CString node_value = ptr_temp->cBlock;
		if (ptr_temp->blockNum == nodenum)
			return blocknum_rem;
		else
		{
			blocknum_rem = ptr_temp->blockNum;
			node_value = ptr_temp->cBlock;
		}
	}
}

/*
6���ҳ�����Ҷ�ӿ�ĺ�һ���ֵ�Ҷ�ӿ飺
����˵����
database�����ݿ�����cstring�ͣ�
table_name������ļ�����cstring��
inform��info�ṹ�壬�������ã�
nodenum������Ҷ�ӿ���
����ֵ��
- 1      �����ж�������飻
0       û�к�һ���ֵ�Ҷ�ӿ飨���˿�Ϊ���Һ��ӣ�
>0      ��һ���ֵ�Ҷ�ӿ�Ŀ��
˼·������find_right_child�������ҳ����Һ��ӣ����ж��Ƿ���nodenum��ȣ�����ȵĻ����������к��ӣ��ҵ�nodenum��ʱ���������ĺ���ڵ㣬���򣬷���0��

*/

int find_next_leaf_sibling(CString database, CString table_name, struct index_info inform,
	int nodenum) 
{
	blockInfo *ptr_temp = get_file_block(database, table_name, inform.length, nodenum);	
	CString node_value = ptr_temp->cBlock;
	if (node_value.Right(1) == "#")
	{
		return 0;
	}
	return  _ttoi(node_value.Right(3));
}

/*
7���ҳ�����Ҷ�ӿ�Ŀ�ţ�
����˵����
database�����ݿ�����cstring�ͣ�
inform��info�ṹ�壬�������ã�
����ֵ��
- 1      �����ж�������飻
0       �����ļ���
>0      ����Ҷ�ӿ�Ŀ��
˼·������B������ԭ��ÿ�β��������ӣ�ֱ���ҵ�Ҷ�ӽڵ㡣
*/
int find_left_child(CString database,CString table_name, index_info  inform)
{
	fileInfo * ptrDatabase;
	ptrDatabase = get_file_info(database, table_name, int(inform.type));// check if the type is right^
	int type_kind = inform.type;
	int type_size = inform.length;
	blockInfo *Block_temp = ptrDatabase->firstBlock;
	CString node_value = Block_temp->cBlock;

	while (1)
	{
		if ('!' == node_value.GetAt(0))			// leaf node
		{
			return Block_temp->blockNum;
		}
		else if ('?' == node_value.GetAt(0))			// normal node
		{
			int num = _ttoi(node_value.Mid(1, 4));     //the whole number of node
			if (0 == num)
				return -1;
			blockInfo *ptr_temp = get_file_block(database, table_name, type_kind,
				_ttoi(node_value.Mid(5 , 3)));
			node_value = ptr_temp->cBlock;
		}
		else
		{
			// �����쳣��
			return -1;
		}
	}
	//�����쳣�� on normal case will not reach there
	cout << "function find_left_child has undefined state" << endl;
	return -1;

}

/*
8���ҳ�����Ҷ�ӿ�Ŀ�ţ�
����˵����
database�����ݿ�����cstring�ͣ�
inform��info�ṹ�壬�������ã�
����ֵ��
- 1      �����ж�������飻
0       �����ļ���
>0      ����Ҷ�ӿ�Ŀ��
˼·������B������ԭ��ÿ�β������Һ��ӣ�ֱ���ҵ�Ҷ�ӽڵ㡣
*/

int find_right_child(CString database, CString table_name, index_info  inform)
{
	fileInfo * ptrDatabase;
	ptrDatabase = get_file_info(database, table_name, int(inform.type));// check if the type is right^
	int type_kind = inform.type;
	int type_size = inform.length;
	blockInfo *Block_temp = ptrDatabase->firstBlock;
	CString node_value = Block_temp->cBlock;

	while (1)
	{
		if ('!' == node_value.GetAt(0))			// leaf node
		{
			return Block_temp->blockNum;
		}
		else if ('?' == node_value.GetAt(0))			// normal node
		{
			int num = _ttoi(node_value.Mid(1, 4));     //the whole number of node
			if (0 == num)
				return -1;
			blockInfo *ptr_temp = get_file_block(database, table_name, type_kind,
				_ttoi(node_value.Mid(5 + (3 + type_size)* num, 3)));
			node_value = ptr_temp->cBlock;
		}
		else
		{
			// �����쳣��
			return -1;
		}
	}
	//�����쳣�� on normal case will not reach there?
	cout << "function find_right_child has undefined state" << endl;
	return -1;
}

/*
9����ȡһ���տ�Ŀ�ţ���ʹ�ܿ�����һ
����˵����
database�����ݿ�����cstring�ͣ�
table_name������ļ�����cstring��
inform��info�ṹ�壬�����á�
����ֵ��
0      �����쳣�飬���߿���>256��
>0     �տ��
˼·���жϿ����Ƿ�>256����û�У��жϿտ������Ƿ�Ϊ�գ�Ϊ����õ��������ܿ����
��1���Ŀտ飬���򣬶�ȡ�տ������ף��޸�������ʹ�ܿ�����1��
*/

int get_new_freeblocknum(CString database, CString table_name, struct index_info& inform)
{
	fileInfo * ptrDatabase;
	ptrDatabase = get_file_info(database, table_name, int(inform.type));// check if the type is right^
	if (ptrDatabase->recordAmount > 256)
		return 0;

	if (ptrDatabase->freeNum)
		return ptrDatabase->freeNum;
	else
	{
		blockInfo *newBlock = (blockInfo *)malloc(sizeof(struct blockInfo));
		//initilize the Block for other part
		newBlock->blockNum = ++ptrDatabase->recordAmount;
		newBlock->charNum = 3;
		newBlock->dirtyBit = 0;
		newBlock->cBlock = "000";
		newBlock->next = ptrDatabase->firstBlock;
		ptrDatabase->firstBlock = newBlock;
		return newBlock->blockNum;
	}
	cout << "error in get_new_freeblocknum" << endl;
	return -1;
}

/*
10���ҿ�num�ĸ��׿�Ŀ�ţ��ڲ���inform.value��·���ϣ�
����˵����
database�����ݿ�����cstring�ͣ�
table_name������ļ�����cstring��
inform��info�ṹ�壬�������ã�
num�����ڿ�Ŀ��
����ֵ��
- 1      �����ж�������飻
0       û�и��׿飨���˿�Ϊ����
>0      ���׿�Ŀ��
˼·������B������ԭ������ֵΪinform.value���ڵ�Ҷ�ӽڵ㣬����;�������Ϊ
num�����ݿ飬�򷵻����ĸ��׽ڵ�
*/

int find_father(CString database, CString table_name, index_info inform, int num) 
{
	{

		inform.offset = 0;
		fileInfo * ptrDatabase;

		//pre handle
		ptrDatabase = get_file_info(database, table_name, int(inform.type));// check if the type is right^
																			// �����ļ�Ϊ��
																			// check if �����޼�¼
		if (NULL == ptrDatabase)
		{
			return -8;
		}

		int type_kind = inform.type;
		int type_size = inform.length;

		CString des_value = inform.value;
		blockInfo *Block_temp = ptrDatabase->firstBlock;

		//�����ļ�Ϊ��
		if (NULL == Block_temp)
		{
			return -8;
		}
		int block_num = Block_temp->blockNum;

		CString node_value = Block_temp->cBlock;

		while (1)
		{
			if (num == Block_temp->blockNum)
				return block_num;
			else
				block_num = Block_temp->blockNum;


			if ('!' == node_value.GetAt(0))			// leaf node
			{
				int whole = _ttoi(node_value.Mid(1, 4));//the whole number of record

				int left, right, middle;
				left = 0;
				right = whole - 1;
				//the following loop is a Binary Search
				while (left <= right)
				{
					middle = (left + right) / 2;
					if (node_value.Mid(10 + (5 + type_size)* middle, type_size) > des_value)
					{
						right = middle - 1;
					}
					else if (node_value.Mid(10 + (5 + type_size)* middle, type_size) < des_value)
					{
						left = middle + 1;
					}
					else
					{
						inform.offset = _ttoi(node_value.Mid(5 + (5 + type_size)* middle, 5));//find the value
						return Block_temp->blockNum;
					}
				}

				return 0;
			}
			else if ('?' == node_value.GetAt(0))			// normal node
			{

				int whole = _ttoi(node_value.Mid(1, 4));     //the whole number of node
				int left, right, middle;
				left = 0;
				right = whole;
				//the following loop is a Binary Search
				while (left < right - 1)
				{
					middle = (left + right) / 2;
					if (node_value.Mid(8 + (3 + type_size)* middle, type_size) > des_value)
					{
						right = middle + 1;
					}
					else if (node_value.Mid(8 + (3 + type_size)* middle, type_size) < des_value)
					{
						left = middle;
					}
					else
					{
						left = middle;
						right = middle + 1;
						break;
					}
				}

				blockInfo *ptr_temp = get_file_block(database, table_name, type_kind,
					_ttoi(node_value.Mid(5 + (3 + type_size)* left, 3)));
				node_value = ptr_temp->cBlock;
			}
			else
			{
				// �����쳣��
				return -1;
			}
		}
		//�����쳣�� on normal case will not reach there?
		cout << "function search_one has undefined state" << endl;
		return 0;
	}

}

/*11��*/
void delete_one(CString database, CString table_name,
	struct index_info & inform) 
{
	fileInfo * ptrDatabase;
	ptrDatabase = get_file_info(database, table_name, int(inform.type));// check if the type is right^
	int Block_num = search_one(database, table_name, inform);
	blockInfo *Block = get_file_block(database, table_name, inform.length, Block_num);
	CString m;
	m = int_to_str(ptrDatabase->freeNum);
	m.Delete(0, 2);
	Block->cBlock = (LPTSTR)(LPCTSTR)m;					// From CString to char *
	ptrDatabase->freeNum = Block_num;

}
