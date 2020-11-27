#pragma once
#include "staff.h"
#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>
#include <iostream>
#include <vector>
#include <unordered_map>

/// <summary>
/// Auxiliary class for constructing T instance with a parameter list P 
/// </summary>
class ctor
{
public:
	template <typename Concrete, typename... P>
	static std::enable_if_t<std::is_constructible<Concrete, P...>::value, std::unique_ptr<Concrete>> constructArgs(P&&... params)
	{
		std::cout << __func__ << " templated..." << std::endl;
		return std::make_unique<Concrete>(std::forward<P>(params)...);
	}

	template <typename Concrete, typename... P>
	static std::unique_ptr<Concrete> constructArgs(...)
	{
		std::cout << __func__ << "(...)" << std::endl;
		return nullptr;
	}

	template <typename Concrete, typename... P>
	static std::unique_ptr<Concrete> constructNoArgs(void)
	{
		std::cout << __func__ << std::endl;
		return std::unique_ptr<Concrete>(new Concrete);
	}

	template <typename Concrete, typename... P>
	static std::unique_ptr<Concrete> constructNoArgs(P&&...)
	{
		std::cout << __func__ << std::endl;
		return nullptr;
	}

	template <typename T, typename... P>
	static std::unique_ptr<IStaffMember> create(P&&... params)
	{
		std::cout << __func__ << std::endl;
		return constructArgs<T, P...>(std::forward<P>(params)...);
	}
};

/// <summary>
/// Represents a structure describing rates
/// </summary>
typedef struct rate_t
{
	rate_t(const double y_rate, const double s_rate, const double m_rate) : year_rate(y_rate), sub_rate(s_rate), max_rate(m_rate) {}
	double year_rate;
	double sub_rate;
	double max_rate;
} rate_t;

/// <summary>
/// In-memory storage
/// </summary>
class Storage
{
public:
	Storage(const Storage&) = delete;
	Storage& operator = (const Storage&) = delete;
	virtual ~Storage() { storage_.clear(); }

	static Storage* get() {
		static Storage instance;
		return &instance;
	}

	/// <summary>
	/// Creates T instance based on IStaffMember interface
	/// </summary>
	/// <typeparam name="T">Input type</typeparam>
	/// <param name="emp">Structure with all necessary data</param>
	/// <returns></returns>
	template<typename T>
	bool create(item_t& emp)
	{
		const auto it = storage_.find(emp.id);

		if (it == storage_.end())
		{
			//emp.id = id_;
			storage_[emp.id] = ctor::create<T>(emp);
			//id_++;
			return true;
		}

		return false;
	}
	/// <summary>
	/// Gets the employee
	/// </summary>
	/// <param name="id">Employee ID</param>
	/// <returns></returns>
	std::shared_ptr<IStaffMember> getEmployee(const unsigned int id);

	/// <summary>
	/// Gets all the employees
	/// </summary>
	/// <param name="records"></param>
	/// <returns></returns>
	bool getEmployees(std::vector<std::shared_ptr<IStaffMember>>& records);

	/// <summary>
	/// Gets a rate
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	std::shared_ptr<rate_t> getRate(emp_type type);
private:
	Storage()
	{
		rates_.insert_or_assign(emp_type::employee, std::make_unique<rate_t>(0.03, 0, 0.3));
		rates_.insert_or_assign(emp_type::manager, std::make_unique<rate_t>(0.05, 0.005, 0.4));
		rates_.insert_or_assign(emp_type::sales, std::make_unique<rate_t>(0.01, 0.003, 0.35));
	};
	std::unordered_map<unsigned int, std::shared_ptr<IStaffMember>> storage_;
	std::unordered_map<emp_type, std::shared_ptr<rate_t>> rates_;
	static unsigned int id_;
};




