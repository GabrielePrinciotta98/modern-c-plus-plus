#include "image.h"

Image::Image(FileContent &&content, resolution_t res)
    : File{std::move(content)}, resolution{res} {}

// TODO provide file type
std::string_view Image::get_type() const
{
	return "IMG";
}

size_t Image::get_size() const
{
  return content.get_size();
}

size_t Image::get_raw_size() const {
  // TODO calculate raw size
  return resolution[0] * resolution[1] * 4; //4 bytes per pixel if colored
}

auto Image::get_resolution() const -> resolution_t { return this->resolution; }

// TODO content update function
void Image::update(FileContent&& new_content, resolution_t size)
{
  content = std::move(new_content);
  resolution = size;
}
