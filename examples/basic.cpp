#include "toon.hpp"
#include <iostream>

int main() {

    auto cfg = toon::parse(R"(

window:
  width: 1280
  height: 720

user:
  name: zepyx
  admin: true

)");

    cfg["window"]["width"] = 190;

    toon::write_file("config.toon", cfg);

    std::cout << toon::dump(cfg);
}