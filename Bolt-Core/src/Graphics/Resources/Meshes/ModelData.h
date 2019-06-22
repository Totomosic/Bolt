#pragma once
#include "Graphics/Buffers/VertexArray.h"
#include "Graphics/Buffers/IndexArray.h"

namespace Bolt
{

	struct BLT_API ModelMapping
	{
	public:
		VertexMapping VertexMap;
		IndexMapping IndexMap;
	};

	struct BLT_API ModelBounds
	{
	public:
		float MinX;
		float MaxX;
		float MinY;
		float MaxY;
		float MinZ;
		float MaxZ;
	};

	struct BLT_API ModelData
	{
	public:
		std::unique_ptr<VertexArray> Vertices;
		std::unique_ptr<IndexArray> Indices;
		ModelBounds Bounds;

	public:
		ModelMapping Map() const;

		template<typename FuncT0>
		void MapAsync(FuncT0 callback)
		{
			MapAsync(std::move(callback), std::function<void()>([]() {}));
		}
		
		template<typename FuncT0, typename FuncT1>
		void MapAsync(FuncT0 callback, FuncT1 finishedCallback) const
		{
			Task t = TaskManager::Run(make_shared_function([mapping{ Map() }, callback{ std::move(callback) }]() mutable
			{
				callback(mapping);
				return std::move(mapping);
			}));
			t.ContinueWithOnMainThread(make_shared_function([callback{ std::move(finishedCallback) }](ModelMapping mapping)
			{
				{
					ModelMapping temp = std::move(mapping);
				}
				callback();
			}));
		}

	};

}