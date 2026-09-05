#include "spin_sol.h"

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SpinSolution>());
    rclcpp::shutdown();
    return 0;
}

//your code here
SpinSolution::SpinSolution() : Node("spinsolution"), last_measurement_time_(this->now()) {
    publisher_ = this->create_publisher<std_msgs::msg::Float64MultiArray>("predictedpos", 10);

    pos_subscription_ = this->create_subscription<std_msgs::msg::Float64MultiArray>(
        "measuredpos", 10, std::bind(&SpinSolution::measured_pos_callback, this, std::placeholders::_1));
    vel_subscription_ = this->create_subscription<std_msgs::msg::Float64MultiArray>(
        "measuredvel", 10, std::bind(&SpinSolution::measured_vel_callback, this, std::placeholders::_1));

    timer_ = this->create_wall_timer(
        std::chrono::milliseconds(100),
        std::bind(&SpinSolution::timer_callback, this)
    );
}

void SpinSolution::measured_pos_callback(const ArrayMsg::SharedPtr msg) {
    current_pos_ = std::array<double, 2>{
        msg->data[0],
        msg->data[1]
    };
    last_measurement_time_ = this->now();
}

void SpinSolution::measured_vel_callback(const ArrayMsg::SharedPtr msg) {
    current_vel_ = std::array<double, 2>{
        msg->data[0],
        msg->data[1]
    };
}

std::array<double, 2> SpinSolution::predict_position(
    std::array<double, 2> current_pos,
    std::array<double, 2> current_vel) {
    const double delta_t = (this->get_clock()->now() - last_measurement_time_).seconds();
    return {
        current_pos[0] + (current_vel[0] * delta_t),
        current_pos[1] + (current_vel[1] * delta_t)
    };
}
