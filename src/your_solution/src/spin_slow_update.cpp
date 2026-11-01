#include "spin_slow_update.h"

using std::placeholders::_1;

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SlowSolution>());
  rclcpp::shutdown();
  return 0;
}

SlowSolution::SlowSolution() : Node("slowsolution") {
  pos_sub_ = create_subscription<ArrayMsg>(
      "/measuredpos", 10, std::bind(&SlowSolution::pos_callback, this, _1));
  pred_pub_ = create_publisher<ArrayMsg>("/predictedpos", 10);
}

void SlowSolution::pos_callback(const ArrayMsg::SharedPtr msg) {
  pred_pub_->publish(*msg);
}
