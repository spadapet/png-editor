#pragma once

#include "Model/OpenFile.h"

namespace anim
{
	class Image;

	class OpenImageFile : public OpenFile
	{
	public:
		OpenImageFile(std::shared_ptr<ProjectFile> file);
		~OpenImageFile();

		void Reset(std::shared_ptr<Image> image, std::string fatalError);

		virtual void Initialize() override;
		virtual void Destroy() override;
		virtual bool IsDirty() const override;
		virtual bool IsLoaded() const override;
		virtual bool IsFatalError() const override;
		virtual std::string GetFatalError() const override;

		// Undo
		virtual size_t GetUndoCount() const override;
		virtual size_t GetRedoCount() const override;
		virtual void Undo() override;
		virtual void Redo() override;

	private:
		bool initialized;
		bool loading;
		bool dirty;
		std::string fatalError;
		std::shared_ptr<Image> image;
		// TODO: undo stack
	};
}
