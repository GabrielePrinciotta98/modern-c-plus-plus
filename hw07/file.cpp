#include "file.h"

#include "filesystem.h"

size_t File::get_size() const { return this->content.get_size(); }

bool File::rename(std::string_view new_name) {
	// TODO: file renaming
	if (filesystem.lock() == nullptr || name == new_name) 
		return false;
	
	name = new_name;
	return true;
}

const std::string &File::get_name() const { return this->name; }

const FileContent &File::get_content() const { return this->content; }

// TODO file constructor
File::File(FileContent&& content, std::string_view name)
	: content{std::move(content)}, name{name}
	{

	}
