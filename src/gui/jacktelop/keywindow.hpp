#include <QtGui/QApplication>
#include <QtGui/QKeyEvent>
#include <QDebug>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QString>
#include <QtGui/QWidget>
#include "lcm/lcm-cpp.hpp"
#include "jacktelop/Vector3_t.hpp"
#include "jacktelop/Twist_t.hpp"

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#endif

class KeyInputWindow : public QWidget {
  Q_OBJECT

public:
  QLabel *label;
  lcm::LCM *lcm;
  QHBoxLayout *layout;
  KeyInputWindow(QWidget *parent = 0); 
  virtual ~KeyInputWindow();

  void keyReleaseEvent(QKeyEvent *e);
};
