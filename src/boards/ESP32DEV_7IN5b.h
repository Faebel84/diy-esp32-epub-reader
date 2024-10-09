#pragma once

#include "Board.h"

class ESP32DEV_7IN5b : public Board
{
public:
  virtual void power_up();
  virtual void prepare_to_sleep();
  virtual Renderer *get_renderer();
  virtual void stop_filesystem();
  virtual ButtonControls *get_button_controls(xQueueHandle ui_queue);
};