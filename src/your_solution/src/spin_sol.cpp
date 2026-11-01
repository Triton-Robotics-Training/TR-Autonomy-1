#include "spin_sol.h"

using namespace std::chrono_literals;
using std::placeholders::_1;

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SpinSolution>());
  rclcpp::shutdown();
  return 0;
}

SpinSolution::SpinSolution() : Node("spinsolution"), last_meas_time_(this->now()) {
  pos_sub_ = create_subscription<ArrayMsg>(
      "/measuredpos", 10, std::bind(&SpinSolution::pos_callback, this, _1));
  vel_sub_ = create_subscription<ArrayMsg>(
      "/measuredvel", 10, std::bind(&SpinSolution::vel_callback, this, _1));
  pred_pub_ = create_publisher<ArrayMsg>("/predictedpos", 10);
  timer_ = create_wall_timer(1ms, std::bind(&SpinSolution::timer_callback, this));
}

void SpinSolution::pos_callback(const ArrayMsg::SharedPtr msg) {
  x_ = msg->data[0];
  y_ = msg->data[1];
  last_meas_time_ = this->now();
  have_pos_ = true;
}

void SpinSolution::vel_callback(const ArrayMsg::SharedPtr msg) {
  vx_ = msg->data[0];
  vy_ = msg->data[1];
  have_vel_ = true;
}

void SpinSolution::timer_callback() {
  if (!have_pos_ || !have_vel_) return;
  double dt = (this->now() - last_meas_time_).seconds();
  ArrayMsg out;
  out.data = {x_ + dt * vx_, y_ + dt * vy_};
  pred_pub_->publish(out);
}
