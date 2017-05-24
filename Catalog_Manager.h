#ifndef CATALOG_MANAGER_H
#define CATALOG_MANAGER_H

#include"MiniSQL.h"


//创建数据库
void Create_Database(CString DB_Name);

//创建表
void Create_Table(CString Table_Name, CString Attr, CString DB_Name, CString & Attr_Name);

//创建索引
void Create_Index(CString Index_Name, CString Table_Name, CString Attr_Name, CString DB_Name, int & length, int & offset, int & type);

//向字符串增加信息
CString add_info(CString info, int length, CString attr);

//验证属性组名是否有效
bool verify_attrs(CString attrs[32], int count);

//删除数据库
void Drop_Database(CString DB_Name);

//删除表
void Drop_Table(CString Table_Name, CString DB_Name, CString index_name[32], int & count);

//删除索引
void Drop_Index(CString Index_Name, CString DB_Name);

//验证并增加属性值
bool verify_add_attr(CString & attr, CString & temp, int type, int length);

//判断是否为表的属性名
bool verify_attr_name(CString attr_name, CFile & file, long header, int attrs, long & offset, int & length, char & type, CString & Index_Name);

//验证属性值组的有效性及表中所有索引
void Verify_Attr(CString DB_Name, CString Table_Name, CString & Attr, index_info nodes[32], int & count);

//删除表内所有元组
void Empty_Table(CString DB_Name, CString Table_Name, CString index_name[32], int & count);

//转换条件组
bool Get_Condition(CString DB_Name, CString Table_Name, CString Condition, condition_info conds[32], int & count, char & cond, index_info nodes[32], int & num);

//获取显示记录格式
void Get_Attr_Info(CString DB_Name, CString Table_Name, attr_info print[32], int & count, CString Attr);

//获取所有属性的信息
void Get_Attr_Info_All(CString DB_Name, CString Table_Name, attr_info print[32], int & count);

//获取表中所有的索引信息
void Get_All_Index(CFile & file, long header, int attrs, index_info nodes[32], int & num);

//使用数据库
CString Use_Database(CString DB_Name);

//执行指定文件
void Exect_File(CString File);

#endif // !CATALOG_MANAGER_H