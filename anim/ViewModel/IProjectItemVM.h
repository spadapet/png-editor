#pragma once

namespace anim
{
	public interface class IProjectItemVM
	{
		property Windows::Storage::IStorageItem ^Item { Windows::Storage::IStorageItem ^get(); }
		property Platform::String ^DisplayName { Platform::String ^get(); }
		property Platform::String ^FullPath { Platform::String ^get(); }
		property int Level { int get(); }
		property bool IsOpen { bool get(); }
	};
}
