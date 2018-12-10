#include "Types.h"
#include "SGQRayCast.h"
#include "..\ObjectCollection.h"
#include "SGQComponents.h"
#include "..\Components\__Components__.h"

namespace Bolt
{

	bool TestAABB(const Ray& ray, const Cuboid& bounds, Vector3f* outHitPosition)
	{
		Vector3f dirfrac;
		dirfrac.x = 1.0f / ray.Direction.x;
		dirfrac.y = 1.0f / ray.Direction.y;
		dirfrac.z = 1.0f / ray.Direction.z;
		// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
		// r.org is origin of ray
		Vector3f lb = bounds.Min;
		Vector3f rt = bounds.Max;
		float t1 = (lb.x - ray.Origin.x)*dirfrac.x;
		float t2 = (rt.x - ray.Origin.x)*dirfrac.x;
		float t3 = (lb.y - ray.Origin.y)*dirfrac.y;
		float t4 = (rt.y - ray.Origin.y)*dirfrac.y;
		float t5 = (lb.z - ray.Origin.z)*dirfrac.z;
		float t6 = (rt.z - ray.Origin.z)*dirfrac.z;

		float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
		float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

		// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
		if (tmax < 0)
		{
			*outHitPosition = ray.Origin + tmax * ray.Direction;
			return false;
		}

		// if tmin > tmax, ray doesn't intersect AABB
		if (tmin > tmax)
		{
			return false;
		}

		*outHitPosition = ray.Origin + tmin * ray.Direction;
		return true;
	}

	SGQRayCast::SGQRayCast(Ray ray) : SGQuery(),
		ray(ray)
	{
	
	}

	std::vector<GameObject*> SGQRayCast::Evaluate(const ObjectCollection& graph, GameObject** mostRelevant, GameObject** leastRelevant) const
	{
		std::vector<std::pair<GameObject*, Vector3f>> result;

		std::vector<GameObject*> objects = graph.Query(SGQComponents({ typeid(MeshRenderer) })).GameObjects;
		for (GameObject* object : objects)
		{
			MeshRenderer& meshRenderer = object->Components().GetComponent<MeshRenderer>();
			Transform& transform = object->transform();
			Cuboid bounds = meshRenderer.GetMeshBounds();
			Vector3f hitPosition;
			if (TestAABB(ray, bounds, &hitPosition))
			{
				result.push_back({ object, hitPosition });
			}
		}

		Vector3f rayOrigin = ray.Origin;
		std::sort(result.begin(), result.end(), [&rayOrigin](const std::pair<GameObject*, Vector3f>& left, const std::pair<GameObject*, Vector3f>& right)
		{
			return (left.second - rayOrigin).LengthSqrd() <= (right.second - rayOrigin).LengthSqrd();
		});
		if (result.size() > 0)
		{
			*mostRelevant = result.front().first;
			*leastRelevant = result.back().first;
		}
		else
		{
			*mostRelevant = nullptr;
			*leastRelevant = nullptr;
		}
		std::vector<GameObject*> objectResult;
		objectResult.reserve(result.size());
		for (auto& pair : result)
		{
			objectResult.push_back(pair.first);
		}
		return objectResult;
	}

}