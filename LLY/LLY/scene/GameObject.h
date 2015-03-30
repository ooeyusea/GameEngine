/*
 * 游戏对象类
 *
 * 所有场景中含有坐标信息的元素，包括摄像机、天空盒等特殊的元素，均继承自GameObject
 * 另外，GameObject用于组织场景中各元素之间的层级关系（每个场景有一个默认的root）
 */

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <vector>
#include "../math/Transform.h"

namespace lly {

	class GameObject
	{
	public:
		static void destory(GameObject* gameObject);

		GameObject();
		virtual ~GameObject();

		void setParent(GameObject* parent);
		GameObject* getParent();

		void addChild(GameObject* child);
		void removeChild(GameObject* child);
		void removeFromParent();

	protected:
		/************************************************************************
		 * 引擎回调函数
		 ************************************************************************/
		virtual void onEnter();			// 进入场景时
		virtual void onExit();			// 离开场景时
		virtual void onDestory();		// 销毁时

		virtual void updateBefore();	// 每帧调用，在update之前调用
		virtual void update();			// 每帧调用
		virtual void updateAfter();		// 每帧调用，在update之后调用

	private:
		Transform _transform;

		GameObject* _parent;					// reference
		std::vector<GameObject*> _children;		// retain
	};
}

#endif // GAMEOBJECT_H_