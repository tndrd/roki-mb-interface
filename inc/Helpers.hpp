#pragma once

#include <iostream>
#include <unistd.h>

namespace Roki::Helpers {

struct DescriptorWrapper final {
private:
  int Fd = -1;

public:
  explicit DescriptorWrapper(int newFd) : Fd{newFd} {}
  bool IsValid() const { return Fd >= 0; }

  int Get() const { return Fd; }

  DescriptorWrapper(const DescriptorWrapper &) = delete;
  DescriptorWrapper &operator=(const DescriptorWrapper &) = delete;

  DescriptorWrapper(DescriptorWrapper &&rhs) : Fd{rhs.Fd} { rhs.Fd = -1; }
  DescriptorWrapper &operator=(DescriptorWrapper &&rhs) {
    std::swap(Fd, rhs.Fd);
    return *this;
  }

  ~DescriptorWrapper() {
    if (IsValid())
      close(Fd);
  }
};

} // namespace Roki::Helpers