#include <string>
#include <mutex>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "json11.hpp"

class Detector
{
    public:
        Detector(std::string datacfg,
                 std::string cfgfile,
                 std::string weights,
                 float threshold,
                 float hier_threshold,
                 float nms);
        void detect(const cv::Mat image, std::vector<json11::Json::object> &results_json);

    private:
        cv::Mat m_resized;
        cv::Size m_size;
        std::mutex m_mutex;
};