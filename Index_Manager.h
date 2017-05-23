//////////////////////////////////////////////////////////
///----------------------------------------------------///
///       Module: Index_Manager                           ///
///       Produced by: Birdy_C                          ///
///       Description: produce index for the project create index for buffer	///
///       date:2017/5/22                             ///
///----------------------------------------------------///
//////////////////////////////////////////////////////////
#if !defined(_API_MODULE_H_)
#define _API_MODULE_H_

#include"MiniSQL.h"


//1．查找一个值等于inform.value(inform为info结构体)的记录在table表中的记录号：
int search_one(CString database, CString table_name, struct index_info & inform);
//2．查找一批>, >= , <, <= inform.value(inform为info结构体)的记录在table表中的记录号：
void search_many(CString database, CString table_name, int& start, int& end, int type,
		struct index_info& inform);

//3．到叶子块满时调用函数：
void insert_divide(CString database, CString table_name, struct index_info& inform,
	int leaf1, int leaf2, char* leafpoint2);

//4．int 型数值转化成5字节的string类型 ：
CString int_to_str(int value);

//5．找出所在叶子块的前一个兄弟叶子块：
int find_prev_leaf_sibling(CString database, CString table_name, struct index_info inform,
	int nodenum);

//6．找出所在叶子块的后一个兄弟叶子块：
int find_next_leaf_sibling(CString database, CString table_name, struct index_info inform,
	int nodenum);

//7．找出最左叶子块的块号：
int find_left_child(CString database, index_info  inform);

//8．找出最右叶子块的块号：
int find_right_child(CString database, index_info  inform);

//9．获取一个空块的块号，并使总块数增一
int get_new_freeblocknum(CString database, CString table_name, struct index_info& inform);

//10．找块num的父亲块的块号（在查找inform.value的路径上）
int find_father(CString database, CString table_name, index_info inform, int num);

//11．
void delete_one(CString database, CString table_name,
	struct index_info & inform);

#endif
