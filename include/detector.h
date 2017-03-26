#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

class Detector
{
    public:
        Detector(std::string datacfg,
                 std::string cfgfile,
                 std::string weights,
                 float threshold,
                 float hier_threshold,
                 float nms);
        std::string detect(cv::Mat image);

    private:
        cv::Mat m_resized;
        cv::Size m_size;
};