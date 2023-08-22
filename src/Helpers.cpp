#include "Helpers.hpp"

namespace Roki {
namespace Helpers {

DescriptorWrapper::DescriptorWrapper(int newFd) : Fd{newFd} {}

bool DescriptorWrapper::IsValid() const { return Fd >= 0; }
int DescriptorWrapper::Get() const { return Fd; }

DescriptorWrapper::DescriptorWrapper(DescriptorWrapper &&rhs) : Fd{rhs.Fd} {
  rhs.Fd = -1;
}

DescriptorWrapper &DescriptorWrapper::operator=(DescriptorWrapper &&rhs) {
  std::swap(Fd, rhs.Fd);
  return *this;
}

DescriptorWrapper::~DescriptorWrapper() {
  if (IsValid())
    close(Fd);
}
}
} // namespace Roki::Helpers