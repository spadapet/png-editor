#pragma once

namespace anim
{
	interface class ITreeHost;

	public ref class TreeRow sealed : Windows::UI::Xaml::Controls::Primitives::ButtonBase
	{
	public:
		TreeRow();
		virtual ~TreeRow();

		property bool IsSelected { bool get(); void set(bool value); }

	private:
		void UpdateVisualState();
		ITreeHost ^GetTreeHost();

		long long pointerOverToken;
	};
}
