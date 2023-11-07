#include "main.h"

void send_val() {
  if (data.Send) {
    reg.update();
    data.Send = false;
  }
}