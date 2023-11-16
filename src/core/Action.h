#pragma once

#include <string>
#include "SFML/Graphics.hpp"
#include "../math/Vec2.h"

class Action {
public:
	std::string m_name = "NONE";
	std::string m_type = "NONE";
	Vec2 m_pos = Vec2(0, 0);

	Action() {

	}
	Action(const std::string& name, const std::string& type)
		: m_name(name)
		, m_type(type)
	{
	}
	Action(const std::string& name, const std::string& type, const Vec2& pos)
		: m_name(name)
		, m_type(type)
		, m_pos(pos)
	{
	}
	const std::string& name() const {
		return m_name;
	}
	const std::string& type() const {
		return m_type;
	}
	const Vec2 pos() const {
		return m_pos;
	}
	std::string toString() const {
		return m_type + " " + m_name;

	}
};