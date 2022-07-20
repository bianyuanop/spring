#ifndef BASE_COMPONENTS_H__
#define BASE_COMPONENTS_H__

#include "Sim/Ecs/EcsMain.h"

template<class T>
struct BasicComponentType {
    T value = 0;
};

template<>
struct BasicComponentType<entt::entity> {
    entt::entity value{entt::null};
};

#define ALIAS_COMPONENT(Component, T) struct Component : public BasicComponentType<T> {};

#endif