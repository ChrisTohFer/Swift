#pragma once

#include "entity.h"
#include "GlobalHeaders/template_helpers.h"

#include <map>
#include <tuple>

namespace SWIFT::EC
{

    //Entity map is a map for a single entity type
    template<typename ...COMPONENT_TYPES>
    class ENTITY_MAP;

    template<typename ...COMPONENT_TYPES>
    class ENTITY_MAP<ENTITY<COMPONENT_TYPES...>>
    {
        std::map<ENTITY_ID, ENTITY<COMPONENT_TYPES...>> m_entities;

    public:
        ENTITY<COMPONENT_TYPES...>& insert(ENTITY<COMPONENT_TYPES...>&& entity)
        {
            return m_entities.emplace(entity.id(), std::move(entity)).first->second;
        }
        void remove(ENTITY_ID id)
        {
            m_entities.erase(id);
        }

        auto begin() { return m_entities.begin(); }
        auto end() { return m_entities.end(); }
        auto size() { return m_entities.size(); }
    };

    //Entity holder is the container for all entities within a scene, it contains maps for a chosen set of entity types
    template<typename ...ENTITY_TYPES>
    class ENTITY_HOLDER
    {
    public:
        //Queries
        template<typename ENTITY_TYPE>
        static constexpr bool has_entity()
        {
            return (std::is_same<ENTITY_TYPE, ENTITY_TYPES>::value || ...);
        }

        //Retrieval functions
        template<typename ENTITY_TYPE>
        ENTITY_MAP<ENTITY_TYPE>& entity_map()
        {
            auto constexpr index = VARIADIC_INDEX<ENTITY_TYPE, ENTITY_TYPES...>::index;
            return std::get<index>(m_maps);
        }

        //Manipulators
        template<typename ENTITY_TYPE>
        ENTITY_TYPE& insert(ENTITY_TYPE&& entity)
        {
            return entity_map<ENTITY_TYPE>().insert(std::move(entity));
        }
        template<typename ENTITY_TYPE>
        void remove(ENTITY_ID id)
        {
            return entity_map<ENTITY_TYPE>().remove(id);
        }

    private:
        std::tuple<ENTITY_MAP<ENTITY_TYPES>...> m_maps;
    };
    
}