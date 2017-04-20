#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <thread>

#include "crow_all.h"
#include "json11.hpp"
#include "detector.h"
#include "lane_extraction.h"

int main()
{
    crow::SimpleApp app;
    std::string datacfg = "/home/evaldas/cyclops/cyclops.data";
    std::string cfgfile = "/home/evaldas/cyclops/cyclops.cfg";
    std::string weights = "/home/evaldas/cyclops/cyclops.weights";

    Detector detector(datacfg, cfgfile, weights, 0.1, 0.5, 0.4);
    LaneExtractor extractor;

    CROW_ROUTE(app, "/cyclops")
    .methods("POST"_method)
    ([&](const crow::request& req)
    {
        std::vector<unsigned char> encoded(req.body.begin(), req.body.end());
        cv::Mat image = cv::imdecode(cv::Mat(encoded), 1);
        std::vector<json11::Json::object> detections;
        detector.detect(image, detections);

        json11::Json::object results = json11::Json::object {
            { "objects", detections }
        };

        return json11::Json(results).dump();
    });

    CROW_ROUTE(app, "/hello")
    .name("hello")
    ([]{
        return "Hello World!";
    });

    app.port(80)
        .multithreaded()
        .run();
}
