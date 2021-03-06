#pragma once
#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif
#include <atomic>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <errno.h>
#include <SDL2/SDL.h>
#include <vector>
#include <cstdint>
#include <thread>

#include "textbuffer.h"

struct Context;

class Console {
 private:
  void ReaderWorker();
  void ProcessOutput(uint8_t *data, size_t size);

  std::atomic<bool> end_threads_{false}; //
  SDL_Surface *surface_ = nullptr;
  TextBuffer buffer_;
  pid_t pid_ = -1;
  int master_ = -1;
  std::unique_ptr<std::thread> read_th_;

  uint32_t char_width_ = 16;
  uint32_t char_height_ = 16;

  uint32_t lines_ = 1;  // Actual console size.
  uint32_t scrollback_lines_ = 1000;

  Context* ctx_ = nullptr;  // Console is not the owner of this object.

 public:
  Console(Context* ctx) : ctx_(ctx) {}

  pid_t GetPid() const;
  void ResetPid();

  void HandleSurfaceChange(SDL_Surface *surface);
  void ResizeTextBuffer(uint32_t w, uint32_t h);  // Pixels.

  bool SpawnChild();
  void CloseMaster();

  void UpdateSurface();
};


