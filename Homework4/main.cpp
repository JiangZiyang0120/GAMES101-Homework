#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

#define CONTROL_POINTS_NUM 4

std::vector<cv::Point2f> control_points;

void mouse_handler(int event, int x, int y, int flags, void *userdata) {
    if (event == cv::EVENT_LBUTTONDOWN && control_points.size() <= CONTROL_POINTS_NUM) {
        std::cout << "Left button of the mouse is clicked - position (" << x << ", "
                  << y << ")" << '\n';
        control_points.emplace_back(x, y);
    }
}

void naive_bezier(const std::vector<cv::Point2f> &points, cv::Mat &window) {
    auto &p_0 = points[0];
    auto &p_1 = points[1];
    auto &p_2 = points[2];
    auto &p_3 = points[3];

    for (double t = 0.0; t <= 1.0; t += 0.001) {
        auto point = std::pow(1 - t, 3) * p_0 + 3 * t * std::pow(1 - t, 2) * p_1 +
                     3 * std::pow(t, 2) * (1 - t) * p_2 + std::pow(t, 3) * p_3;

        window.at<cv::Vec3b>(point.y, point.x)[2] = 255;
    }
}

cv::Point2f recursive_bezier(const std::vector<cv::Point2f> &control_points, float t) {
    // TODO: Implement de Casteljau's algorithm
    int size = control_points.size();
    int order = size - 1;
    int combination = 1;

    float t_pow[size], sub_t_pow[size], sub_t = 1 - t;
    t_pow[0] = 1;
    sub_t_pow[0] = 1;
    for (size_t i = 1; i != size; ++i) {
        t_pow[i] = t * t_pow[i - 1];
        sub_t_pow[i] = sub_t * sub_t_pow[i - 1];
    }

    cv::Point2f result(0, 0);

    for (int i = 0; i != size; ++i) {
        result += combination * t_pow[i] * sub_t_pow[size - i - 1] * control_points[i];
        combination = combination * (order - i) / (i + 1);
    }

    return result;
}

void bezier(const std::vector<cv::Point2f> &control_points, cv::Mat &window) {
    // TODO: Iterate through all t = 0 to t = 1 with small steps, and call de Casteljau's 
    // recursive Bezier algorithm.
    for (float t = 0.0f; t <= 1.0f; t += 0.001) {
        auto point = recursive_bezier(control_points, t);
        for (auto xv: {point.x - 1, point.x, point.x + 1})
            for (auto yv: {point.y - 1, point.y, point.y + 1}) {
            }
    }
}

int main() {
    cv::Mat window = cv::Mat(700, 700, CV_8UC3, cv::Scalar(0));
    cv::cvtColor(window, window, cv::COLOR_BGR2RGB);
    cv::namedWindow("Bezier Curve", cv::WINDOW_AUTOSIZE);

    cv::setMouseCallback("Bezier Curve", mouse_handler, nullptr);

    int key = -1;
    while (key != 27) {
        for (auto &point: control_points) {
            cv::circle(window, point, 3, {255, 255, 255}, 3);
        }

        if (control_points.size() == CONTROL_POINTS_NUM) {
//            naive_bezier(control_points, window);
            bezier(control_points, window);

            cv::imshow("Bezier Curve", window);
            cv::imwrite("my_bezier_curve.png", window);
            key = cv::waitKey(0);

            return 0;
        }

        cv::imshow("Bezier Curve", window);
        key = cv::waitKey(20);
    }

    return 0;
}
