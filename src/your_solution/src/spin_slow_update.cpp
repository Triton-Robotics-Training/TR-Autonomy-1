#include "spin_slow_update.h"

const float MEASURED_PUBLISH_PERIOD = 0.0001;

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SlowSolution>());
  rclcpp::shutdown();
  return 0;
}

//SlowSolution Constructor definition
SlowSolution::SlowSolution() : Node("slowsolution") {
  
  //create publisher for predicted position with queue depth of 10 (stores last 10 messages)
  //this-> is just to invoke the create_publisher function of the Node class which SlowSolution inherits
  pred_pub_ = this->create_publisher<ArrayMsg>("/predictedpos",10);

  //create subscriber to measured position with queue depth of 10 (stores last 10 messages)
  //When a message arrives on /measuredpos, call pos_callback and pass it the message
  pos_sub_ = this->create_subscription<ArrayMsg>("/measuredpos", 10, std::bind(&SlowSolution::pos_callback, this, std::placeholders::_1)); 

}

//SlowSolution position callback definition
//Publishes /predictedpos everytime the pos_sub_ recieves a new /measuredpos
void SlowSolution::pos_callback(const ArrayMsg::SharedPtr msg) {
  pred_pub_->publish(*msg);
}