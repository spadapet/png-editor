#pragma once

#include "Core/Event.h"

namespace anim
{
	class GraphDevice;
	class OpenFile;
	class PaneInfo;
	class ProjectFile;
	class ProjectFolder;
	class ProjectItem;

	enum class AppMode
	{
		None,
		Edit,
	};

	class AppState : public std::enable_shared_from_this<AppState>
	{
	public:
		AppState();
		~AppState();

		static std::shared_ptr<AppState> Create();
		static std::shared_ptr<AppState> CreateForDesigner();

		// Events
		ChangedEvent PropertyChanged;
		Event<std::shared_ptr<ProjectFolder>> ProjectFolderAdded;
		Event<std::shared_ptr<ProjectFolder>> ProjectFolderRemoved;
		Event<std::shared_ptr<OpenFile>> FileOpened;
		Event<std::shared_ptr<OpenFile>> FileClosed;
		Event<std::shared_ptr<OpenFile>, bool &> FileFocus;

		// Properties
		const std::vector<std::shared_ptr<OpenFile>> &GetOpenFiles() const;
		const std::vector<std::shared_ptr<PaneInfo>> &GetPanes() const;
		const std::vector<std::shared_ptr<anim::ProjectFolder>> &GetProjectFolders() const;
		std::shared_ptr<PaneInfo> GetNonePane() const;
		AppMode GetMode() const;
		std::shared_ptr<GraphDevice> GetGraph() const;

		// Methods
		void AddProjectFolder(Windows::Storage::StorageFolder ^folder);
		void RemoveProjectFolder(Windows::Storage::StorageFolder ^folder);
		std::shared_ptr<OpenFile> EditFile(Windows::Storage::StorageFile ^file, bool focus = true);
		std::shared_ptr<OpenFile> EditFile(std::shared_ptr<ProjectFile> file, bool focus = true);
		void CloseFile(std::shared_ptr<OpenFile> openFile);

		std::shared_ptr<ProjectItem> RegisterProjectItem(std::shared_ptr<ProjectItem> parent, Windows::Storage::IStorageItem ^item);
		void UnregisterProjectItem(Platform::String ^path);

		// Logical DPI
		void SetDpi(double dpi);
		double PixelToDip(double pixel);
		double DipToPixel(double dip);
		RectFloat PixelToDip(RectFloat pixel);
		RectFloat DipToPixel(RectFloat dip);
		RectDouble PixelToDip(RectDouble pixel);
		RectDouble DipToPixel(RectDouble dip);

	private:
		void Initialize();
		void InitializeForDesigner();
		void ResetPanes();

		std::shared_ptr<PaneInfo> nonePane;
		std::vector<std::shared_ptr<PaneInfo>> panes;
		std::vector<std::shared_ptr<ProjectFolder>> projectFolders;
		std::vector<std::shared_ptr<OpenFile>> openFiles;
		std::unordered_map<std::string, std::weak_ptr<ProjectItem>> itemCache;
		std::shared_ptr<GraphDevice> graph;
		double dpiScale;
	};
}
