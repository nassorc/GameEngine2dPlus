#pragma once

#include <tuple>
#include "Components.h"
#include "Index.h"

class EntityManager;

template <class T, class Tuple>
struct Index;

template <class T, class... Types>
struct Index<T, std::tuple<T, Types...>> {
    static const std::size_t value = 0;
};

template <class T, class U, class... Types>
struct Index<T, std::tuple<U, Types...>> {
    static const std::size_t value = 1 + Index<T, std::tuple<Types...>>::value;
};

typedef std::tuple<
	CTransform,
	CLifespan,
    CHealth,
	CInput,
	CBoundingBox,
	CAnimation,
	CGravity,
	CState,
	CDraggable,
    CDamage,
    CInvincibility,
    CPatrol,
    CFollowPlayer,
    CText,
    CShape,
    CSprite,
    CParallax,
    CDialogue,
    CFrameCollision,
    CMovable,
    CHitbox,
    CRect,
    CCollide
> ComponentTuple;

// how to get a specfic component without an identifier:
// std::get<CInput>(myTuple).has == true

class Entity {
	friend class EntityManager;

	bool             m_active = true;
	size_t           m_id     = 0;
	std::string      m_tag    = "default";
	ComponentTuple   m_components;

	// constructor is private so we can never create entities outside the entityManager which had friend access
	Entity(const size_t id, const std::string& tag) : m_id(id), m_tag(tag) {}
public:
	template <class T, class... TArgs>
	inline T& addComponent(TArgs&&... mArgs) {
		auto& component = getComponent<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.has = true;
		return component;
	}
	template<class T>
	inline T& getComponent() {
		return std::get<Index<T, ComponentTuple>::value>(m_components);
	}

	template<class T>
	inline const T& getComponent() const {
		return std::get<Index<T, ComponentTuple>::value>(m_components);
	}

	template<class T>
	inline bool hasComponent() const {
		return getComponent<T>().has;
	}

    template<class T, class... TArgs>
    inline Entity& add(TArgs&&... mArgs) {
        auto& component = getComponent<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return *this;
    }

//	template<class T>
//	void removeComponent() {
//		getComponent<T>() = T();
//	}

	// private member access functions
	bool isActive() const {
		return m_active;
	}
	bool isDed() {
		return static_cast<bool>(!(m_active));
	}
	void setNotActive() {
		m_active = false;
	}
	const std::string& tag() const {
		return m_tag;
	}
	const size_t id() const {
		return m_id;
	}
	void destroy() {
    m_active = false;
	}
};