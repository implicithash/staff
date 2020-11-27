#include "Staff.h"
#include <algorithm>

void Manager::addSubStaff(const int id)
{
	sub_staff_.push_back(id);
};

void Manager::removeSubStaff(const int id)
{
	sub_staff_.erase(std::remove(sub_staff_.begin(), sub_staff_.end(), id), sub_staff_.end());
}

void Sales::addSubStaff(const int id)
{
	sub_staff_.push_back(id);
};

void Sales::removeSubStaff(const int id)
{
	sub_staff_.erase(std::remove(sub_staff_.begin(), sub_staff_.end(), id), sub_staff_.end());
}
