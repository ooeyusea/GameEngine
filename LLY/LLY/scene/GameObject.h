/*
 * ��Ϸ������
 *
 * ���г����к���������Ϣ��Ԫ�أ��������������պе������Ԫ�أ����̳���GameObject
 * ���⣬GameObject������֯�����и�Ԫ��֮��Ĳ㼶��ϵ��ÿ��������һ��Ĭ�ϵ�root��
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
		 * ����ص�����
		 ************************************************************************/
		virtual void onEnter();			// ���볡��ʱ
		virtual void onExit();			// �뿪����ʱ
		virtual void onDestory();		// ����ʱ

		virtual void updateBefore();	// ÿ֡���ã���update֮ǰ����
		virtual void update();			// ÿ֡����
		virtual void updateAfter();		// ÿ֡���ã���update֮�����

	private:
		Transform _transform;

		GameObject* _parent;					// reference
		std::vector<GameObject*> _children;		// retain
	};
}

#endif // GAMEOBJECT_H_