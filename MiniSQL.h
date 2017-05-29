#if !defined(_MINISQL_H_)
#define _MINISQL_H_

#include<iostream>
#include<afx.h>
#include<stdlib.h>
#include<math.h>
#include<direct.h>

#define FILE_COUNTER 1
#define FILE_INFO_LENGTH 671
#define ATTR_INFO_LENGTH 20
#define FILE_HEAD_LENGTH 31



//定义delete 语句的条件信息
struct condition_info
{
	char left_type;
	int left_len;
	long left_offset;
	CString left_index_name;
	char right_type;
	int right_len;
	long right_offset;
	CString right_index_name;
	CString const_data;
	CString condition;
};	

//定义select 语句显示各属性信息
struct attr_info
{
	CString attr_name;
	int offset;
	int length;
	char type;
};


// 所读取块的信息
struct blockInfo
{
	int blockNum;				// the block number of the block, which indicate it when it be newed
	bool dirtyBit;				// 0 -> flase
								// 1 -> indicate dirty, write back
	blockInfo *next;			// the pointer point to next block 
	fileInfo *file;				// the pointer point to the file, which the block belongs to
	int charNum;				// the number of chars in the block
	char *cBlock;				// the array space for storing the records in the block in buffer
	int iTime;					// it indicate the age of the block in use 
	int lock;					// prevent the block from replacing
};

struct fileInfo
{
	int type;					// 0-> data file
								// 1 -> index file
	CString fileName;			// the name of the file
	int recordAmount;			// the number of record in the file
	int freeNum;                // the free block number which could be used for the file
	int recordLength;			// the length of the record in the file
	fileInfo *next;				// the pointer points to the next file
	blockInfo *firstBlock;		// point to the first block within the file
};

//定义表的索引节点信息
struct index_info
{
	CString index_name;        //the name of the index file
	int length;                //the length of the value
	char type;                 //the type of the value
							   //0---int,1---float,2----char(n)    
	long offset;               //the record offset in the table file
	CString value;             //the value
};

#endif