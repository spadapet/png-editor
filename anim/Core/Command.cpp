#include "pch.h"
#include "Core/Command.h"

anim::Command::Command(std::function<void(Platform::Object ^)> &&execute)
	: execute(execute)
{
}

anim::Command::Command(
	std::function<void(Platform::Object ^)> &&execute,
	std::function<bool(Platform::Object ^)> &&canExecute)
	: execute(execute)
	, canExecute(canExecute)
{
}

void anim::Command::NotifyCanExecuteChanged()
{
	this->CanExecuteChanged(this, nullptr);
}

bool anim::Command::CanExecute(Platform::Object ^parameter)
{
	return this->canExecute == nullptr || this->canExecute(parameter);
}

void anim::Command::Execute(Platform::Object ^parameter)
{
	if (this->execute != nullptr)
	{
		this->execute(parameter);
	}
}
