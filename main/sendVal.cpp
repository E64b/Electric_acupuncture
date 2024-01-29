#include "main.h"

void sendVal() {
  if (data.send && data.error == false) {
    reg.update();
    data.send = false;
  }
}