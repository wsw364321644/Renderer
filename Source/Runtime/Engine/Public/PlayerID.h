#pragma once
#include <stdint.h>
#include <string>
#include <variant>
#include <optional>

struct FPlatformUserId_t {
	struct _Tag {};
	constexpr explicit FPlatformUserId_t(_Tag) {}
};
inline constexpr FPlatformUserId_t PLATFORMUSERID_NONE{ FPlatformUserId_t::_Tag{} };

struct FPlatformUserId
{
	typedef std::variant<std::string, uint32_t> IdType;
	FPlatformUserId(std::string id) {
		InternalIdOpt = id;
	}

	constexpr FPlatformUserId(uint32_t id) {
		InternalIdOpt = id;
	}

	constexpr FPlatformUserId(){}

	constexpr FPlatformUserId(FPlatformUserId_t id) {}

	bool IsValid() const
	{
		return !InternalIdOpt.has_value();
	}

	/** Returns the internal id for debugging/etc */
	std::string_view GetInternalIdStr() const
	{
		if (!IsValid()) {
			return "";
		}
		auto& InternalId= InternalIdOpt.value();
		if (!std::holds_alternative<std::string>(InternalId)) {
			return "";
		}
		return std::get<std::string>(InternalId);
	}

	uint32_t GetInternalIdNum() const
	{
		if (!IsValid()) {
			return 0;
		}
		auto& InternalId = InternalIdOpt.value();
		if (!std::holds_alternative<uint32_t>(InternalId)) {
			return 0;
		}
		return std::get<uint32_t>(InternalId);
	}


	bool operator==(const FPlatformUserId& Other) const
	{
		return InternalIdOpt == Other.InternalIdOpt;
	}

	bool operator==(const FPlatformUserId_t& Other) const
	{
		return !InternalIdOpt.has_value();
	}

	bool operator!=(const FPlatformUserId& Other) const
	{
		return InternalIdOpt != Other.InternalIdOpt;
	}

	bool operator!=(const FPlatformUserId_t& Other) const
	{
		return InternalIdOpt.has_value();
	}

	struct FPlatformUserId& operator=(this struct FPlatformUserId& self, const char* id)
	{
		self.InternalIdOpt =id;
		return self;
	}

	struct FPlatformUserId& operator=(this struct FPlatformUserId& self,uint32_t id)
	{
		self.InternalIdOpt = id;
		return self;
	}
private:
	std::optional <IdType> InternalIdOpt;
};


