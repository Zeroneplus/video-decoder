1. ��C++���̿��������õ�yuv��Ƶ HeavyHand_1080p.yuv ���� https://developer.nvidia.com/heavy-hand ��վ
2. ʹ�õ���x264����������yuv�����h264ԭʼ������
3. ��ر�������Ϊ��
C:\x264-r3018-db0d417.exe --fps 25 --input-res 1920x1080 --output C:\HeavyHand_1080p.no_B_frames.no_cabac.h264 C:\HeavyHand_1080p.yuv
C:\x264-r3018-db0d417.exe --fps 25 --input-res 1920x1080 --tff --output C:\HeavyHand_1080p.no_B_frames.no_cabac.tff.h264 C:\HeavyHand_1080p.yuv
C:\x264-r3018-db0d417.exe --fps 25 --input-res 1920x1080 --no-cabac --output C:\HeavyHand_1080p.B_frames.no_cabac.h264 C:\HeavyHand_1080p.yuv
C:\x264-r3018-db0d417.exe --fps 25 --input-res 1920x1080 --tff --no-cabac --output C:\HeavyHand_1080p.B_frames.no_cabac.tff.h264 C:\HeavyHand_1080p.yuv
C:\x264-r3018-db0d417.exe --fps 25 --input-res 1920x1080 --bframes 0 --no-cabac --output C:\HeavyHand_1080p.no_B_frames.no_cabac.h264 C:\HeavyHand_1080p.yuv
C:\x264-r3018-db0d417.exe --fps 25 --input-res 1920x1080 --bframes 0 --no-cabac --tff --output C:\HeavyHand_1080p.no_B_frames.no_cabac.tff.h264 C:\HeavyHand_1080p.yuv
C:\x264-r3018-db0d417.exe --fps 25 --input-res 1920x1080 --bframes 0 --output C:\HeavyHand_1080p.no_B_frames.cabac.no_tff.h264 C:\HeavyHand_1080p.yuv
C:\x264-r3018-db0d417.exe --fps 25 --input-res 1920x1080 --bframes 0 --tff --output C:\HeavyHand_1080p.no_B_frames.cabac.tff.h264 C:\HeavyHand_1080p.yuv
C:\x264-r3018-db0d417.exe --fps 25 --input-res 1920x1080 --bframes 4 --no-cabac --output C:\HeavyHand_1080p.B_frames_4.no_cabac.no_tff.h264 C:\HeavyHand_1080p.yuv
