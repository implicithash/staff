#include "store_manager.h"

std::shared_ptr<IStaffMember> Storage::getEmployee(const unsigned int id)
{
	const auto it = storage_.find(id);

	if (it != storage_.end())
	{
		return it->second;
	}

	return nullptr;
}

bool Storage::getEmployees(std::vector<std::shared_ptr<IStaffMember>>& records)
{
	for (auto& e : storage_)
	{
		records.emplace_back(e.second);
	}
	return true;
}

std::shared_ptr<rate_t> Storage::getRate(const emp_type type)
{
	const auto it = rates_.find(type);

	if (it != rates_.end())
	{
		return it->second;
	}

	return nullptr;
}

unsigned int Storage::id_ = 1;