#pragma once

namespace anim
{
	class ProjectFile;

	class OpenFile : public std::enable_shared_from_this<OpenFile>
	{
	public:
		OpenFile(std::shared_ptr<ProjectFile> file);
		~OpenFile();

	private:
		std::shared_ptr<ProjectFile> file;
	};
}
