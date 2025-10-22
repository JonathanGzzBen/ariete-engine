#include "ariete-engine/script_reader.h"

auto script_reader_create(const char* file_path) -> ScriptReader {
  std::ifstream input_stream(file_path);
  if (!input_stream.good() || !input_stream.is_open()) {
    std::println(stderr, "Could not open file '{}'", file_path);
    return {.valid = false};
  }
  return {.valid = true, .input_stream = std::move(input_stream)};
}

auto script_reader_get_next_line(ScriptReader* script_reader) -> std::string {
  std::string line;
  std::getline(script_reader->input_stream, line);
  script_reader->current_line = line;
  return line;
}

auto script_reader_get_current_line(ScriptReader* script_reader)
    -> std::string {
  return script_reader->current_line;
}

auto script_reader_eof(const ScriptReader& script_reader) -> bool {
  return script_reader.input_stream.eof();
}

auto script_reader_reset(ScriptReader* script_reader) -> void {
  script_reader->input_stream.seekg(0, std::ios::beg);
}