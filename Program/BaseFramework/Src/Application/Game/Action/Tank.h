#include"../GameObject.h"

class Tank :public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;

	void Update()override;

	void UpdateCollision();

	int GetCrystal() { return m_crystal; }

	virtual void Draw2D() override;

private:

	std::shared_ptr<Texture> m_spTankTex;
	Matrix m_TankMat;

	bool m_hit = false;
	std::shared_ptr<GameObject> m_sphuman;

	int m_crystal = 0;
	float i = 0;
	float m_rot;
	int m_frame = 0;
	Vec3 pos;

};