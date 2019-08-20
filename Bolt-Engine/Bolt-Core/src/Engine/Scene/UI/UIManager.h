#pragma once
#include "UIElement.h"
#include "../ObjectFactory.h"

namespace Bolt
{

	class BLT_API UIManager
	{
	private:
		class BLT_API EventListenerHandler
		{
		private:
			struct BLT_API ListenerMapping
			{
			public:
				EventEmitterBase* Emitter;
				uint32_t Listener;
			};
		private:
			std::vector<ListenerMapping> m_Listeners;

		public:
			EventListenerHandler();
			~EventListenerHandler();

			void AddListener(EventEmitterBase* emitter, uint32_t listener);

			template<typename EmitterT>
			void AddListener(EventEmitter<EmitterT>& emitter, const typename EventEmitter<EmitterT>::callback_t& callback, ListenerPriority priority = ListenerPriority::Medium)
			{
				uint32_t listenerId = emitter.AddEventListener(callback, priority);
				AddListener((EventEmitterBase*)&emitter, listenerId);
			}
		};

	private:
		ObjectFactory m_Factory;
		std::unique_ptr<UIElement> m_RootElement;
		UIElement* m_FocusedElement;
		EventListenerHandler m_Listeners;

	public:
		UIManager(Layer* layer);

		void Initialize();

		const ObjectFactory& Factory() const;
		ObjectFactory& Factory();
		const UIElement& Root() const;
		UIElement& Root();

		void Clear() const;

		friend class UIElement;

	private:
		void SetFocusedElement(UIElement* element);
		std::vector<UIElement*> GetElementsUnderPoint(const Vector2f& point, int limit = -1) const;
		void GetElementsUnderPointRecursive(const Vector2f& point, UIElement* currentElement, std::vector<UIElement*>& elements, int limit) const;

	};

}