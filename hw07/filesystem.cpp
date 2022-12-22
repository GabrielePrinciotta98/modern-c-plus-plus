#include "filesystem.h"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <sstream>

Filesystem::Filesystem() {}

bool Filesystem::register_file(const std::string &name,
                               const std::shared_ptr<File> &file) {
  // TODO: Something needs to be done here 

  //std::shared_ptr<Filesystem> self = std::make_shared<Filesystem>();
  std::shared_ptr<Filesystem> thisptr = this->shared_from_this();

  // TODO: You need some checks here
  if (file == nullptr)
    return false;

  if (file->filesystem.lock() != nullptr || name.empty()){ //check if another filesystem has the file
    return false;
  }

  auto find_name = std::find_if(files.begin(), files.end(), 
    [&name](const auto& f) { return f->get_name() == name; });

  if(find_name != files.end()){  //check if a file with the same name exists
    return false;
  }

  file->filesystem = std::move(thisptr);

  // TODO: More updates you need to do!
  file->rename(name);
  // TODO register a new file here
  files.emplace_back(file);
  return true;
}

bool Filesystem::remove_file(std::string_view name) {
  // TODO file removal
  auto find_name = std::find_if(files.begin(), files.end(), 
    [&name](const auto& f) { return f->get_name() == name; }); 
  if (find_name != files.end()){
    (*find_name)->filesystem.lock() = nullptr;
    files.erase(find_name);
    return true;
  }
  return false;
}

bool Filesystem::rename_file(std::string_view source, std::string_view dest) {
  // TODO file renaming
  auto find_source = std::find_if(files.begin(), files.end(), 
    [&source](const auto& f) { return f->get_name() == source; }); 
  if (find_source == files.end()){
    return false;
  }
  auto find_dest = std::find_if(files.begin(), files.end(), 
    [&dest](const auto& f) { return f->get_name() == dest; }); 
  if (find_dest != files.end()){
    return false;
  }

  return (*find_source)->rename(dest);
}

std::shared_ptr<File> Filesystem::get_file(std::string_view name) const {
  // TODO

  auto find_name = std::find_if(files.begin(), files.end(), 
    [&name](const auto& f) { return f->get_name() == name; }); 
  if (find_name != files.end()){
    return *find_name;
  }
  return nullptr;
}

size_t Filesystem::get_file_count() const {
  // TODO
  return files.size();
}

size_t Filesystem::in_use() const {
  // TODO sum up all real file sizes
  size_t size = 0;
  for (size_t i = 0; i < files.size(); i++)
  {
    size += files[i]->get_size();
  }
  //size_t size = std::accumulate(files.begin(), files.end(), 0, 
  //  [](std::shared_ptr<File> a, std::shared_ptr<File> b){ return a->get_size() + b->get_size(); });
  return size;
}

// convenience function so you can see what files are stored
std::string Filesystem::file_overview(bool sort_by_size) {
  std::ostringstream output;
  // this function is not tested, but it may help you when debugging.

  output << "files in filesystem: " << std::endl;

  // for (auto&& entry : this->files) {
  //     // TODO: fix printing name, type and size
  //     output << entry.get_name << std::endl;
  // }
  return std::move(output).str();
}

std::vector<std::shared_ptr<File>>
Filesystem::files_in_size_range(size_t max, size_t min) const {
  // TODO: find the matching files and return them
  std::vector<std::shared_ptr<File>> files_in_range;
  for (size_t i=0; i<files.size(); i++)
  {
    size_t current_size = files[i]->get_size();
    if (current_size >= min && current_size <= max){
      files_in_range.emplace_back(files[i]);
    }
  }
  return files_in_range;
}
