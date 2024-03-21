#pragma once
#include <stdint.h>
#include <string>
#include <variant>
#include <optional>

struct FInputDeviceId_t {
	struct _Tag {};
	constexpr explicit FInputDeviceId_t(_Tag) {}
};
inline constexpr FInputDeviceId_t INPUTDEVICEID_NONE{ FInputDeviceId_t::_Tag{} };

struct FInputDeviceId
{
	constexpr FInputDeviceId(uint32_t id) {
		InternalId = id;
	}

	constexpr FInputDeviceId(){}

	constexpr FInputDeviceId(FInputDeviceId_t id) {}

	bool IsValid() const
	{
		return !InternalId.has_value();
	}

	uint32_t GetInternalId() const
	{
		if (!IsValid()) {
			return 0;
		}
		return InternalId.value();
	}


	bool operator==(const FInputDeviceId& Other) const
	{
		return InternalId == Other.InternalId;
	}

	bool operator==(const FInputDeviceId_t& Other) const
	{
		return !InternalId.has_value();
	}

	bool operator!=(const FInputDeviceId& Other) const
	{
		return InternalId != Other.InternalId;
	}

	bool operator!=(const FInputDeviceId_t& Other) const
	{
		return InternalId.has_value();
	}

	struct FInputDeviceId& operator=(this struct FInputDeviceId& self,uint32_t id)
	{
		self.InternalId = id;
		return self;
	}
private:
	std::optional <uint32_t> InternalId;
};


