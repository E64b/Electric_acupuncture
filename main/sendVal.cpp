#include "main.h"

void send_val() {
  if (data.send) {
    reg.update();
    data.send = false;
  }
}