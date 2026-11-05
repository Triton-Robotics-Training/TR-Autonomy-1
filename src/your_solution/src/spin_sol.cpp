#include "spin_sol.h"
#include <functional>
#include <memory>
#include <chrono>

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SpinSolution>());
  rclcpp::shutdown();
  return 0;
}

//your code here
SpinSolution::SpinSolution() : Node("spinsolution") {
  //RCLCPP_INFO(this->get_logger(), "Remove this statement from spin_sol.cpp");
  // your code here
  last_position_time_ = this->now();
  predicted_pos_pub_ = this->create_publisher<ArrayMsg>("/predictedpos", 10);
  measured_pos_sub_ =
        this->create_subscription<ArrayMsg>(
            "/measuredpos", 10, std::bind(&SpinSolution::position_callback, this, std::placeholders::_1)
        );
  measured_vel_sub_ =
        this->create_subscription<ArrayMsg>(
            "/measuredvel", 10, std::bind(&SpinSolution::velocity_callback, this, std::placeholders::_1)
        );
  timer_ = this->create_wall_timer(std::chrono::milliseconds(1), std::bind(&SpinSolution::prediction_callback, this)
    );
}
// Position subscriber callback
void SpinSolution::position_callback(const ArrayMsg::SharedPtr msg) {

    // Store measured position
    x = msg->data[0];
    y = msg->data[1];
    // Record measurement arrival time
    last_position_time_ = this->now();
    has_position_ = true;
}

// velocity subscriber callback
void SpinSolution::velocity_callback(const ArrayMsg::SharedPtr msg) {

    // Store measured velocity
    vx = msg->data[0];
    vy = msg->data[1];
    has_velocity_ = true;
}

// Timer callback: calculate and publish predicted position
void SpinSolution::prediction_callback() {
    // Wait until both measurements have been received
    if (!has_position_ || !has_velocity_) {
        return;
    }
    // Calculate elapsed time since the last position measurement
    rclcpp::Duration elapsed_time =
        this->now() - last_position_time_;

    double delta_t = elapsed_time.seconds();
    // Predict future position
    double predicted_x = x + delta_t * vx;
    double predicted_y = y + delta_t * vy;

    // Create prediction message
    auto predicted_msg = ArrayMsg();
    predicted_msg.data.push_back(predicted_x);
    predicted_msg.data.push_back(predicted_y);
    // Publish prediction
    predicted_pos_pub_->publish(predicted_msg);
}