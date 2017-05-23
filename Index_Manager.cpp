//////////////////////////////////////////////////////////
///----------------------------------------------------///
///       Module: Index_Manager.cpp												///
///       Produced by: Birdy & C													///
///       Description: produce index for the project create index for buffer	///
///       date:2017/5/22                             ///
///----------------------------------------------------///
///////////////////////////////////////////////////////////
#include "Index_Manager.h"

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
正数  叶子块号， 若inform.offset>0，记录存在表文件中，记录号为inform.offset
正数  叶子块号 ，若inform.offset＝0，记录不在表文件中，返回的叶子块号在函数insert_one中要用（因为将值插入改块就够了）
思路：按照b＋树的原理，遍历中间和叶子节点，直到找到其值，返回。
fileInfo* get_file_info(CString DB_Name,CString fileName, int m_fileType)
//Get_Index_Info(database, inform.index_name, int & record_count, inform.length, int & free);

*/
int search_one(CString database, CString table_name, struct index_info &inform) 
{
	//get_file_info(database, CString fileName, int m_fileType)



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

}
	/*
	3．插入一个值的函数，值和类型在参数inform结构体中：
	参数说明：
	database：数据库名，cstring型；
	table_name：表格文件名，cstring型
	inform：info结构体，是引用。
	思路：先调用search_one函数，找到要插入的叶子块块号，并判断是否值已经存在，若不存在，当叶子未满时，则找到位子插入，否则，调用insert_divide函数分割，再插入。
*/

void insert_one(CString database, CString  table_name, struct index_info & inform) 
{

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

}
/*
4．int 型数值转化成5字节的string类型 ：
参数说明：value： int型
返回值：  cstring型

*/
CString int_to_str(int value) 
{
	CString temp;
	while (value)
	{
		temp += char(value & 0xFF);
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
int find_left_child(CString database, index_info  inform) 
{

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

int find_right_child(CString database, index_info  inform) 
{

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
}

/*11．*/
void delete_one(CString database, CString table_name,
	struct index_info & inform) 
{

}
