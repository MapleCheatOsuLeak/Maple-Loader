#include "HardwareHelper.h"

#include <intrin.h>
#include <sstream>

#include "smbios.h"
#include "../../UI/UI.h"
#include "../Crypto/CryptoHelper.h"

std::string HardwareHelper::getGPUID()
{
	D3DADAPTER_IDENTIFIER9 id;
	if (!SUCCEEDED(UI::D3D->GetAdapterIdentifier(0, 0, &id)))
		return { };

	return std::string(id.Description) + std::string("-") + std::to_string(id.VendorId) + std::string("-") + std::to_string(id.DeviceId);
}

std::string HardwareHelper::getCPUVendor()
{
	int regs[4] = { 0 };
	char vendor[13];
	__cpuid(regs, 0);
	memcpy(vendor, &regs[1], 4);
	memcpy(vendor + 4, &regs[3], 4);
	memcpy(vendor + 8, &regs[2], 4);
	vendor[12] = '\0';

	return std::string(vendor);
}

std::string HardwareHelper::getMotherboardInfo()
{
	std::string manufacturer;
	std::string product;

	const DWORD smbios_data_size = GetSystemFirmwareTable('RSMB', 0, nullptr, 0);

	auto* const heap_handle = GetProcessHeap();
	auto* const smbios_data = static_cast<smbios::raw_smbios_data*>(HeapAlloc(heap_handle, 0, static_cast<size_t>(smbios_data_size)));
	if (!smbios_data)
	{
		return { };
	}

	const DWORD bytes_written = GetSystemFirmwareTable('RSMB', 0, smbios_data, smbios_data_size);
	if (bytes_written != smbios_data_size)
	{
		return { };
	}

	smbios::parser meta;
	const BYTE* buff = smbios_data->smbios_table_data;
	const auto buff_size = static_cast<size_t>(smbios_data_size);

	meta.feed(buff, buff_size);
	for (auto& header : meta.headers)
	{
		smbios::string_array_t strings;
		smbios::parser::extract_strings(header, strings);

		if (header->type == smbios::types::baseboard_info)
		{
			auto* const x = reinterpret_cast<smbios::baseboard_info*>(header);

			if (x->length == 0)
				break;

			manufacturer = strings[x->manufacturer_name];
			product = strings[x->product_name];
		}
	}

	HeapFree(heap_handle, 0, smbios_data);

	if (manufacturer.empty() || product.empty())
		return { };

	return manufacturer + '-' + product;
}

std::string HardwareHelper::GetHWID()
{
	std::string gpuID = getGPUID();
	std::string cpuVendor = getCPUVendor();
	std::string motherboardInfo = getMotherboardInfo();

	if (gpuID.empty() && cpuVendor.empty() && motherboardInfo.empty())
		return { };

	std::stringstream hwid;
	hwid << gpuID << "|" << cpuVendor << "|" << motherboardInfo;

	return CryptoHelper::GetMD5Hash(hwid.str());
}