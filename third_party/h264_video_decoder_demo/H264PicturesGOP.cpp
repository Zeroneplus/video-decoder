//
// H264PicturesGOP.cpp
// h264_video_decoder_demo
//
// Created by: 386520874@qq.com
// Date: 2019.09.01 - 2021.02.14
//

#include "H264PicturesGOP.h"
#include "CommonFunction.h"
#include "H264CommonFunc.h"
#include <algorithm>


CH264PicturesGOP::CH264PicturesGOP()
{
    int ret = init();
}


CH264PicturesGOP::~CH264PicturesGOP()
{
    int ret = unInit();
}


int CH264PicturesGOP::init()
{
    int ret = 0;

    m_gop_size = 0;
    m_dpb_for_output_length = 0;
    max_num_reorder_frames = 0;
    
    int32_t size_dpb = H264_MAX_DECODED_PICTURE_BUFFER_COUNT;

    for (int i = 0; i < size_dpb; i++)
    {
        m_dpb_for_output[i] = NULL;

        m_DecodedPictureBuffer[i] = new CH264Picture;
        if (m_DecodedPictureBuffer[i] == NULL)
        {
            LOG_ERROR("new CH264Picture failed! m_DecodedPictureBuffer[%d] == NULL\n", i);
            return -1;
        }
    }

    return 0;
}


int CH264PicturesGOP::unInit()
{
    int ret = 0;
    int32_t size_dpb = H264_MAX_DECODED_PICTURE_BUFFER_COUNT;
    
    m_dpb_for_output_length = 0;
    max_num_reorder_frames = 0;

    for (int i = 0; i < size_dpb; i++)
    {
        SAFE_DELETE(m_DecodedPictureBuffer[i]);
    }

    return 0;
}


int CH264PicturesGOP::getOneEmptyPicture(CH264Picture *&pic)
{
    int ret = 0;
    int32_t size_dpb = H264_MAX_DECODED_PICTURE_BUFFER_COUNT;

    for (int i = 0; i < size_dpb; i++)
    {
        if (m_DecodedPictureBuffer[i]->m_picture_coded_type == H264_PICTURE_CODED_TYPE_UNKNOWN) //�ظ����ñ��ͷ��˵Ĳο�֡
        {
            pic = m_DecodedPictureBuffer[i];
            RETURN_IF_FAILED(pic == NULL, -1);
            return 0;
        }
    }

    return -1;
}


int CH264PicturesGOP::getOneOutPicture(CH264Picture *newDecodedPic, CH264Picture *&outPic)
{
    int32_t i = 0;
    int32_t index = -1;

    //----------����P֡���---------------
    if (max_num_reorder_frames == 0)
    {
        outPic = newDecodedPic; //ֱ�ӷ��أ�һ��ΪIP���룬��������û��B֡�����
        return 0;
    }
    
    //----------����B֡���---------------
    //���ҳ��Ѿ�������ϣ����һ�������DPB�е�POC��С����һ֡
    for (i = 0; i < m_dpb_for_output_length; ++i)
    {
        RETURN_IF_FAILED(m_dpb_for_output[i] == NULL, -1);

        if (i == 0 || m_dpb_for_output[i]->m_picture_frame.PicOrderCnt < m_dpb_for_output[index]->m_picture_frame.PicOrderCnt)
        {
            index = i;
        }
    }

    if (newDecodedPic == NULL) //һ���ʾ�������ʱ��flush����
    {
        if (m_dpb_for_output_length > 0) //˵��m_dpb_index_for_output[]���黹������δ�����֡
        {
            if (index >= 0)
            {
                outPic = m_dpb_for_output[index]; //���DPB�е�POC��С����һ֡
            }
            else //if (index < 0) //˵��������û��֡��
            {
                outPic = NULL;
                m_dpb_for_output_length = 0;
                return 0;
            }

            m_dpb_for_output_length--;

            for (i = index; i < m_dpb_for_output_length; ++i)
            {
                m_dpb_for_output[i] = m_dpb_for_output[i + 1]; //����Ԫ��������ǰ�ƶ�һ����Ԫ
            }
        }
        else
        {
            outPic = NULL; //˵�������г���û��֡��
        }
    }
    else //if (newDecodedPic != NULL) //һ���ʾ��������е�push_a_decoded_frame_and_get_a_display_frame����
    {
        if (m_dpb_for_output_length < max_num_reorder_frames) //˵��m_dpb_index_for_output[]���黹û������ֻpush����get
        {
            m_dpb_for_output[m_dpb_for_output_length] = newDecodedPic;
            m_dpb_for_output_length++;
            outPic = NULL; //Ŀǰ��û�п������֡
        }
        else //if (m_dpb_for_output_length >= max_num_reorder_frames) //˵��m_dpb_index_for_output[]������������get����push
        {
            RETURN_IF_FAILED(index < 0, -1);

            if (newDecodedPic->m_picture_frame.PicOrderCnt < m_dpb_for_output[index]->m_picture_frame.PicOrderCnt)
            {
                outPic = newDecodedPic; //˵����ǰ��������ѽ���֡��POC����С�ģ�һ��ΪB֡
            }
            else
            {
                outPic = m_dpb_for_output[index]; //���DPB�е�POC��С����һ֡
                m_dpb_for_output[index] = newDecodedPic; //�൱��push a frame
            }
        }
    }

    return 0;
}

