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
����  Ҷ�ӿ�ţ� ��inform.offset>0����¼���ڱ��ļ��У���¼��Ϊinform.offset
����  Ҷ�ӿ�� ����inform.offset��0����¼���ڱ��ļ��У����ص�Ҷ�ӿ���ں���insert_one��Ҫ�ã���Ϊ��ֵ����Ŀ�͹��ˣ�
˼·������b������ԭ�������м��Ҷ�ӽڵ㣬ֱ���ҵ���ֵ�����ء�
fileInfo* get_file_info(CString DB_Name,CString fileName, int m_fileType)
//Get_Index_Info(database, inform.index_name, int & record_count, inform.length, int & free);

*/
int search_one(CString database, CString table_name, struct index_info &inform) 
{
	//get_file_info(database, CString fileName, int m_fileType)



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

}
	/*
	3������һ��ֵ�ĺ�����ֵ�������ڲ���inform�ṹ���У�
	����˵����
	database�����ݿ�����cstring�ͣ�
	table_name������ļ�����cstring��
	inform��info�ṹ�壬�����á�
	˼·���ȵ���search_one�������ҵ�Ҫ�����Ҷ�ӿ��ţ����ж��Ƿ�ֵ�Ѿ����ڣ��������ڣ���Ҷ��δ��ʱ�����ҵ�λ�Ӳ��룬���򣬵���insert_divide�����ָ�ٲ��롣
*/

void insert_one(CString database, CString  table_name, struct index_info & inform) 
{

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

}
/*
4��int ����ֵת����5�ֽڵ�string���� ��
����˵����value�� int��
����ֵ��  cstring��

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
int find_left_child(CString database, index_info  inform) 
{

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

int find_right_child(CString database, index_info  inform) 
{

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
}

/*11��*/
void delete_one(CString database, CString table_name,
	struct index_info & inform) 
{

}
