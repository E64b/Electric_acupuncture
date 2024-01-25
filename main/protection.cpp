#include "main.h"

void protection() {
  if (data.amperage_uA > data.maxAmperage) {
    data.error = true;
  }
}