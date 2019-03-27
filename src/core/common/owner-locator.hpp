#ifndef OWNER_LOCATOR_HPP_
#define OWNER_LOCATOR_HPP_

#include "gobo-core-config.h"

#include "common/instance.hpp"
#include "common/locator.hpp"

namespace go {

template <typename OwnerType> OwnerType &OwnerLocator::GetOwner(void)
{
    return Instance::Get().Get<OwnerType>();
}

} // namespace go

#endif // OWNER_LOCATOR_HPP_
