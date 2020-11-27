#pragma once
#include <iostream>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace boost::gregorian;

enum class emp_type { employee, manager, sales };

/// <summary>
/// Abstract staff member
/// </summary>
class IStaffMember
{
public:
	IStaffMember()
	{}
	virtual ~IStaffMember() = default;

	virtual std::string name() const = 0;

	const double base_rate = 10000;
protected:
	std::string name_;
	date date_;
};

typedef struct
{
	unsigned int id;
	emp_type type;
	unsigned int chief_id;
	std::string s_date;
	std::string name;
	std::vector<int> sub_staff;
} item_t;

/// <summary>
/// Employee
/// </summary>
class Employee : public IStaffMember
{
public:
	explicit Employee(const item_t item) : chief_id_(item.chief_id), type_(item.type)
	{
		id_ = item.id;
		name_ = item.name;
		date_ = from_string(item.s_date);
	}
	Employee(const Employee&) = delete;
	Employee(Employee&&) = delete;
	Employee& operator = (const Employee&) = delete;
	Employee& operator = (Employee&&) = delete;
	~Employee() = default;
	
	std::string name() const override { return name_; }

	emp_type type() const { return type_; }
	unsigned int id() const { return id_; }
	date date() const { return date_; }
private:
	unsigned int id_;
	unsigned int chief_id_;
	emp_type type_;
};

/// <summary>
/// Manager
/// </summary>
class Manager : public IStaffMember
{
public:
	explicit Manager(item_t item) : chief_id_(item.chief_id), type_(item.type), sub_staff_(std::move(item.sub_staff))
	{
		id_ = item.id;
		name_ = item.name;
		date_ = from_string(item.s_date);
	}
	Manager(const Manager&) = delete;
	Manager(Manager&&) = delete;
	Manager& operator = (const Manager&) = delete;
	Manager& operator = (Manager&&) = delete;
	~Manager() = default;
	
	void addSubStaff(const int id);
	void removeSubStaff(const int id);

	std::string name() const override { return name_; }

	emp_type type() const { return type_; }
	unsigned int id() const { return id_; }
	date date() const { return date_; }
	std::vector<int>& sub_staff() { return sub_staff_; }
private:
	unsigned int id_;
	unsigned int chief_id_;
	emp_type type_;
	std::vector<int> sub_staff_;
};

/// <summary>
/// Sales
/// </summary>
class Sales : public IStaffMember
{
public:
	explicit Sales(item_t item) : chief_id_(item.chief_id), type_(item.type), sub_staff_(std::move(item.sub_staff))
	{
		id_ = item.id;
		name_ = item.name;
		date_ = from_string(item.s_date);
	}
	Sales(const Sales&) = delete;
	Sales(Sales&&) = delete;
	Sales& operator = (const Sales&) = delete;
	Sales& operator = (Sales&&) = delete;
	~Sales() = default;

	void addSubStaff(const int id);
	void removeSubStaff(const int id);

	std::string name() const override { return name_; }

	emp_type type() const { return type_; }
	unsigned int id() const { return id_; }
	date date() const { return date_; }
	std::vector<int>& sub_staff() { return sub_staff_; }
private:
	unsigned int id_;
	unsigned int chief_id_;
	emp_type type_;
	std::vector<int> sub_staff_;
};

