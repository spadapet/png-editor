#pragma once

namespace anim
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public interface class ILayerVM : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
		void Draw(Microsoft::Graphics::Canvas::CanvasDrawingSession ^session, Windows::Foundation::Rect rect);
	};
}
