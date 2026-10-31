#ifndef YOUR_SOLUTION_SRC_SPIN_SLOW_UPDATE_H_
#define YOUR_SOLUTION_SRC_SPIN_SLOW_UPDATE_H_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
using ArrayMsg = std_msgs::msg::Float64MultiArray;

class SlowSolution : public rclcpp::Node {
 public:
  SlowSolution();
 private:

  //Subscription is in the ros client library for c++ namespace (rclcpp) which is where key components of ROS are
  //ArrayMsg is std_msgs::msg::Float64MultiArray which is just a multidimensional matrix of 64 bit floats, in this case it's just coordinates for the position
  //std_msgs::msg is the namespace for messages in ros2 which are just datatypes like string, float, bool, header, etc. for data sent between nodes
  //trailing _ in pos_sub_ is to tell you it's a member variable of SlowSolution
  //SharedPtr is a smart pointer that garbage collects itself without needing to call delete

  //define publisher and subscriber attributes for slow solution class
  rclcpp::Subscription<ArrayMsg>::SharedPtr pos_sub_;
  rclcpp::Publisher<ArrayMsg>::SharedPtr pred_pub_;


  //The callback that will be called anytime pos_sub_ gets new data. It will just publish back the message it got (returning predictedpos as measuredpos)
  //const just signals that this callback is read-only (won't alter msg)
  void pos_callback(const std_msgs::msg::Float64MultiArray::SharedPtr msg);


};

#endif //YOUR_SOLUTION_SRC_SPIN_SLOW_UPDATE_H_
