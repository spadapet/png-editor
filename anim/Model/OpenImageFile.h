#pragma once

#include "Model/OpenFile.h"

namespace anim
{
	class OpenImageFile : public OpenFile
	{
	public:
		OpenImageFile(std::shared_ptr<ProjectFile> file);
		~OpenImageFile();

		virtual void Initialize() override;
		virtual void Destroy() override;

	private:
	};
}
