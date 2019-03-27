#ifndef LOCATOR_HPP_
#define LOCATOR_HPP_

#include "gobo-core-config.h"

#include <gobo/platform/toolchain.h>

namespace go {

class Instance;

class InstanceLocator
{
public:
    Instance &GetInstance(void) const;

protected:
    InstanceLocator(Instance &aInstance)
    {
        GO_UNUSED_VARIABLE(aInstance);
    }
};

class OwnerLocator
{
public:
    template <typename OwnerType> OwnerType &GetOwner(void);

protected:
    OwnerLocator(void *aOwner)
    {
        GO_UNUSED_VARIABLE(aOwner);
    }
};

} // namespace go

#endif // LOCATOR_HPP_
