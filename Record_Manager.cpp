#include"API_Module.h"
#include"Catalog_Manager.h"
#include"Record_Manager.h"
#include"Index_Manager.h"

//释放数据库的内存缓冲区
void Close_Database(CString DB_Name, bool closetype)
{
	printf("call Record_Manager->Close_Database(...);\n");
	printf("释放数据库的内存缓冲区\n");
}

//释放表或索引的内存缓冲区
void Close_File(CString DB_Name, CString filename, int filetype, bool closetype)
{
	printf("call Record_Manager->Close_File(...);\n");
	printf("释放表或索引的内存缓冲区\n");
}

//向表中插入元组
void Insert_Item(CString DB_Name, CString Table_Name, CString Attr, int & record_Num)
{
	printf("call Record_Manager->Insert_Item(...);\n");
	printf("向表中插入元组\n");
}

//打印选择属性
void Print_Head(attr_info print[32], int count)
{
	printf("call Record_Manager->Print_Head(...);\n");
	printf("打印选择属性\n");
}

//打印出选择结果
void Print_To_Screen(CString record, attr_info print[32], int count)
{
	printf("call Record_Manager->Print_To_Screen(...);\n");
	printf("打印出选择结果\n");
}

//有where，但无可用索引的select语句
void Select_Without_Useful_Cond(CString DB_Name, CString Table_Name, condition_info conds[10], int count, attr_info print[32], int Count, char cond)
{
	printf("call Record_Manager->Select_Without_Useful_Cond(...);\n");
	printf("有where，但无可用索引的select语句\n");
}
//有where且有可用索引的select 语句
void Select_Without_Useful_Cond(CString DB_Name, CString Table_Name, condition_info conds[10], int count, attr_info print[32], int Count, char cond, int index)
{
	printf("call Record_Manager->Select_Without_Useful_Cond(...);\n");
	printf("有where且有可用索引的select 语句\n");
}

//用等于索引的选择语句
void Select_With_Equal_Cond(CString DB_Name, CString Table_Name, condition_info conds[32], int count, attr_info print[32], int Count, char cond, index_info Index)
{
	printf("call Record_Manager->Select_With_Equal_Cond(...);\n");
	printf("用等于索引的选择语句\n");
}

//用大于或大于等于索引的选择语句
void Select_With_Greater_Cond(CString DB_Name, CString Table_Name, condition_info conds[32], int count, attr_info print[32], int Count, char cond, index_info Index, int type)
{
	printf("call Record_Manager->Select_With_Greater_Cond(...);\n");
	printf("用大于或大于等于索引的选择语句\n");
}

//用小于或小于等于索引的选择语句
void Select_With_Smaller_Cond(CString DB_Name, CString Table_Name, condition_info conds[32], int count, attr_info print[32], int Count, char cond, index_info Index, int type)
{
	printf("call Record_Manager->Select_With_Smaller_Cond(...);\n");
	printf("用小于或小于等于索引的选择语句\n");
}

//判断是否符合where条件组
bool Confirm_To_Where(CString record, condition_info conds[10], int count, char cond)
{
	printf("call Record_Manager->Confirm_To_Where(...);\n");
	printf("判断是否符合where条件组\n");
}

//判断是否符合某个指定条件
bool Confirm(CString record, condition_info condition)
{
	printf("call Record_Manager->Confirm(...);\n");
	printf("判断是否符合某个指定条件\n");
}

//选择语句（无where）
void Select_No_Where(CString DB_Name, CString Table_Name, attr_info print[32], int count)
{
	printf("call Record_Manager->Select_No_Where(...);\n");
	printf("选择语句（无where）\n");
}

//选择语句（有where）
void Select_With_Where(CString DB_Name, CString Table_Name, condition_info conds[10], int count, char cond, attr_info print[32], int Count)
{
	printf("call Record_Manager->Select_With_Where(...);\n");
	printf("选择语句（有where）\n");
}

//删除语句（有where）
void Delete_With_Where(CString DB_Name, CString Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond)
{
	printf("call Record_Manager->Delete_With_Where(...);\n");
	printf("删除语句（有where）\n");
}

//有where, 但无可用索引的delete语句
void Delete_Without_Useful_Cond(CString DB_Name, CString Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond)
{
	printf("call Record_Manager->Delete_Without_Useful_Cond(...);\n");
	printf("有where, 但无可用索引的delete语句\n");
}

//有where且有可用索引的delete 语句
void Delete_With_Useful_Cond(CString DB_Name, CString Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond, int index)
{
	printf("call Record_Manager->Delete_With_Useful_Cond(...);\n");
	printf("有where且有可用索引的delete 语句\n");
}

//用等于索引的删除语句
void Delete_With_Equal_Cond(CString DB_Name, CString Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond, index_info Index)
{
	printf("call Record_Manager->Delete_With_Equal_Cond(...);\n");
	printf("用等于索引的删除语句\n");
}

//用大于或大于等于索引的删除语句
void Delete_With_Greater_Cond(CString DB_Name, CString Table_Name, condition_info conds[10], int count, index_info nodes[32], int num, char cond, index_info Index, int type)
{
	printf("call Record_Manager->Delete_With_Greater_Cond(...);\n");
	printf("用大于或大于等于索引的删除语句\n");
}

//用小于或小于等于索引的删除语句
void Select_With_Smaller_Cond(CString DB_Name, CString Table_Name, condition_info conds[32], int count, index_info nodes[32], int num, char cond, index_info Index, int type)
{
	printf("call Record_Manager->Select_With_Smaller_Cond(...);\n");
	printf("用小于或小于等于索引的删除语句\n");
}

//查找有无可用索引
bool Find_Useful_Cond(condition_info conds[10], int count, int & index)
{
	printf("call Record_Manager->Find_Useful_Cond(...);\n");
	printf("查找有无可用索引\n");
}

//插入所有索引节点
void Insert_Index_All(CString DB_Name, CString Table_Name, CString Index_Name, int length, int offset, int type)
{
	printf("call Record_Manager->Insert_Index_All(...);\n");
	printf("插入所有索引节点\n");
}

//验证可否插入
bool Verify_Insertable(CString DB_Name, CString Table_Name, index_info nodes[32], int count, CString Attr)
{
	printf("call Record_Manager->Verify_Insertable(...);\n");
	printf("验证可否插入\n");
}

//退出程序
void Quit(CString DB_Name)
{
	printf("call Record_Manager->Quit(...);\n");
	printf("退出程序\n");
}
