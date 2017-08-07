#include "YuvTest.h"

USING_NS_CC;

template<typename T>
T clamp(T value, T min_value, T max_value)
{
    if (value<min_value)
    {
        return min_value;
    }
    else if (value>max_value)
    {
        return max_value;
    }
    else
    {
        return value;
    }
}

cocos2d::Image* yuvToImage(const std::string& yuv_file_name)
{
    Data data = FileUtils::getInstance()->getDataFromFile(yuv_file_name);
    unsigned char* src_data = data.getBytes();
    int width = 540;
    int height = 720;
    unsigned char* u_data = src_data + width*height;
    unsigned char* v_data = u_data + width*height / 4;

    unsigned char* target_data = new unsigned char[width*height*4];
//     for (int i = 0; i < width*height; ++i)
//     {
//         byte y = src_data[i];
//         byte u = src_data[width*height + i / 4];
//         byte v = src_data[(int)(width*height*1.25) + i / 4];
//         target_data[i * 4] = clamp((int)(y + 1.4075*(v - 128)),0,255);
//         target_data[i * 4 + 1] = clamp((int)(y - 0.3455*(u - 128) - 0.7169*(v - 128)),0,255);
//         target_data[i * 4 + 2] = clamp((int)(y + 1.779*(u - 128)),0,255);
//         target_data[i * 4 + 3] = 0xff;
// //         byte y = src_data[i];
// //         byte u = src_data[width*height + i / 4];
// //         byte v = src_data[(int)(width*height*1.25) + i / 4];
// //         target_data[i * 4] = ((y - 16.0 / 255.0)*1.164 + 1.596*v - 128.0 / 255.0)*255;
// //         target_data[i * 4 + 1] = ((y - 16.0 / 255.0)*1.164 - 0.813*v - 128.0 / 255.0 - 0.392*u - 128.0 / 255.0)*255;
// //         target_data[i * 4 + 2] = ((y - 16.0 / 255.0)*1.164 + 2.017*u - 128.0 / 255.0)*255;
// //         target_data[i * 4 + 3] = 0xff;
//     }
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            byte y = src_data[i*width + j];
            byte u = u_data[(i / 2) * (width / 2) + (j / 2)];
            byte v = v_data[(i / 2) * (width / 2) + (j / 2)];
            target_data[(i*width + j) * 4] = clamp((int)(y + 1.4075*(v - 128)), 0, 255);
            target_data[(i*width + j) * 4 + 1] = clamp((int)(y - 0.3455*(u - 128) - 0.7169*(v - 128)), 0, 255);
            target_data[(i*width + j) * 4 + 2] = clamp((int)(y + 1.779*(u - 128)), 0, 255);
            target_data[(i*width + j) * 4 + 3] = 0xff;
        }
    }
    Image* img = new Image();
    img->initWithRawData(target_data, width*height * 4, width, height, 4);
    img->saveToFile("C:/Users/ljh/AppData/Local/LightGame/yuv_to_rgba.png", false);
    return img;
}
