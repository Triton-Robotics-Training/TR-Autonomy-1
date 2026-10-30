#ifndef YOUR_SOLUTION_SRC_SPIN_SOL_H_
#define YOUR_SOLUTION_SRC_SPIN_SOL_H_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
using ArrayMsg = std_msgs::msg::Float64MultiArray;

class SpinSolution : public rclcpp::Node {
 public:
  SpinSolution();
 private:
  void pos_callback(const ArrayMsg::SharedPtr msg);
  void vel_callback(const ArrayMsg::SharedPtr msg);
  void timer_callback();

  rclcpp::Subscription<ArrayMsg>::SharedPtr pos_sub_;
  rclcpp::Subscription<ArrayMsg>::SharedPtr vel_sub_;
  rclcpp::Publisher<ArrayMsg>::SharedPtr pred_pub_;
  rclcpp::TimerBase::SharedPtr timer_;

  // latest measurement and when it arrived
  double x_ = 0.0, y_ = 0.0;
  double vx_ = 0.0, vy_ = 0.0;
  rclcpp::Time last_time_;
  bool have_measurement_ = false;
};

#endif //YOUR_SOLUTION_SRC_SPIN_SOL_H_
