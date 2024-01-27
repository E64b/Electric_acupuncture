#include "main.h"

void sendVal() {
  if (data.send) {
    reg.update();
    data.send = false;
  }
}