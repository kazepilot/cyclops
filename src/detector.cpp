#include <iostream>
#include <darknet/kaze_eye.h>
#include <sstream>

#include "detector.h"
#include "json11.hpp"
#include "detection.h"

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
    char *results = calloc(1, 1000 * sizeof(char));
    int height = image.rows;
    int width = image.cols;
    int channels = image.channels();
    int step = (int)(image.step1());

    // to prevent concurrent inputs for GPU
    std::lock_guard<std::mutex> lock(m_mutex);
    kaze_predict(height, width, channels, step, image.data, results);

    std::string str(results);
    free(results);

    std::string line;
    std::vector<Detection> detections;
    Detection detection;
    std::stringstream ss(str);

    while (std::getline(ss, line))
    {
        detection = Detection(line);
        detections.push_back(detection);
    }
    return json11::Json(detections).dump();
}