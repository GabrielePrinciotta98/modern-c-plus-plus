#include "document.h"

Document::Document(FileContent &&content) : File{std::move(content)} {}

// TODO provide file type
std::string_view Document::get_type() const
{
	return "DOC";
}

size_t Document::get_size() const
{
  return content.get_size();
}

size_t Document::get_raw_size() const {
  // TODO get the document size
  return content.get_size();
}

unsigned Document::get_character_count() const {
  // TODO count non whitespace characters
	unsigned whitespaces = std::count_if(content.get()->begin(), content.get()->end(), 
    [](char c){ return c == ' ' || c == '\n' || c == '\t'; });
  return content.get_size() - whitespaces;
}

// TODO content update function
void Document::update(FileContent&& new_content)
{
	content = std::move(new_content);
}
