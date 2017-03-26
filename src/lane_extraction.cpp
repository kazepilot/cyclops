#include <cmath>
#include "lane_extraction.h"

#include <iostream>

std::vector<json11::Json::object> LaneExtractor::extract(const cv::Mat image)
{
    cv::Scalar lower(0, 160, 0);
    cv::Scalar upper(100, 255, 80);
    cv::Mat hls, mask, masked, gray;
    int height, width;
    std::vector<cv::Point> results;
    std::vector< std::vector<cv::Point> > contours, lines;

    cv::cvtColor(image, hls, cv::COLOR_BGR2HLS);
    cv::inRange(hls, lower, upper, mask);
    cv::bitwise_and(image, image, masked, mask);
    cv::cvtColor(masked, gray, cv::COLOR_RGB2GRAY);

    height = gray.rows;
    width = gray.cols;

    cv::findContours(gray, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Point> approx, temp_line;
    double eps, area, min_b = 30;
    cv::Rect bounding;
    cv::Point topmost(width / 2, height / 2), bottommost(width / 2, height / 2);
    cv::Point line_func, side_pt(0, 0);

    for (auto &contour : contours)
    {
        eps = 0.01 * cv::arcLength(contour, true);
        cv::approxPolyDP(contour, approx, eps, true);
        area = cv::contourArea(approx);
    
        if (area > 30000)
            continue;

        bounding = cv::boundingRect(approx);
        if (bounding.width < 180 || bounding.height < 100)
            continue;

        for (auto &point : approx)
        {
            if (point.y > topmost.y)
                topmost = point;

            if (point.y < bottommost.y)
                bottommost = point;
        }

        line_func = line_function(topmost, bottommost);
        for (auto &point : approx)
        {
            double b = point.y - line_func.x * point.x;
            if (b > min_b)
            {
                min_b = b;
                side_pt = point;
            }
        }

        temp_line.push_back(topmost);
        if (side_pt.x > 0 || side_pt.y > 0)
            temp_line.push_back(side_pt);
        temp_line.push_back(bottommost);

        lines.push_back(temp_line);

        temp_line.clear();
    }

    std::cout << "After contours\n";

    cv::Point p1(0, height), p2(width, height);
    size_t elems, i;

    if (lines.size() > 1)
    {
        elems = lines[0].size();
        if (dist(p1, lines[0][elems - 1]) < dist(p2, lines[0][elems - 1]))
        {
            cv::Point temp;
            temp = p1;
            p1 = p2;
            p2 = temp;
        }

        results.push_back(p1);

        std::cout << "Setuped for loops\n";

        for(i = elems - 1; i > 0; i--)
            results.push_back(lines[0][i]);

        std::cout << "After first loop\n";

        for(i = 0; i < lines[1].size(); i++)
            results.push_back(lines[0][i]);

        std::cout << "After second loop\n";

        results.push_back(p2);
    }
    else if(lines.size() > 0)
    {
        elems = lines[0].size();
        if (dist(p1, lines[0][elems - 1]) < dist(p2, lines[0][elems - 1]))
        {
            cv::Point temp;
            temp = p1;
            p1 = p2;
            p2 = temp;
        }

        results.push_back(p1);

        for(i = elems; i >= 0; i--)
            results.push_back(lines[0][i]);
    
        results.push_back(p2);
    }

    std::cout << "Made results\n";

    return jsonify(results);
}

cv::Point LaneExtractor::line_function(cv::Point p1, cv::Point p2)
{
    double a = (p2.y - p1.y) / (p2.x - p1.x + 1e-5);
    double b = p1.y - a * p1.x;

    return cv::Point(a, b);
}

double LaneExtractor::dist(cv::Point p1, cv::Point p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

std::vector<json11::Json::object> LaneExtractor::jsonify(std::vector<cv::Point> points)
{
    std::vector<json11::Json::object> pts;
    json11::Json::object temp;
    for (auto &pt : points)
    {
        temp = json11::Json::object {
            { "x", pt.x },
            { "y", pt.y }
        };
        pts.push_back(temp);
    }
    return pts;
}