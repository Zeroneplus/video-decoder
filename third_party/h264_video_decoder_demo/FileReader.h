//
// FileReader.h
// h264_video_decoder_demo
//
// Created by: 386520874@qq.com
// Date: 2019.09.01 - 2021.02.14
//

#ifndef __FILE_READER_H__
#define __FILE_READER_H__

#include <stdint.h>
#include <string>

class CFileReader
{
public:
    std::string        m_filename;
    FILE *             m_fp;
    int                m_file_offset; //�ļ�ƫ����
    int                m_buffer_size;
    unsigned char *    m_buffer_start;
    unsigned char *    m_buffer_end; //��Ϊ�������ݲ�һ����������m_buffer[0..m_buffer_size]��������Ҫһ��endָ��
    unsigned char *    m_buffer_pointer_pos;

public:
    CFileReader();
    ~CFileReader();

    int init(const char *filename);
    int unInit();
    
    int getNextStartCode(unsigned char *data, int size, unsigned char *&pos1, unsigned char *&pos2,int &startCodeLength1,int &startCodeLength2);
    int getNextH264NalUnitByStartCode(unsigned char *&data, int &size);
};

#endif //__FILE_READER_H__
