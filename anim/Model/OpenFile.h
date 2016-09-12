#pragma once

namespace anim
{
	class ProjectFile;

	class OpenFile : public std::enable_shared_from_this<OpenFile>
	{
	public:
		OpenFile(std::shared_ptr<ProjectFile> file);
		~OpenFile();

		virtual void Initialize();
		virtual void Destroy();

		std::shared_ptr<ProjectFile> GetFile() const;

	private:
		std::shared_ptr<ProjectFile> file;
	};
}
