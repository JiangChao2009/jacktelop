#include "jacktelop/keywindow.hpp"

KeyInputWindow::~KeyInputWindow() {}

KeyInputWindow::KeyInputWindow(QWidget *parent) {
    this->label = new QLabel(this);
    this->layout = new QHBoxLayout();
    label->setText("hello world!");
    layout->addWidget(label);
    setLayout(layout);
    this->lcm = new lcm::LCM("udpm://239.255.76.67:7667?ttl=1");
}

void KeyInputWindow::keyReleaseEvent(QKeyEvent *e) {
        if (!lcm->good()) {
      return;
    }
    
    double forward  = .2;
    double backward = -.2;
    double left     = .5;
    double right    = -.5;
    jacktelop::Twist_t twist;

    twist.linear.x = 0.0;
    twist.linear.y = 0.0;
    twist.linear.z = 0.0;
    twist.angular.x = 0.0;
    twist.angular.y = 0.0;
    twist.angular.z = 0.0;

    switch (e->key()) {
    case Qt::Key_Up:
      this->label->setText("Up");
      twist.linear.x = forward;
      break;
    case Qt::Key_Down:
      this->label->setText("Down");
      twist.linear.x = backward;
      break;
    case Qt::Key_Left:
      this->label->setText("Left");
      twist.angular.z = left;
      break;
    case Qt::Key_Right:
      this->label->setText("Right");
      twist.angular.z  = right;
      break;
    default:
      this->label->setText("Wrong Key");
      break;
    }
    lcm->publish("/cmd_vel", &twist);
}
