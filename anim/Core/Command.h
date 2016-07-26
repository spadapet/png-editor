#pragma once

namespace anim
{
	private ref class Command sealed : Windows::UI::Xaml::Input::ICommand
	{
	internal:
		Command(std::function<void(Platform::Object ^)> &&execute);
		Command(std::function<void(Platform::Object ^)> &&execute, std::function<bool(Platform::Object ^)> &&canExecute);

	public:
		void NotifyCanExecuteChanged();

		// ICommand
		virtual event Windows::Foundation::EventHandler<Platform::Object ^> ^CanExecuteChanged;
		virtual bool CanExecute(Platform::Object ^parameter);
		virtual void Execute(Platform::Object ^parameter);

	private:
		std::function<bool(Platform::Object ^)> canExecute;
		std::function<void(Platform::Object ^)> execute;
	};
}
