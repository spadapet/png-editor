#pragma once

namespace anim
{
	class AppState;

	public ref class ViewAppState sealed
	{
	internal:
		ViewAppState(AppState *parent);

	public:
		virtual ~ViewAppState();

	private:
		void ModelPropertyChanged(const char *name);

		AppState *parent;
		size_t modelChangedCookie;
	};
}
