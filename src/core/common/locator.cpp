#include "locator.hpp"

#include "common/instance.hpp"

namespace go {

Instance &InstanceLocator::GetInstance(void) const
{
    return Instance::Get();
}

} // namespace go
