#include "filecontent.h"


// TODO constructors
FileContent::FileContent(const std::string& content)
    : content{std::make_shared<const std::string>(content)}
    {}

FileContent::FileContent(std::string&& content)
    : content{std::make_shared<const std::string>(std::move(content))}
    {}

FileContent::FileContent(const char* content)
    : content{std::make_shared<const std::string>(content)}
    {}   

// TODO member functions
size_t FileContent::get_size() const
{
    return content->size();
}

std::shared_ptr<const std::string> FileContent::get() const
{
    return content;
}
