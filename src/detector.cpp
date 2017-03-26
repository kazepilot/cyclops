#include "detector.h"
#include <iostream>
#include <darknet/kaze_eye.h>

Detector::Detector(std::string datacfg,
                 std::string cfgfile,
                 std::string weights,
                 float threshold,
                 float hier_threshold,
                 float nms)
    {

        kaze_init(datacfg.c_str(), cfgfile.c_str(), weights.c_str(), threshold, hier_threshold, nms);
        m_size = cv::Size(416, 416);
    }


std::string Detector::detect(cv::Mat image)
{
    char *results = calloc(1, 500 * sizeof(char));
    int height = image.rows;
    int width = image.cols;
    int channels = image.channels();
    int step = (int)(image.step1());
  
    kaze_predict(height, width, channels, step, image.data, results);  
    std::string str(results);
    free(results);
    return str;
}