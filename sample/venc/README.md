1）功能说明：
该模块是爱芯SDK包提供的视频(h264/h265/jpeg/mjpeg)编码单元示例代码，方便客户快速理解和掌握视频编码相关接口的使用。
代码演示了如下流程：视频编码模块初始化、编码Send线程发送帧数据、编码Get线程获取和保存码流数据、视频编码模块去初始化。
编译后得到可执行程序axcl_sample_venc,位于/opt/bin/axcl目录，可用于视频编码功能验证。

-w：配置源数据width
-h：配置源数据height
-i ：输入的源数据路径
-l：输入源数据YUV格式,（1: I420; 3: NV12; 4: NV21; 13: YUYV422; 14: UYVY422）默认1
-N ：配置编码通道数目，默认开启四路通道编码，分别编码 h264/h265/mjpeg/jpeg
-n：开启循环编码后，指定编多少帧
-W: 编码后码流是否写入文件。（默认为1，写入。0：不写入）

2）使用示例：
举例一：查看help信息
axcl_sample_venc -H

举例二：开启两路通道编码1080p NV12格式（通道0：h264，通道1：h265）
axcl_sample_venc -w 1920 -h 1080 -i  1080p_nv12.yuv  -N 2  -l 3

举例三：开启两路通道循环编码3840x2160 NV21格式（通道0：h264，通道1：h265），编码10帧
axcl_sample_venc -w 3840 -h 2160 -i  3840x2160_nv21.yuv  -N 2  -l 4  -n 10

举例四：指定编码一路Mjpeg码流，分辨率为1920x1080,yuv420p,编码5帧
axcl_sample_venc -w 1920 -h 1080 -i  1920x1080_yuv420p.yuv  -N 1 --bChnCustom 1 --codecType 2  -l 1  -n 5

3）运行结果：
运行成功后，执行Ctrl+C退出，在当前目录应生成码流文件，名称如 264/265/jpg/mjpg 后缀，用户可打开看实际效果。

4）注意事项：
a) 示例代码中部分参数可能不是最优，仅作为API演示。实际开发中，用户需结合具体业务场景配置参数。
b) h264/h265 支持最大分辨率为 8192x8192
c) jpeg/mjpeg 支持最大分辨率为 16384x16384
