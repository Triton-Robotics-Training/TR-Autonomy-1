#include "spin_sol.h"

#include <chrono>
#include <functional>

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SpinSolution>());
  rclcpp::shutdown();
  return 0;
}

SpinSolution::SpinSolution() : Node("spinsolution"), last_time_(this->now()) {
  pos_sub_ = this->create_subscription<ArrayMsg>(
      "measuredpos", 10, std::bind(&SpinSolution::pos_callback, this, std::placeholders::_1));
  vel_sub_ = this->create_subscription<ArrayMsg>(
      "measuredvel", 10, std::bind(&SpinSolution::vel_callback, this, std::placeholders::_1));
  pred_pub_ = this->create_publisher<ArrayMsg>("predictedpos", 10);

  // predict every 1 ms, much faster than the 1.5 s measurements
  timer_ = this->create_wall_timer(
      std::chrono::milliseconds(1), std::bind(&SpinSolution::timer_callback, this));
}

void SpinSolution::pos_callback(const ArrayMsg::SharedPtr msg) {
  if (msg->data.size() < 2) return;
  x_ = msg->data[0];
  y_ = msg->data[1];
  last_time_ = this->now();
  have_measurement_ = true;
}

void SpinSolution::vel_callback(const ArrayMsg::SharedPtr msg) {
  if (msg->data.size() < 2) return;
  vx_ = msg->data[0];
  vy_ = msg->data[1];
}

void SpinSolution::timer_callback() {
  if (!have_measurement_) return;

  // x_p = x + dt * v
  double dt = (this->now() - last_time_).seconds();
  ArrayMsg msg;
  msg.data = {x_ + dt * vx_, y_ + dt * vy_};
  pred_pub_->publish(msg);
}
