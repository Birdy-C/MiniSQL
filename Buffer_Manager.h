//////////////////////////////////////////////////////////
///----------------------------------------------------///
///       Module: Buffer_Manager                           ///
///       Produced by: Birdy                          ///
///       Description:��¼����ģ�飨Record Manager������������ģ�飨Index Manager��
///�򻺳�������������Ҫ�����ݣ������������������ڻ������в鿴�����Ƿ���ڣ������ڣ�
///ֱ�ӷ��أ����򣬴Ӵ����н����ݶ��뻺������Ȼ�󷵻ء�       ///
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
	DB_NameΪ���ݿ����ơ�����ʵ�ִ����ѷ�����ڴ浥Ԫ�л�ȡ�����滻���ڴ�顣
	�����ж���������blockHandle���Ƿ��п��õ��ڴ�飬����д������ͷȡһ�鷵�ء�
	��û�п��ÿ飬ʹ��LRU�㷨�ҵ�û��ʹ�õ�ʱ����Ŀ飨itime��󣩣���������λ��ȡ����itime���㣻�����ÿ鷵�ء�
*/
blockInfo *findBlock��CString DB_Name��;
/*
	ʵ�ְ�ָ��m_blockInfo ��ָ��Ŀ������ļ�ͷָ��m_fileInfo��ָ��Ŀ�����Ľ�β��ͬʱ��m_blockInfo��ָ��
	�Ŀ��fileָ��ָ��m_fileInfo��
*/
void replace(fileInfo * m_fileInfo, blockInfo * m_blockInfo);
	/*�����ļ������ļ����Ͳ��Ҹ��ļ��Ƿ����ڴ棬
	����ǣ������ļ��Ŀ�ţ����ڴ��в�ѯ�ÿ飬
	��ÿ��Ѿ����ڴ棬���ظÿ��ָ�룬
	����ÿ�û�����ڴ棬�ж������������Ƿ��п���Ŀ飬
	���д����ҵ�һ�飨�����ͷ�������ÿ��blockNUM����Ϊ����blockNUM��
	���û�У��ж������ڴ��еĿ����Ƿ��Ѿ��ﵽ������������
	���û�У�Ϊ������·���һ�飬�����ӵ���Ӧ���ļ�ͷ��ָ������Ľ�β��
	����Ѵﵽ��ʹ��LRU�㷨���ҵ�һ���滻�飬���ո�����Ҫ����г�ʼ�������������ӵ�ָ���ļ�������Ľ�β��
	����ļ�û�����ڴ棬����get_file_info(CString DB_Name,CString fileName, int m_fileType)��Ϊ�ļ�����һ��ͷָ�롣Ȼ���blockHandle���·����ʹ��LRU�㷨�ҵ�һ���滻�飨�������������ƣ������ÿ鰴Ҫ���ʼ����
	*/
blockInfo *get_file_block(CString DB_Name, CString Table_Name, int fileType, int blockNum);

/*
	����closeFile(DB_Name,filename,fileType,m_flag),����ر��ļ���
*/
void closeDatabase(CString DB_Name, bool m_flag);

/*
	�����ļ������ļ����ͣ������ڴ��Ƿ�������ļ������û�к�������
	����ҵ����ļ�����1�����ڿ������е�ÿ���飬����dirtyλΪ1������writeBlock��DB_Name����ָ�룩�����ÿ������д�ش��̣����dirtyλΪ0����������Ȼ������������ӵ����������С��ͷ��ļ�ͷ��struct ��
*/
void closeFile(CString DB_Name, CString m_fileName, int m_fileType, bool m_flag);
/*
	��blockָ����ָ��Ŀ������д�ش��̡�
*/
void writeBlock(CString DB_Name, blockInfo * block);

/*
	���Ȳ��Ҹ��ļ��Ƿ��Ѿ����ڴ��д��ڣ�����ǣ����ظ��ļ�ͷ��
	��������ڣ��ж��ڴ������е��ļ����Ƿ���������ļ�����
	������ǣ�Ϊ�����һ���ļ�ͷ��struct��������Ҫ����г�ʼ�����ļ�������һ������get_tablei_info(DB_Name,fileName,fileinfo->recordAmount,fileinfo->recordLength,fileinfo->freeNum)��get_index_info(DB_Name,fileName,fileinfo->recordAmount,fileinfo->recordLength,fileinfo->freeNum)�Ը��ļ�ͷ���г�ʼ���������ظ��ļ�ͷ��
	������ǣ�ѡȡ�ļ�ͷ����ĵ�һ���ļ�ͷ����Ϊ�滻����ȶ�ѡ�����ļ����йر��ļ�������ʹ���ڴ��е��ļ���Ŀ����һ��������Ϊ������������һ���ļ�ͷ������get_tablei_info(DB_Name,fileName,fileinfo->recordAmount,fileinfo->recordLength,fileinfo->freeNum)��get_index_info(DB_Name,fileName,fileinfo->recordAmount,fileinfo->recordLength,fileinfo->freeNum)�Ը��ļ�ͷ���г�ʼ�������ظ��ļ�ͷ��
*/
fileInfo* get_file_info(CString DB_Name, CString fileName, int m_fileType);
//���ȵ���block_in_buffer��CString DB_Name,CString m_fileName, int m_blockNum, int m_fileType����������Ѿ����ڴ棬�Ͳ��ٴӴ�����ȡ��ȡ�ÿ飬ֱ�ӷ��ظÿ飻��������ڴ棬���ȵ���get_file_block(CString DB_Name,CString m_fileName, int m_blockNum, int m_fileType)����ȡһ���飬�Ӵ����ж�ȡ�ÿ�����ݵ��ڴ棬���ظÿ顣
blockInfo* readBlock(CString DB_Name, CString m_fileName, int m_blockNum, int m_fileType);
//����closeFile(DB_Name,temp->fileName, temp->type, true)�����ر������Ѵ򿪵��ļ����ͷ��������������еĿ顣
void quitProg(CString DB_Name);
//��ȡ���ļ���Ϣ��
void Get_Table_Info(CString DB_Name, CString Table_Name, int &record_count, int & record_len, int & free);
//	��ȡ�����ļ���Ϣ��
void Get_Index_Info(CString DB_Name, CString Index_Name, int & record_count, int & record_len, int & free);
//	�����ļ���Ϣд�ش��̡�
void Write_Table_Info(CString DB_Name, CString Table_Name, int & record_count, int & record_len, int & free);
//	�������ļ���Ϣд�ش��̡�
void Write_Index_Info(CString DB_Name, CString Index_Name, int & record_count, int & record_len, int & free);

#endif