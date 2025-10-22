#ifndef SCRIPT_READER_H
#define SCRIPT_READER_H
#include <fstream>
#include <print>
#include <string>

struct ScriptReader {
  bool valid;
  std::string current_line;
  std::ifstream input_stream;
};

auto script_reader_create(const char* file_path) -> ScriptReader;

auto script_reader_get_next_line(ScriptReader* script_reader) -> std::string;

auto script_reader_get_current_line(ScriptReader* script_reader) -> std::string;

auto script_reader_eof(const ScriptReader& script_reader) -> bool;

auto script_reader_reset(ScriptReader* script_reader) -> void;

#endif  // SCRIPT_READER_H
