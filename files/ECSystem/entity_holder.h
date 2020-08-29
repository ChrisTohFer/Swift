#pragma once

#include "entity.h"
#include "GlobalHeaders/template_helpers.h"

#include <map>
#include <set>
#include <tuple>

namespace SWIFT::EC
{
    //Entity map is a map for a single entity type
    template<typename ...COMPONENT_TYPES>
    class ENTITY_MAP;

    template<typename ...COMPONENT_TYPES>
    class ENTITY_MAP<ENTITY<COMPONENT_TYPES...>>
    {
        using MAP = std::map<ENTITY_ID, ENTITY<COMPONENT_TYPES...>>;
        MAP m_entities;

    public:
        ENTITY<COMPONENT_TYPES...>& insert(ENTITY<COMPONENT_TYPES...>&& entity)
        {
            return m_entities.emplace(entity.id(), std::move(entity)).first->second;
        }
        void insert(typename MAP::iterator begin, typename MAP::iterator end)
        {
            m_entities.insert(begin, end);
        }
        void insert(ENTITY_MAP& other)
        {
            m_entities.insert(other.begin(), other.end());
        }
        void remove(ENTITY_ID id)
        {
            m_entities.erase(id);
        }
        void remove(std::set<ENTITY_ID> id_set)
        {
            for(auto id : id_set)
                m_entities.erase(id);
        }

        auto begin() { return m_entities.begin(); }
        auto end() { return m_entities.end(); }
        auto size() const { return m_entities.size(); }
        auto clear() { m_entities.clear(); }
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
        template<typename ENTITY_TYPE>
        size_t count()
        {
            return entity_map<ENTITY_TYPE>().size();
        }
        template<>
        size_t count<void>()
        {
            return (entity_map<ENTITY_TYPES>().size() + ...);   //Total entity count
        }

        //Retrieval functions
        template<typename ENTITY_TYPE>
        ENTITY_MAP<ENTITY_TYPE>& entity_map()
        {
            static_assert((std::is_same<ENTITY_TYPES, ENTITY_TYPE>::value || ...)); //Will fail if we don't contain the map that is asked for

            auto constexpr index = VARIADIC_INDEX<ENTITY_TYPE, ENTITY_TYPES...>::index;
            return std::get<index>(m_entities);
        }
        template<typename ENTITY_TYPE>
        ENTITY_MAP<ENTITY_TYPE>& instantiated_entity_map()
        {
            static_assert((std::is_same<ENTITY_TYPES, ENTITY_TYPE>::value || ...)); //Will fail if we don't contain the map that is asked for

            auto constexpr index = VARIADIC_INDEX<ENTITY_TYPE, ENTITY_TYPES...>::index;
            return std::get<index>(m_instantiated);
        }
        
        //Manipulators
        void update()
        {
            //Insert all instantiated entities into the main maps then clear the instantiated maps
            (entity_map<ENTITY_TYPES>().insert(instantiated_entity_map<ENTITY_TYPES>()), ...);
            (instantiated_entity_map<ENTITY_TYPES>().clear(), ...);

            //Remove all destroyed entities and clear the set
            (entity_map<ENTITY_TYPES>().remove(m_destroyed), ...);  //Slightly inefficient - all maps must be checked for each ID
            m_destroyed.clear();
        }
        template<typename ENTITY_TYPE>
        ENTITY_TYPE& instantiate(ENTITY_TYPE&& entity)
        {
            return instantiated_entity_map<ENTITY_TYPE>().insert(std::move(entity));
        }
        void destroy(ENTITY_ID id)
        {
            m_destroyed.emplace(id);
        }

        //Direct addition and removal of entities
        template<typename ENTITY_TYPE>
        ENTITY_TYPE& insert(ENTITY_TYPE&& entity)
        {
            return entity_map<ENTITY_TYPE>().insert(std::move(entity));
        }
        template<typename ENTITY_TYPE>
        void remove(ENTITY_ID id)
        {
            entity_map<ENTITY_TYPE>().remove(id);
        }

    private:
        std::tuple<ENTITY_MAP<ENTITY_TYPES>...> m_entities;
        std::tuple<ENTITY_MAP<ENTITY_TYPES>...> m_instantiated;
        std::set<ENTITY_ID>                     m_destroyed;
    };
    
}