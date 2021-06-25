// Copyright Epic Games, Inc. All Rights Reserved.


#include "GenericPlatform\IDGenerator.h"


uuids::uuid IDGenerator::NewID()
{
	std::random_device rd;
	auto seed_data = std::array<int, 6> {};
	std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
	std::ranlux48_base generator(seq);

	uuids::basic_uuid_random_generator<std::ranlux48_base> gen(&generator);
	uuids::uuid const id = gen();
	return id;
}
