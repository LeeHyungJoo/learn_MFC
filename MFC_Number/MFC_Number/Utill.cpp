#include "pch.h"
#include "Utill.h"


DOUBLE Utill::GetRandomDouble()
{
	std::random_device rd;
	std::default_random_engine  gen(rd());

	std::uniform_real<DOUBLE> dis(
		//(std::numeric_limits<DOUBLE>::lowest)(),
		(std::numeric_limits<DOUBLE>::min)(),
		(std::numeric_limits<DOUBLE>::max)()
	);

	return dis(gen) * (rand() & 1 ? -1 : 1);
}

FLOAT Utill::GetRandomTest()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real<FLOAT> dis(
		-500,
		500
	);

	return dis(gen);
}
