#ifndef YOUR_SOLUTION_SRC_SPIN_SOL_H_
#define YOUR_SOLUTION_SRC_SPIN_SOL_H_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
using ArrayMsg = std_msgs::msg::Float64MultiArray;

class SpinSolution : public rclcpp::Node {
 public:
  SpinSolution();
 private:
    
    //Subscription is in the ros client library for c++ namespace (rclcpp) which is where key components of ROS are
    //ArrayMsg is std_msgs::msg::Float64MultiArray which is just a multidimensional matrix of 64 bit floats, in this case it's just coordinates for the position
    //std_msgs::msg is the namespace for messages in ros2 which are just datatypes like string, float, bool, header, etc. for data sent between nodes
    //trailing _ in pos_sub_ is to tell you it's a member variable of SlowSolution
    //SharedPtr is a smart pointer that garbage collects itself without needing to call delete

    //define publisher and subscriber attributes for solution class
    rclcpp::Subscription<ArrayMsg>::SharedPtr pos_sub_;
    rclcpp::Subscription<ArrayMsg>::SharedPtr vel_sub_;
    rclcpp::Publisher<ArrayMsg>::SharedPtr pred_pub_;

    //define current position and current velocity attributes
    ArrayMsg current_vel_;
    ArrayMsg current_pos_;

    //define timer for publishing predicted position
    rclcpp::TimerBase::SharedPtr timer_;

    //tracks time since last measured pos and vel updates so that we can calculate delta_t
    rclcpp::Time last_msg_time_;

    //define timer callback which updates predicted position every 5 milliseconds
    void timer_callback();

    //The callback that will be called anytime pos_sub_ gets new data. It will update current_pos_ to measuredpos/
    //const just signals that this callback is read-only (won't alter msg)
    void pos_callback(const ArrayMsg::SharedPtr msg);

    //The callback that will be called anytime vel_sub_ gets new data. It will update current_vel_ to measuredvel/
    void vel_callback(const ArrayMsg::SharedPtr msg);
};

#endif //YOUR_SOLUTION_SRC_SPIN_SOL_H_
