#include "headers.h"
File::File(string file_path, string file_name, string SHA, int bytes)
{
    this->file_path = file_path;
    this->SHA = SHA;
    this->file_name = file_name;
    this->size = size;
    this->available_chunk = {};
}