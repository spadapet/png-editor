#pragma once

namespace anim
{
	ref class TreeRow;

	public interface class ITreeHost
	{
		property TreeRow ^SelectedTreeRow { TreeRow ^get(); void set(TreeRow ^value); }
	};
}
