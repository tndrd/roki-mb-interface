#pragma once

#include <iostream>
#include <unistd.h>

namespace MbInterface {
namespace Helpers {

struct DescriptorWrapper final {
private:
  int Fd = -1;

public:
  explicit DescriptorWrapper(int newFd = -1);
  bool IsValid() const;
  int Get() const;

  DescriptorWrapper(const DescriptorWrapper &) = delete;
  DescriptorWrapper &operator=(const DescriptorWrapper &) = delete;

  DescriptorWrapper(DescriptorWrapper &&rhs);
  DescriptorWrapper &operator=(DescriptorWrapper &&rhs);

  ~DescriptorWrapper();
};
} // namespace Helpers
} // namespace MbInterface