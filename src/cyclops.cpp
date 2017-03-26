#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "crow_all.h"
#include "json11.hpp"
#include "detector.h"
#include "lane_extraction.h"

#include <iostream>

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
        std::vector<json11::Json::object> detections = detector.detect(image);
        std::cout << "Detector finished\n";
        std::vector<json11::Json::object> extractions = extractor.extract(image);
        std::cout << "Extractor finished\n";
        json11::Json::object results = json11::Json::object {
            { "objects", detections },
            { "road", extractions }
        };

        std::cout << "Merged objects\n";

        return json11::Json(results).dump();
    });

    app.port(1234)
        .multithreaded()
        .run();
}
