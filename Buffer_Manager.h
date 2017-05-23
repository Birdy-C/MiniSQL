//////////////////////////////////////////////////////////
///----------------------------------------------------///
///       Module: Buffer_Manager                           ///
///       Produced by: Birdy                          ///
///       Description:记录管理模块（Record Manager）和索引管理模块（Index Manager）
///向缓冲区管理申请所要的数据，缓冲区管理器首先在缓冲区中查看数据是否存在，若存在，
///直接返回，否则，从磁盘中将数据读入缓冲区，然后返回。       ///
///                    functions between Interpreter,  ///
///                    Record_Maner and Index_Manager///
///       date: 2017/5/23                             ///
///----------------------------------------------------///
//////////////////////////////////////////////////////////

#if !defined(_BUFFER_MANAGER_H_)
#define _BUFFER_MANAGER_H_

#include"MiniSQL.h"

#include "Buffer_Manager.h"

/*
	DB_Name为数据库名称。函数实现从现已分配的内存单元中获取用于替换的内存块。
	首先判断垃圾链表blockHandle中是否有可用的内存块，如果有从链表的头取一块返回。
	如没有可用块，使用LRU算法找到没有使用的时间最长的块（itime最大），从其现在位置取出，itime置零；并将该块返回。
*/
blockInfo *findBlock（CString DB_Name）;
/*
	实现把指针m_blockInfo 所指向的块连到文件头指针m_fileInfo所指向的块链表的结尾，同时将m_blockInfo所指向
	的块的file指针指向m_fileInfo。
*/
void replace(fileInfo * m_fileInfo, blockInfo * m_blockInfo);
	/*根据文件名，文件类型查找该文件是否在内存，
	如果是，根据文件的块号，从内存中查询该块，
	如该块已经在内存，返回该块的指针，
	如果该块没有在内存，判断垃圾链表中是否有空余的块，
	如有从中找到一块（链表的头），将该块的blockNUM设置为参数blockNUM，
	如果没有，判断现在内存中的块数是否已经达到了最大块数限制
	如果没有，为其分配新分配一块，并链接到对应的文件头所指的链表的结尾；
	如果已达到，使用LRU算法，找到一个替换块，按照给定的要求进行初始化，并将其链接到指定文件块链表的结尾。
	如果文件没有在内存，调用get_file_info(CString DB_Name,CString fileName, int m_fileType)来为文件分配一个头指针。然后从blockHandle或新分配或使用LRU算法找到一个替换块（方法与上面类似），将该块按要求初始化。
	*/
blockInfo *get_file_block(CString DB_Name, CString Table_Name, int fileType, int blockNum);

/*
	调用closeFile(DB_Name,filename,fileType,m_flag),逐个关闭文件。
*/
void closeDatabase(CString DB_Name, bool m_flag);

/*
	根据文件名和文件类型，查找内存是否有这个文件，如果没有函数结束
	如果找到，文件数减1，对于块链表中的每个块，而且dirty位为1，调用writeBlock（DB_Name，块指针），将该块的内容写回磁盘，如果dirty位为0，不作处理。然后把真个块链表加到垃圾链表中。释放文件头的struct 。
*/
void closeFile(CString DB_Name, CString m_fileName, int m_fileType, bool m_flag);
/*
	把block指针所指向的块的内容写回磁盘。
*/
void writeBlock(CString DB_Name, blockInfo * block);

/*
	首先查找该文件是否已经在内存中存在，如果是，返回该文件头；
	如果不存在，判断内存中已有的文件数是否等于最大的文件数，
	如果不是，为其分配一个文件头的struct，并根据要求进行初始化，文件个数加一，调用get_tablei_info(DB_Name,fileName,fileinfo->recordAmount,fileinfo->recordLength,fileinfo->freeNum)或get_index_info(DB_Name,fileName,fileinfo->recordAmount,fileinfo->recordLength,fileinfo->freeNum)对给文件头进行初始化，并返回该文件头；
	如果不是，选取文件头链表的第一个文件头，作为替换项，首先对选定的文件进行关闭文件操作，使得内存中的文件数目减少一个，进而为可以重新申请一个文件头，调用get_tablei_info(DB_Name,fileName,fileinfo->recordAmount,fileinfo->recordLength,fileinfo->freeNum)或get_index_info(DB_Name,fileName,fileinfo->recordAmount,fileinfo->recordLength,fileinfo->freeNum)对给文件头进行初始化，返回该文件头。
*/
fileInfo* get_file_info(CString DB_Name, CString fileName, int m_fileType);
//首先调用block_in_buffer（CString DB_Name,CString m_fileName, int m_blockNum, int m_fileType），如果块已经在内存，就不再从磁盘中取读取该块，直接返回该块；如果不再内存，首先调用get_file_block(CString DB_Name,CString m_fileName, int m_blockNum, int m_fileType)来获取一个块，从磁盘中读取该块的内容到内存，返回该块。
blockInfo* readBlock(CString DB_Name, CString m_fileName, int m_blockNum, int m_fileType);
//调用closeFile(DB_Name,temp->fileName, temp->type, true)，来关闭所有已打开的文件，释放垃圾链表中所有的块。
void quitProg(CString DB_Name);
//读取表文件信息。
void Get_Table_Info(CString DB_Name, CString Table_Name, int &record_count, int & record_len, int & free);
//	读取索引文件信息。
void Get_Index_Info(CString DB_Name, CString Index_Name, int & record_count, int & record_len, int & free);
//	将表文件信息写回磁盘。
void Write_Table_Info(CString DB_Name, CString Table_Name, int & record_count, int & record_len, int & free);
//	将索引文件信息写回磁盘。
void Write_Index_Info(CString DB_Name, CString Index_Name, int & record_count, int & record_len, int & free);

#endif