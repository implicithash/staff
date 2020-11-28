#pragma once
//#include "staff.h"
#include "store_manager.h"

/// <summary>
/// Salary calculator
/// </summary>
class SCalculator final
{
public:
	static const int days_per_year = 356;
	static SCalculator* get() {
		static SCalculator instance;
		return &instance;
	}
	SCalculator(const SCalculator&) = delete;
	SCalculator& operator = (const SCalculator&) = delete;
	virtual ~SCalculator() = default;

	/// <summary>
	/// Calculates a salary based on dates of work and position percentage
	/// </summary>
	/// <typeparam name="T">Input type</typeparam>
	/// <param name="dt">Any future date which differs from the date of employment</param>
	/// <param name="id">Employee ID</param>
	/// <returns>Evaluated value</returns>
	template <typename T>
	double calc(date dt, const unsigned int id)
	{
		const auto emp_ptr = Storage::get()->getEmployee(id);
		if (emp_ptr == nullptr)
			return 0;

		const auto employee = dynamic_cast<T*>(emp_ptr.get());
		const auto rate = Storage::get()->getRate(employee->type());

		// count all the previous years
		const auto years = (dt - employee->date()).days() / days_per_year;

		auto cur_salary = employee->base_rate;

		const auto max_salary = cur_salary + cur_salary * rate->max_rate;
		if (std::is_same<T, Employee>::value)
		{
			cur_salary = calcCurSalary(years, max_salary, cur_salary, rate);
		}
		else if (std::is_same<T, Manager>::value)
		{
			auto staff_salary = calcSubLevel<Manager>(id);
			cur_salary = calcCurSalary(years, max_salary, cur_salary, rate, staff_salary);
		}
		else if (std::is_same<T, Sales>::value)
		{
			double staff_salary = 0;
			staff_salary = calcSubLevels<Sales>(id, staff_salary);
			cur_salary = calcCurSalary(years, max_salary, cur_salary, rate, staff_salary);
		}

		return cur_salary;
	}

private:
	SCalculator() {}
	template <typename T>
	double calcSubLevel(const unsigned int id)
	{
		const auto emp_ptr = Storage::get()->getEmployee(id);
		if (emp_ptr == nullptr)
			return 0;

		double staff_salary = 0;
		const auto employee = dynamic_cast<T*>(emp_ptr.get());
		for (auto& sub : employee->sub_staff())
		{
			const auto e_ptr = Storage::get()->getEmployee(id);
			if (e_ptr == nullptr)
				staff_salary += 0;
			staff_salary += e_ptr->base_rate;
		}
		return staff_salary;
	}

	template <typename T>
	double calcSubLevels(const unsigned int id, double& staff_salary)
	{
		const auto emp_ptr = Storage::get()->getEmployee(id);
		if (emp_ptr == nullptr) {
			staff_salary += 0;
			return staff_salary;
		}

		const auto employee = dynamic_cast<T*>(emp_ptr.get());

		for (auto& sub_id : employee->sub_staff())
		{
			const auto e_ptr = Storage::get()->getEmployee(sub_id);
			staff_salary += e_ptr->base_rate;

			if (dynamic_cast<Employee*>(e_ptr.get()))
				return staff_salary;
			else if (dynamic_cast<Manager*>(e_ptr.get()))
				staff_salary = calcSubLevels<Manager>(sub_id, staff_salary);
			else if (dynamic_cast<Sales*>(e_ptr.get()))
				staff_salary = calcSubLevels<Sales>(sub_id, staff_salary);
		}
		return staff_salary;
	}

	double calcCurSalary(const int& years, const double& max_salary, double& cur_salary, const std::shared_ptr<rate_t>& rate, const double& staff_salary = 0)
	{
		for (auto i = 0; i < years; ++i)
		{
			const auto indexed_salary = cur_salary + cur_salary * rate->year_rate + staff_salary * rate->sub_rate;
			cur_salary = indexed_salary > max_salary ? max_salary : indexed_salary;
		}
		return cur_salary;
	}
};

template <class D, class S>
bool IsType(const S* src)
{
	return dynamic_cast<const D*>(src) != nullptr;
}
