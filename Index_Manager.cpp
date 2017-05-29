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
//1．查找一个值等于inform.value(inform为info结构体)的记录在table表中的记录号：

参数说明：
database：数据库名，cstring型；
table_name：表格文件名，cstring型
inform：info结构体，是引用。
函数结束返回时，记录号保存在inform.offset中，若没有，inform.offset＝0。
返回值：
- 7    inform.offset = 0    索引文件为空
- 8    inform.offset = 0    表中无记录
- 1    inform.offset = 0    所查值的类型错误（int，float，char(n)）
- 3    inform.offset = 0    读到异常块
正数  叶子块号， 若inform.offset > 0，记录存在表文件中，记录号为inform.offset
正数  叶子块号 ，若inform.offset＝0，记录不在表文件中，返回的叶子块号在函数insert_one中要用（因为将值插入改块就够了）
思路：按照b＋树的原理，遍历中间和叶子节点，直到找到其值，返回。

*/
int search_one(CString database, CString table_name, struct index_info &inform) 
{

	inform.offset = 0;
	fileInfo * ptrDatabase;

	//pre handle
	ptrDatabase = get_file_info(database, table_name, int(inform.type));// check if the type is right^
																		// 索引文件为空
	// check if 表中无记录
	if (NULL == ptrDatabase) 
	{
		return -8;
	}

	int type_kind = inform.type;
	int type_size = inform.length;

	//check if 所查值的类型错误（int，float，char(n)）
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

	//索引文件为空
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
			// 读到异常块
			return -3;
		}
	}
	//读到异常块 on normal case will not reach there?
	cout << "function search_one has undefined state"<<endl;
	return -3;
}

