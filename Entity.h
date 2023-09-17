#pragma once

#include "Components.h"
#include <tuple>

class EntityManager;

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
    CShape
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
		return std::get<T>(m_components);
	}

	template<class T>
	inline const T& getComponent() const {
		return std::get<T>(m_components);
	}

	template<class T>
	inline bool hasComponent() const {
		return getComponent<T>().has;
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