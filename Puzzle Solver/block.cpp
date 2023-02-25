#include "block.h"
#include <cmath>
#include <iostream>

int Block::height() const {
  return data[0].size();
}

int Block::width() const {
  return data.size();
}

void Block::render(PNG &im, int x) const {
  // for (int i=0; i < (signed) data.size(); i++) {
  //   for (int y=0; y < (signed) data[0].size(); y++) {
  //     *(im.getPixel(i + x, y)) = data[i][y];
  //   }
  // }
  // std::cout << to_string(width() + x) << endl;
  for (unsigned i=0; i < (signed)width(); i++) {
    for (unsigned y=0; y < (signed)height(); y++) {
      HSLAPixel *left = im.getPixel(i + x, y);

      *left = data[i][y];
    }
  }
}

void Block::build(PNG &im, int x, int width) {
  for (int i=x; i < x+width; i++) {
    vector<HSLAPixel> tempVector;
    for (int j=0; j < (signed) im.height(); j++) {
      tempVector.push_back(*im.getPixel(i, j));
    }
    data.push_back(tempVector);
  }
}
