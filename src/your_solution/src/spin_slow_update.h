#ifndef YOUR_SOLUTION_SRC_SPIN_SLOW_UPDATE_H_
#define YOUR_SOLUTION_SRC_SPIN_SLOW_UPDATE_H_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
using ArrayMsg = std_msgs::msg::Float64MultiArray;

class SlowSolution : public rclcpp::Node {
 public:
  SlowSolution();
 private:
  rclcpp::Subscription<ArrayMsg>::SharedPtr sub_;
  rclcpp::Publisher<ArrayMsg>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  ArrayMsg latest_msg_;
    
  void sub_cb(const ArrayMsg::SharedPtr msg);
  void timer_cb();
};

#endif //YOUR_SOLUTION_SRC_SPIN_SLOW_UPDATE_H_
