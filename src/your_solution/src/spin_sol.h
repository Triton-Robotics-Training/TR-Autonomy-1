#ifndef YOUR_SOLUTION_SRC_SPIN_SOL_H_
#define YOUR_SOLUTION_SRC_SPIN_SOL_H_

#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
using ArrayMsg = std_msgs::msg::Float64MultiArray;

// Message type used by all three topics
using ArrayMsg = std_msgs::msg::Float64MultiArray;

class SpinSolution : public rclcpp::Node {
public:
  SpinSolution();

private:
  // your code here
  // Subscriber callbacks
  void position_callback(const ArrayMsg::SharedPtr msg);
  void velocity_callback(const ArrayMsg::SharedPtr msg);
  void prediction_callback();

  // Publisher
  rclcpp::Publisher<ArrayMsg>::SharedPtr predicted_pos_pub_;

  // Subscribers
  rclcpp::Subscription<ArrayMsg>::SharedPtr measured_pos_sub_;
  rclcpp::Subscription<ArrayMsg>::SharedPtr measured_vel_sub_;

  // Wall timer
  rclcpp::TimerBase::SharedPtr timer_;

  // Latest measured position and velcoity
  double x = 0.0, y = 0.0, vx = 0.0, vy = 0.0;

  // Time when the latest position measurement arrived
  rclcpp::Time last_position_time_;

  // Flags to ensure valid measurements are received
  bool has_position_ = false, has_velocity_ = false;
};

#endif // YOUR_SOLUTION_SRC_SPIN_SOL_H_