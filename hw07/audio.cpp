#include "audio.h"

Audio::Audio(FileContent &&content, unsigned duration)
    : File{std::move(content)}, duration{duration} {}

// TODO provide file type
std::string_view Audio::get_type() const
{
	return "AUD";
}

size_t Audio::get_size() const
{
  return content.get_size();
}

size_t Audio::get_raw_size() const {
  // TODO calculate raw size
  return duration * 2 * 2 * 48000; //2 bytes * 2 channels * frequency
}

unsigned Audio::get_duration() { return this->duration; }

// TODO content update function
void Audio::update(FileContent&& new_content, unsigned new_duration)
{
  content = std::move(new_content);
  duration = new_duration;
}