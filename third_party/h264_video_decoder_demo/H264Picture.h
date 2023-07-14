//
// H264Picture.h
// h264_video_decoder_demo
//
// Created by: 386520874@qq.com
// Date: 2019.09.01 - 2021.02.14
//

#ifndef __H264_PICTURE_H__
#define __H264_PICTURE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "H264PictureBase.h"


class CH264Picture
{
public:
    CH264PictureBase           m_picture_frame;
    CH264PictureBase           m_picture_top_filed;
    CH264PictureBase           m_picture_bottom_filed;
    CH264PictureBase *         m_current_picture_ptr; //ָ��m_picture_frame����m_picture_top_filed����m_picture_bottom_filed
    CH264PictureBase *         m_picture_previous_ref; //ǰһ���ѽ���Ĳο�ͼ��
    CH264PictureBase *         m_picture_previous; //ǰһ���ѽ����ͼ��
    H264_PICTURE_CODED_TYPE    m_picture_coded_type; //H264_PICTURE_CODED_TYPE_FRAME
    H264_PICTURE_CODED_TYPE    m_picture_coded_type_marked_as_refrence; //����֡����һ�������Ϊ�ο�֡��ο���
    
    int32_t         TopFieldOrderCnt;
    int32_t         BottomFieldOrderCnt;
    int32_t         PicOrderCntMsb;
    int32_t         PicOrderCntLsb;
    int32_t         FrameNumOffset;
    int32_t         absFrameNum;
    int32_t         picOrderCntCycleCnt;
    int32_t         frameNumInPicOrderCntCycle;
    int32_t         expectedPicOrderCnt;
    int32_t         PicOrderCnt;
    int32_t         PicNum; //To each short-term reference picture ���ڲο�ͼ��
    int32_t         LongTermPicNum; //To each long-term reference picture ���ڲο�ͼ��
    H264_PICTURE_MARKED_AS         reference_marked_type; //I,P��Ϊ�ο�֡��mark״̬
    int32_t         m_is_decode_finished; //��֡�Ƿ�������; 0-δ������ϣ�1-�Ѿ��������
    int32_t         m_is_in_use; //��֡�����Ƿ�����ʹ��; 0-δʹ�ã�1-����ʹ��

public:
    CH264Picture();
    ~CH264Picture();
    
    int init();
    int reset();

    int decode_one_slice(CH264SliceHeader &slice_header, CBitstream &bs, CH264Picture *(&dpb)[16]);
    CH264Picture & operator = (const CH264Picture &src);
    int copyPicture(const CH264Picture &src);
};

#endif //__H264_PICTURE_H__
