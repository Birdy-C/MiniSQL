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


//1������һ��ֵ����inform.value(informΪinfo�ṹ��)�ļ�¼��table���еļ�¼�ţ�
int search_one(CString database, CString table_name, struct index_info & inform);
//2������һ��>, >= , <, <= inform.value(informΪinfo�ṹ��)�ļ�¼��table���еļ�¼�ţ�
void search_many(CString database, CString table_name, int& start, int& end, int type,
		struct index_info& inform);

//3����Ҷ�ӿ���ʱ���ú�����
void insert_divide(CString database, CString table_name, struct index_info& inform,
	int leaf1, int leaf2, char* leafpoint2);

//4��int ����ֵת����5�ֽڵ�string���� ��
CString int_to_str(int value);

//5���ҳ�����Ҷ�ӿ��ǰһ���ֵ�Ҷ�ӿ飺
int find_prev_leaf_sibling(CString database, CString table_name, struct index_info inform,
	int nodenum);

//6���ҳ�����Ҷ�ӿ�ĺ�һ���ֵ�Ҷ�ӿ飺
int find_next_leaf_sibling(CString database, CString table_name, struct index_info inform,
	int nodenum);

//7���ҳ�����Ҷ�ӿ�Ŀ�ţ�
int find_left_child(CString database, index_info  inform);

//8���ҳ�����Ҷ�ӿ�Ŀ�ţ�
int find_right_child(CString database, index_info  inform);

//9����ȡһ���տ�Ŀ�ţ���ʹ�ܿ�����һ
int get_new_freeblocknum(CString database, CString table_name, struct index_info& inform);

//10���ҿ�num�ĸ��׿�Ŀ�ţ��ڲ���inform.value��·���ϣ�
int find_father(CString database, CString table_name, index_info inform, int num);

//11��
void delete_one(CString database, CString table_name,
	struct index_info & inform);

#endif
