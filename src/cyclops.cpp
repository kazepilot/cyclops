#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "crow_all.h"

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/cyclops")
    .methods("POST"_method)
    ([&](const crow::request& req)
    {
        std::vector<unsigned char> encoded(req.body.begin(), req.body.end());

        cv::Mat image = cv::imdecode(cv::Mat(encoded), 1);

        return "Image decoded";
    });

    app.port(1234)
        .multithreaded()
        .run();
}
