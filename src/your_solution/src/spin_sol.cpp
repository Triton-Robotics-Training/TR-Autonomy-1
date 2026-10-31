#include "spin_sol.h"

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SpinSolution>());
  rclcpp::shutdown();
  return 0;
}

SpinSolution::SpinSolution() : Node("spinsolution") {
    // sub to pos
    pos_sub_ = this->create_subscription<ArrayMsg>(
        "/measuredpos", 10,
        std::bind(&SpinSolution::pos_cb, this, std::placeholders::_1));

    // sub to vel
    vel_sub_ = this->create_subscription<ArrayMsg>(
        "/measuredvel", 10,
        std::bind(&SpinSolution::vel_cb, this, std::placeholders::_1));

    // pub predictions
    pub_ = this->create_publisher<ArrayMsg>("/predictedpos", 10);

    //timer
    timer_ = this->create_wall_timer(
        std::chrono::milliseconds(5),
        std::bind(&SpinSolution::timer_cb, this));
}

void SpinSolution::pos_cb(const ArrayMsg::SharedPtr msg) {
    latest_pos_ = *msg;
    last_msg_time_ = this->now();
    received_data_ = true;
}

void SpinSolution::vel_cb(const ArrayMsg::SharedPtr msg) {
    latest_vel_ = *msg;
}

void SpinSolution::timer_cb() {
    if (!received_data_ || latest_pos_.data.size() < 2 || latest_vel_.data.size() < 2) {
        return;
    }

    double dt = (this->now() - last_msg_time_).seconds();

    ArrayMsg predicted_msg = latest_pos_;

    predicted_msg.data[0] = latest_pos_.data[0] + (latest_vel_.data[0] * dt);
    predicted_msg.data[1] = latest_pos_.data[1] + (latest_vel_.data[1] * dt);

    pub_->publish(predicted_msg);
}
