#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

Image::Image(const std::string &a_path)
{
  if((data = (Pixel*)stbi_load(a_path.c_str(), &width, &height, &channels, sizeof(Pixel))) != nullptr)
  {
    size = width * height * channels;
  }
}

Image::Image(int a_width, int a_height, int a_channels)
{
  data = new Pixel[a_width * a_height ]{};

  if(data != nullptr)
  {
    width = a_width;
    height = a_height;
    size = a_width * a_height * a_channels;
    channels = a_channels;
    self_allocated = true;
  }
}

Image::Image(const Image &other) {
  width = other.width;
  height = other.height;
  channels = other.channels;
  size = other.size;
  self_allocated = other.self_allocated;
  if (other.self_allocated) {
    data = new Pixel[width * height] {};
  } else {
    data = (Pixel *)malloc(size);
  }
  if (data != nullptr) {
    memcpy(data, other.data, size);
  }
}

Image::Image(Image &&other) {
  width = other.width;
  height = other.height;
  channels = other.channels;
  size = other.size;
  self_allocated = other.self_allocated;
  data = other.data;
  other.data = nullptr;
  other.size = 0;
  other.width = -1;
  other.height = -1;
  other.channels = 3;
  other.self_allocated = false;
}

Image &Image::operator= (const Image &other) {
  if (this == &other) {
    return *this;
  }
  this->~Image();
  width = other.width;
  height = other.height;
  channels = other.channels;
  size = other.size;
  self_allocated = other.self_allocated;
  if (other.self_allocated) {
    data = new Pixel[width * height] {};
  } else {
    data = (Pixel *)malloc(size);
  }
  if (data != nullptr) {
    memcpy(data, other.data, size);
  }
  return *this;
}

Image &Image::operator= (Image &&other) {
  if (this == &other) {
    return *this;
  }
  this->~Image();
  width = other.width;
  height = other.height;
  channels = other.channels;
  size = other.size;
  self_allocated = other.self_allocated;
  data = other.data;
  other.data = nullptr;
  other.size = 0;
  other.width = -1;
  other.height = -1;
  other.channels = 3;
  other.self_allocated = false;
  return *this;
}

void Image::GetSubImage(Image &screen, int x, int y) {
  y = this->height - y - screen.height;
  Pixel Black = {47, 47, 46, 255};
  int i = 0;
  for (; i + y < 0; i++) { //Заполнить черным пространство сверху (если нужно)
    for (int j = 0; j < screen.width; j++) {
      screen.data[i * screen.width + j] = Black;
    }
  }
  for (; i < std::min(screen.height, height - y); i++) {
    int j = 0;
    for (; j + x < 0; j++) { // Заполнить черным пространство слева
      screen.data[i * screen.width + j] = Black;
    }
    int copy_width = std::min(width, std::min(screen.width-j, width - x));
    memcpy(screen.data + i * screen.width + j, data + (y + i)*width + x + j, copy_width*sizeof(Pixel));
    for(j += copy_width; j < screen.width; j++) { //Заполнить черным пространство справа
      screen.data[i * screen.width + j] = Black;
    }
  }
  //Заполнить снизу.
  for (; i < screen.height; i++) {
    for (int j = 0; j < screen.width; j++) {
      screen.data[i * screen.width + j] = Black;
    }
  }

}

void Image::PutTile(Image &source, int x_src, int y_src, int x_dst, int y_dst, int scale, bool invert) {
  if (x_src < 0 || y_src < 0 || x_src + tileSize > source.Width() || y_src + tileSize > source.Height()) {
    std::cout << "Unable to put tile on image: incorrect source coords" << std::endl;
    return;
  }
  if (x_dst < 0 || y_dst < 0 || x_dst + tileSize*scale > width || y_dst + tileSize*scale > height) {
    std::cout << "Unable to put tile on image: incorrect destination coords" << std::endl;
    return;
  }
  if (scale <= 0) {
    std::cout << "Unable to put tile on image: Incoorect scale" << std::endl;
    return;
  }

  for (int j = 0; j < tileSize; j++) {
    for (int i = 0; i < tileSize; i++) {
      for (int k = 0; k < scale; k++) {
        for (int t = 0; t < scale; t++) {
          if (invert) {
            PutPixel(x_dst + i*scale + t, y_dst + j*scale + k, source.GetPixel(x_src + tileSize - i - 1, y_src + j));
          } else {
            PutPixel(x_dst + i*scale + t, y_dst + j*scale + k, source.GetPixel(x_src + i, y_src + j));
          }
        }
      }
    }
  }
}


int Image::Save(const std::string &a_path)
{
  auto extPos = a_path.find_last_of('.');
  if(a_path.substr(extPos, std::string::npos) == ".png" || a_path.substr(extPos, std::string::npos) == ".PNG")
  {
    stbi_write_png(a_path.c_str(), width, height, channels, data, width * channels);
  }
  else if(a_path.substr(extPos, std::string::npos) == ".jpg" || a_path.substr(extPos, std::string::npos) == ".JPG" ||
          a_path.substr(extPos, std::string::npos) == ".jpeg" || a_path.substr(extPos, std::string::npos) == ".JPEG")
  {
    stbi_write_jpg(a_path.c_str(), width, height, channels, data, 100);
  }
  else
  {
    std::cerr << "Unknown file extension: " << a_path.substr(extPos, std::string::npos) << "in file name" << a_path << "\n";
    return 1;
  }

  return 0;
}



Image::~Image()
{
  if (data == nullptr) {
    return;
  }
  if(self_allocated)
    delete [] data;
  else
  {
    stbi_image_free(data);
  }
}