//2．查找一批>, >= , <, <= inform.value(inform为info结构体)的记录在table表中的记录号：
/*
参数说明：
database：数据库名，cstring型；
table_name：表格文件名，cstring型
inform：info结构体，是引用，
type： 3 是 找出所有 < inform.value 的记录;
4 是 找出所有 <= inform.value 的记录;
1 是 找出所有 > inform.value; 的记录；
2 是 找出所有 >= inform.value 的记录;
函数返回时：
若没有：start＝0；
否则： start＝索引文件中所需记录所在的叶子节点的起始块号
end＝索引文件中所需记录所在的叶子节点的终止块号
inform.offset ＝索引文件start起始块中的第一个所需值的偏移量（>, >= ）
或则
＝索引文件end 终止块中的最后一个所需值的偏移量（<, <= ）
思路：分两类考虑，按照b＋树的原理，遍历中间和叶子节点，直到找到起始值或终止值所在的块时，返回。

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
	case 4:			//		<= inform.value 的记录;
		start = find_left_child(database, table_name, inform);
		end = search_one(database, table_name, inform);
		break;
	default:
		cout << "wrong type" << endl;
	}
	return;

}

/*
	3．插入一个值的函数，值和类型在参数inform结构体中：
	参数说明：
	database：数据库名，cstring型；
	table_name：表格文件名，cstring型
	inform：info结构体，是引用。
	思路：先调用search_one函数，找到要插入的叶子块块号，并判断是否值已经存在，若不存在，当叶子未满时，则找到位子插入，否则，调用insert_divide函数分割，再插入。
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
2．到叶子块满时调用函数：
参数说明：
database：数据库名，cstring型；
table_name：表格文件名，cstring型
inform：info结构体，是引用。
leaf1：原来满的叶子块的块号，此块已经分割好
leaf2：新的块号，此块保存有原来满的叶子块leaf1的右半部分
leafpoint2：指向新块leaf2的指针

思路：当节点满时，由insert_one函数调用insert_divide函数，先找出所有要分割的节点的父节点，自上而下进行分割，然后由insert_divide函数再递归调用insert_one函数进行插入（这时肯定能插入）。
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
4．int 型数值转化成5字节的string类型 ：
参数说明：value： int型
返回值：  cstring型
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
5．找出所在叶子块的前一个兄弟叶子块：
参数说明：
database：数据库名，cstring型；
table_name：表格文件名，cstring型
inform：info结构体，不是引用，
nodenum：所在叶子块块号
返回值：
- 1      函数中读到错误块；
0       没有前一个兄弟叶子块（即此块为最左孩子）
>0      前一个兄弟叶子块的块号
思路：调用find_left_child函数，找出最左孩子，并判断是否与nodenum相等，不相等的话，
遍历所有孩子，找到nodenum块时，返回他的前面节点，否则，返回0。
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
6．找出所在叶子块的后一个兄弟叶子块：
参数说明：
database：数据库名，cstring型；
table_name：表格文件名，cstring型
inform：info结构体，不是引用，
nodenum：所在叶子块块号
返回值：
- 1      函数中读到错误块；
0       没有后一个兄弟叶子块（即此块为最右孩子）
>0      后一个兄弟叶子块的块号
思路：调用find_right_child函数，找出最右孩子，并判断是否与nodenum相等，不相等的话，遍历所有孩子，找到nodenum块时，返回他的后面节点，否则，返回0。

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
7．找出最左叶子块的块号：
参数说明：
database：数据库名，cstring型；
inform：info结构体，不是引用，
返回值：
- 1      函数中读到错误块；
0       索引文件空
>0      最左叶子块的块号
思路：根据B＋树的原理，每次查找最左孩子，直到找到叶子节点。
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
			// 读到异常块
			return -1;
		}
	}
	//读到异常块 on normal case will not reach there
	cout << "function find_left_child has undefined state" << endl;
	return -1;

}

/*
8．找出最右叶子块的块号：
参数说明：
database：数据库名，cstring型；
inform：info结构体，不是引用，
返回值：
- 1      函数中读到错误块；
0       索引文件空
>0      最右叶子块的块号
思路：根据B＋树的原理，每次查找最右孩子，直到找到叶子节点。
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
			// 读到异常块
			return -1;
		}
	}
	//读到异常块 on normal case will not reach there?
	cout << "function find_right_child has undefined state" << endl;
	return -1;
}

/*
9．获取一个空块的块号，并使总块数增一
参数说明：
database：数据库名，cstring型；
table_name：表格文件名，cstring型
inform：info结构体，是引用。
返回值：
0      读到异常块，或者块数>256块
>0     空块号
思路：判断块数是否>256，若没有，判断空块链表是否为空，为空则得到（现有总块号数
＋1）的空块，否则，读取空块链表首，修改链表；并使总块数＋1。
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
10．找块num的父亲块的块号（在查找inform.value的路径上）
参数说明：
database：数据库名，cstring型；
table_name：表格文件名，cstring型
inform：info结构体，不是引用，
num：所在块的块号
返回值：
- 1      函数中读到错误块；
0       没有父亲块（即此块为根）
>0      父亲块的块号
思路：根据B＋树的原理，查找值为inform.value所在的叶子节点，若中途碰到块号为
num的数据块，则返回他的父亲节点
*/

int find_father(CString database, CString table_name, index_info inform, int num) 
{
	{

		inform.offset = 0;
		fileInfo * ptrDatabase;

		//pre handle
		ptrDatabase = get_file_info(database, table_name, int(inform.type));// check if the type is right^
																			// 索引文件为空
																			// check if 表中无记录
		if (NULL == ptrDatabase)
		{
			return -8;
		}

		int type_kind = inform.type;
		int type_size = inform.length;

		CString des_value = inform.value;
		blockInfo *Block_temp = ptrDatabase->firstBlock;

		//索引文件为空
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
				// 读到异常块
				return -1;
			}
		}
		//读到异常块 on normal case will not reach there?
		cout << "function search_one has undefined state" << endl;
		return 0;
	}

}

/*11．*/
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
