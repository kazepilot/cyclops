#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "crow_all.h"
#include "detector.h"

int main()
{
    crow::SimpleApp app;
    std::string datacfg = "/home/evaldas/cyclops/cyclops.data";
    std::string cfgfile = "/home/evaldas/cyclops/cyclops.cfg";
    std::string weights = "/home/evaldas/cyclops/cyclops.weights";
    Detector detector(datacfg, cfgfile, weights, 0.1, 0.5, 0.4);

    CROW_ROUTE(app, "/cyclops")
    .methods("POST"_method)
    ([&](const crow::request& req)
    {
        std::vector<unsigned char> encoded(req.body.begin(), req.body.end());

        cv::Mat image = cv::imdecode(cv::Mat(encoded), 1);
        std::string results = detector.detect(image);

        return results;
    });

    app.port(1234)
        .multithreaded()
        .run();
}
