#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

#define X_AXIS 3
#define Y_AXIS 1
#define Z_AXIS -1

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos) {
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,
            -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle) {
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.

    rotation_angle = rotation_angle / 180 * M_PI;

    float cosV = cos(rotation_angle), sinV = sin(rotation_angle);

    model << cosV, -sinV, 0, 0,
            sinV, cosV, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;

    return model;
}


// Parameters:
// - eye_fov: the largest field angle of eyesight
// - aspect_ratio: the ratio of horizon pixels with vertical pixels
Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar) {
    // Students will implement this function
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.

    Eigen::Matrix4f per2orth = Eigen::Matrix4f::Identity();
    per2orth << zNear, 0, 0, 0,
            0, zNear, 0, 0,
            0, 0, zNear + zFar, -zFar * zNear,
            0, 0, 1, 0;

    float l, r, b, t;
    t = zNear * static_cast<float>(tan(eye_fov / 180 * MY_PI)) * 0.5;
    b = -t;
    l = aspect_ratio * t;
    r = -l;

    Eigen::Matrix4f orth;
    orth << 2 / (r - l), 0, 0, -(r + l) / (r - l),
            0, 2 / (t - b), 0, -(t + b) / (t - b),
            0, 0, 2 / (zNear - zFar), -(zNear + zFar) / (zNear - zFar),
            0, 0, 0, 1;

    projection = orth * per2orth;

    return projection;
}

Eigen::Matrix4f get_rotation(Eigen::Vector3f axis, float angle) {
    float radian = angle / 180 * MY_PI;
    float cosV = cos(radian), sinV = sin(radian);
    Eigen::Matrix3f rotation = Eigen::Matrix3f::Identity(),
            cross = Eigen::Matrix3f::Identity();
    cross << 0, -axis[2], axis[1],
            axis[2], 0, -axis[0],
            -axis[1], axis[0], 0;

    rotation = cosV * Eigen::Matrix3f::Identity()
               + (1 - cosV) * (axis * axis.transpose())
               + sinV * cross;

    Eigen::Matrix4f rotation4D = Eigen::Matrix4f::Identity();
    for (size_t i = 0; i != 3; ++i)
        for (size_t j = 0; j != 3; ++j)
            rotation4D(i, j) = rotation(i, j);


    return rotation4D;
}

int main(int argc, const char **argv) {
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2,  0, -2},
                                     {0,  2, -2},
                                     {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};
    Eigen::Vector3f axis({X_AXIS, Y_AXIS, Z_AXIS});

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        // r.set_model(get_model_matrix(angle));
        r.set_model(get_rotation(axis, angle));

        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(60, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        // r.set_model(get_model_matrix(angle));
        r.set_model(get_rotation(axis, angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 10;
        } else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}
