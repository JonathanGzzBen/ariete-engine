#ifndef ENGINE_STATE_H
#define ENGINE_STATE_H
#include "graphic_context.h"
#include "script_reader.h"

struct EngineState {
  bool valid;
  GraphicContext graphic_context;
  ScriptReader script_reader;
};

auto engine_state_create(const char* script_reader_file) -> EngineState {
  auto script_reader = script_reader_create(script_reader_file);
  return {
      .valid = true,
      .script_reader = std::move(script_reader),
  };
}

#endif  // ENGINE_STATE_H