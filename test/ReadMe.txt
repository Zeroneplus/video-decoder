
https://gist.github.com/commshare/51ee9e3a11ac86c1b483
http://trace.eas.asu.edu/yuv/

x264 --no-8x8dct --no-cabac --fps 25 --input-res 352x288 --output akiyo_cif_352x288.h264 akiyo_cif_352x288.yuv

x264 --no-8x8dct --no-cabac --fps 25 --input-res 176x144 --output akiyo_qcif_176x144.h264 akiyo_qcif_176x144.yuv

x264 --no-8x8dct --no-cabac --fps 25 --input-res 352x288 --output bus_cif_352x288.h264 bus_cif_352x288.yuv

x264 --no-8x8dct --no-cabac --fps 25 --input-res 352x288 --output bus_cif_352x288.h264 waterfall_cif.yuv

x264 --no-8x8dct --no-cabac --fps 25 --input-res 1920x1080 --output HeavyHand_1920x1080.h264 HeavyHand_1080p.yuv

ffplay -hide_banner -f rawvideo -video_size 176x144  -pixel_format yuv420p -i 0000_I.yuv420p

ffplay -hide_banner -f rawvideo -video_size 352x288  -pixel_format yuv420p -i 0000_I.yuv420p

ffplay -hide_banner -f rawvideo -video_size 1920x1088  -pixel_format yuv420p -i 0000_I.yuv420p
