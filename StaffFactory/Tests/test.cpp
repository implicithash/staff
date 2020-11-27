#include "pch.h"

#include "../StaffFactory/store_manager.h"
#include "../StaffFactory/calc_salary.h"
#include "../StaffFactory/staff.h"
#include <mutex>

namespace staff
{
	class StaffStorageTest : public ::testing::Test
	{
	public:
		StaffStorageTest(const StaffStorageTest&) = delete;
		StaffStorageTest(StaffStorageTest&&) = delete;
		StaffStorageTest& operator = (const StaffStorageTest&) = delete;
		StaffStorageTest& operator = (StaffStorageTest&&) = delete;
		StaffStorageTest() = default;
		~StaffStorageTest() = default;

		template<typename T>
		void createEmployee();
		void calcEmployeeFirstYearSalary();
		void calcEmployeeSecondYearSalary();
		void calcSalesFirstYearSalary();
		void calcManagerFirstYearSalary();
	};

	template<typename T>
	void StaffStorageTest::createEmployee()
	{
		item_t t1 = { .id = 15, .chief_id = 0, .s_date = "2016/5/7", .name = "gmaltsev" };
		auto result = Storage::get()->create<T>(t1);
		EXPECT_TRUE(result);
		
		const auto existed_emp_ptr = Storage::get()->getEmployee(15);
		auto ptr = dynamic_cast<T*>(existed_emp_ptr.get());
		EXPECT_NE(ptr, nullptr);

		EXPECT_EQ(ptr->id(), 15);

		const auto null_emp_ptr = Storage::get()->getEmployee(16);
		ptr = dynamic_cast<T*>(null_emp_ptr.get());
		EXPECT_EQ(ptr, nullptr);
	}
	
	void StaffStorageTest::calcEmployeeFirstYearSalary()
	{
		item_t t1 = { .id = 10, .type = emp_type::employee, .chief_id = 0, .s_date = "2020/1/18", .name = "gmaltsev" };
		auto result = Storage::get()->create<Employee>(t1);

		date dt(2021, 11, 22);
		auto salary = SCalculator::get()->calc<Employee>(dt, 10);

		// employee salary after first year
		EXPECT_EQ(salary, 10300);
	}

	void StaffStorageTest::calcEmployeeSecondYearSalary()
	{
		item_t t1 = { .id = 11, .type = emp_type::employee, .chief_id = 0, .s_date = "2019/1/18", .name = "gmaltsev" };
		auto result = Storage::get()->create<Employee>(t1);

		date dt(2021, 11, 22);
		auto salary = SCalculator::get()->calc<Employee>(dt, 11);

		// employee salary after second year
		EXPECT_EQ(salary, 10609);
	}

	void StaffStorageTest::calcSalesFirstYearSalary()
	{
		item_t t1 = { .id = 1, .type = emp_type::sales, .chief_id = 0, .s_date = "2019/1/18", .name = "gmaltsev", .sub_staff = {2,3} };
		auto result = Storage::get()->create<Sales>(t1);

		item_t t2 = { .id = 2, .type = emp_type::manager, .chief_id = 1, .s_date = "2019/6/18", .name = "vnikitin", .sub_staff = {4, 5} };
		result = Storage::get()->create<Manager>(t2);

		item_t t3 = { .id = 3, .type = emp_type::employee, .chief_id = 1, .s_date = "2020/3/18", .name = "amazeev" };
		result = Storage::get()->create<Employee>(t3);

		item_t t4 = { .id = 4, .type = emp_type::sales, .chief_id = 2, .s_date = "2020/2/18", .name = "asemenov" };
		result = Storage::get()->create<Sales>(t4);

		item_t t5 = { .id = 5, .type = emp_type::employee, .chief_id = 2, .s_date = "2020/1/18", .name = "dmalyshev" };
		result = Storage::get()->create<Employee>(t5);

		date dt(2020, 11, 22);
		auto salary = SCalculator::get()->calc<Sales>(dt, 1);

		// sales salary after first year
		EXPECT_EQ(salary, 10220);
	}

	void StaffStorageTest::calcManagerFirstYearSalary()
	{
		item_t t1 = { .id = 12, .type = emp_type::manager, .chief_id = 0, .s_date = "2019/1/18", .name = "gmaltsev", .sub_staff = {13,14} };
		auto result = Storage::get()->create<Manager>(t1);

		item_t t2 = { .id = 13, .type = emp_type::employee, .chief_id = 12, .s_date = "2019/6/15", .name = "vnikitin", .sub_staff = {} };
		result = Storage::get()->create<Employee>(t2);

		item_t t3 = { .id = 14, .type = emp_type::employee, .chief_id = 12, .s_date = "2020/3/22", .name = "amazeev" };
		result = Storage::get()->create<Employee>(t3);

		date dt(2020, 11, 22);
		auto salary = SCalculator::get()->calc<Manager>(dt, 12);

		// manager salary after first year
		EXPECT_EQ(salary, 10600);
	}

	class StaffStorageTest_InMemStorage : public StaffStorageTest {};

	TEST_F(StaffStorageTest_InMemStorage, createEmployee)
	{
		ASSERT_NO_FATAL_FAILURE(createEmployee<Manager>());
	}
	TEST_F(StaffStorageTest_InMemStorage, calcEmployeeFirstYearSalary)
	{
		ASSERT_NO_FATAL_FAILURE(calcEmployeeFirstYearSalary());
	}

	TEST_F(StaffStorageTest_InMemStorage, calcEmployeeSecondYearSalary)
	{
		ASSERT_NO_FATAL_FAILURE(calcEmployeeSecondYearSalary());
	}
	TEST_F(StaffStorageTest_InMemStorage, calcManagerFirstYearSalary)
	{
		ASSERT_NO_FATAL_FAILURE(calcManagerFirstYearSalary());
	}

	TEST_F(StaffStorageTest_InMemStorage, calcSalesFirstYearSalary)
	{
		ASSERT_NO_FATAL_FAILURE(calcSalesFirstYearSalary());
	}

}

