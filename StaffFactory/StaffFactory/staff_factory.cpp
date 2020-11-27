#include "store_manager.h"
#include "calc_salary.h"
#include "staff.h"

int main(int argc, char** argv)
{
	item_t t1 = { .id = 1, .type = emp_type::manager, .chief_id = 0, .s_date = "2020/6/18", .name = "gmaltsev", .sub_staff = {2,3} };
	auto result = Storage::get()->create<Manager>(t1);

	item_t t2 = { .id = 2, .type = emp_type::manager, .chief_id = 1, .s_date = "2019/6/18", .name = "vnikitin", .sub_staff = {4, 5} };
	result = Storage::get()->create<Manager>(t2);

	item_t t3 = { .id = 3, .type = emp_type::employee, .chief_id = 1, .s_date = "2020/3/18", .name = "amazeev" };
	result = Storage::get()->create<Employee>(t3);

	item_t t4 = { .id = 4, .type = emp_type::sales, .chief_id = 2, .s_date = "2020/2/18", .name = "asemenov" };
	result = Storage::get()->create<Sales>(t4);

	item_t t5 = { .id = 5, .type = emp_type::employee, .chief_id = 2, .s_date = "2020/1/18", .name = "dmalyshev" };
	result = Storage::get()->create<Manager>(t5);

	auto employee = Storage::get()->getEmployee(1);
	auto gl = dynamic_cast<Sales*>(employee.get());
	auto id = gl->id();

	date dt(2021, 7, 22);
	auto salary = SCalculator::get()->calc<Sales>(dt, 1);

	return 0;
}
