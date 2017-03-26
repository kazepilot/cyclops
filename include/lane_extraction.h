#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "json11.hpp"

class LaneExtractor
{
    public:
        LaneExtractor() { }
        std::vector<json11::Json::object> extract(const cv::Mat image);

    private:
        cv::Point line_function(cv::Point p1, cv::Point p2);
        double dist(cv::Point p1, cv::Point p2);
        std::vector<json11::Json::object> jsonify(std::vector<cv::Point> points);
};