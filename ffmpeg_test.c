
#include <stdio.h>

#include <libavformat/avformat.h>
#include <libavutil/avutil.h>

int main()
{
    const char* filename = "./test/trailer.h264";
    int ret, idx;
    const AVInputFormat* iformat = av_find_input_format("h264");
    AVFormatContext* format_ctx = avformat_alloc_context();
    AVPacket* pkt = av_packet_alloc();

    ret = avformat_open_input(&format_ctx, filename, iformat, NULL);
    if (ret < 0) {
        printf("avformat_open_input error\n");
        return -1;
    }

    ret = avformat_find_stream_info(format_ctx, NULL);
    if (ret < 0) {
        printf("avformat_find_stream_info error\n");
        return -1;
    }

    idx = av_find_best_stream(format_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (idx < 0) {
        printf("av_find_best_stream error\n");
        return -1;
    }

    while ((ret = av_read_frame(format_ctx, pkt)) >= 0) {
        printf("avpkt size is %d\n", pkt->size);
        av_packet_unref(pkt);
    }

    av_packet_free(&pkt);
    avformat_close_input(&format_ctx);
}