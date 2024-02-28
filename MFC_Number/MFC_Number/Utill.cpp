#include "pch.h"
#include "Utill.h"


DOUBLE Utill::GetRandomFloat()
{
	std::random_device rd;
	std::default_random_engine  gen(rd());

	std::uniform_real<FLOAT> dis(
		-(100000 - DBL_EPSILON),
		100000 - DBL_EPSILON
	);

	return dis(gen);
}

FLOAT Utill::GetRandomTest()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real<FLOAT> dis(
		-200,
		200
	);

	return dis(gen);
}
