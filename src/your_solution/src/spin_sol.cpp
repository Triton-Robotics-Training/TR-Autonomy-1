#include "spin_sol.h"

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SpinSolution>());
  rclcpp::shutdown();
  return 0;
}

//Added 
SpinSolution::SpinSolution() : Node("spinsolution") {
  
  //create publisher for predicted position with queue depth of 10 (stores last 10 messages)
  //this-> is just to invoke the create_publisher function of the Node class which SlowSolution inherits
  pred_pub_ = this->create_publisher<ArrayMsg>("/predictedpos",10);

  //create subscriber to measured position with queue depth of 10 (stores last 10 messages)
  //When a message arrives on /measuredpos, call pos_callback and pass it the message
  pos_sub_ = this->create_subscription<ArrayMsg>("/measuredpos", 10, std::bind(&SpinSolution::pos_callback, this, std::placeholders::_1)); 

  //create subscriber to measured velocity with queue depth of 10 (stores last 10 messages)
  //When a message arrives on /measuredvel, call vel_callback and pass it the message
  vel_sub_ = this->create_subscription<ArrayMsg>("/measuredvel", 10, std::bind(&SpinSolution::vel_callback, this, std::placeholders::_1)); 
  
  //create timer to trigger timer callback every 5 miliseconds in wall time (real-world time as opposed to CPU time which only counts when CPU is working)
  timer_ = this->create_wall_timer(
    std::chrono::milliseconds(1),
    std::bind(&SpinSolution::timer_callback, this)
  );

  //Set initial last message time
  last_msg_time_ = this->now();

}

//SpinSolution position callback definition
// everytime the pos_sub_ recieves a new /measuredpos
void SpinSolution::pos_callback(const ArrayMsg::SharedPtr msg) {
  current_pos_ = *msg;

  //update last message time. Position and velocity are posted at roughly the same time so I'm only calling it in pos_callback
  last_msg_time_ = this->now();

  //RCLCPP_INFO calls the ros logger at the info level (rather than warn, error, debug, or fatal)
  //this->get_logger() gets this specific node's logger so we can see where the info comes from
  RCLCPP_INFO(this->get_logger(), "new pos: %f, %f", current_pos_.data[0], current_pos_.data[1]);
}

//SpinSolution velocity callback definition
// everytime the vel_sub_ recieves a new /measuredvel
void SpinSolution::vel_callback(const ArrayMsg::SharedPtr msg) {
  current_vel_ = *msg;
  RCLCPP_INFO(this->get_logger(), "new vel: %f", current_vel_.data[0]);
}

//Calculates and publishes predicted position, called by timer every millisecond
void SpinSolution::timer_callback() {

  //guard against timer callbacks where no measured pos or vel data has been published yet
  if (current_pos_.data.size() < 2 || current_vel_.data.size() < 2) {
    return;
  }

  //subtracting two Time objects gives a Duration object. .seconds() converts duration to float
  double delta_t = (this->now() - last_msg_time_).seconds();

  ArrayMsg pred_pos;
  pred_pos.data = {
    current_pos_.data[0] + (delta_t * current_vel_.data[0]),  //x
    current_pos_.data[1] + (delta_t * current_vel_.data[1])   //y
  };
  pred_pub_->publish(pred_pos);
}