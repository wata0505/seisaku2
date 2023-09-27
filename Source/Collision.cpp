#include "Collision.h"


//���Ƌ��̌�������
bool Collision::IntersectSphereVsSphere(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	DirectX::XMFLOAT3& outPositionB)
{
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	//��������
	float range = radiusA + radiusB;
	if (lengthSq > range * range)
	{
		return false;
	}

	// A��B�������o��
	Vec = DirectX::XMVector3Normalize(Vec);
	Vec = DirectX::XMVectorScale(Vec, range);
	PositionB = DirectX::XMVectorAdd(PositionA, Vec);
	DirectX::XMStoreFloat3(&outPositionB, PositionB);

	return true;
}

bool Collision::IntersectCylinderVsCylinder(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	float heightA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	float heightB,
	DirectX::XMFLOAT3& outPositionB)
{
	//A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
	if (positionA.y > positionB.y + heightB)
	{
		return false;
	}

	// A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
	if (positionA.y + heightA < positionB.y)
	{
		return false;
	}

	// XZ���ʂł͈̔̓`�F�b�N
	float vx = positionB.x - positionA.x;
	float vz = positionB.z - positionA.z;
	float range = radiusA + radiusB;
	float lengthSq = (vx * vx) + (vz * vz);
	if (lengthSq > range * range)
	{
		return false;
	}
	//�P�ʃx�N�g����
	float length = sqrtf(lengthSq);
	vx /= length;
	vz /= length;

	// A��B�������o��
	outPositionB.x = positionA.x + vx * range;
	outPositionB.y = positionB.y;
	outPositionB.z = positionA.z + vz * range;
	return true;
}

// ���Ɖ~���̌�������
bool Collision::IntersectSphereVsCylinder(
	const DirectX::XMFLOAT3& spherePosition,
	float sphereRadius,
	const DirectX::XMFLOAT3& cylinderPosition,
	float cylinderRadius,
	float cylinderHeight,
	DirectX::XMFLOAT3& outCylinderPosition)
{
	// �����`�F�b�N
	if (spherePosition.y + sphereRadius < cylinderPosition.y) return false;
	if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderHeight) return false;
	// XZ���ʂł͈̔̓`�F�b�N
	float vx = cylinderPosition.x - spherePosition.x;
	float vz = cylinderPosition.z - spherePosition.z;
	float range = sphereRadius + cylinderRadius;
	float distXZ = sqrtf(vx * vx + vz * vz);
	if (distXZ > range) return false;
	// �����~���������o��
	vx /= distXZ;
	vz /= distXZ;
	outCylinderPosition.x = spherePosition.x + (vx * range);
	outCylinderPosition.y = cylinderPosition.y;
	outCylinderPosition.z = spherePosition.z + (vz * range);

	return true;

}
//�J�v�Z���Ƌ�
bool Collision::IntersectCapsuleVsSphere(
	const DirectX::XMFLOAT3& CapPosition,
	const DirectX::XMFLOAT3& CapEndPosition,
	float CapRadius,
	const DirectX::XMFLOAT3& spherePosition,
	float sphereRadius)
{
	DirectX::XMVECTOR capPos = DirectX::XMLoadFloat3(&CapPosition);
	DirectX::XMVECTOR capEndPos = DirectX::XMLoadFloat3(&CapEndPosition);
	DirectX::XMVECTOR direction = { DirectX::XMVectorSubtract(capEndPos,capPos) };	// �J�v�Z���̉~�������̒��S�x�N�g��

	DirectX::XMVECTOR spPos = DirectX::XMLoadFloat3(&spherePosition);

	float l = DirectX::XMVectorGetX(DirectX::XMVector3Length(direction));	// �J�v�Z���̉~�������̒��S�x�N�g���̒���

	// direction�̐��K��
	direction = DirectX::XMVector3Normalize(direction);

	float t = {};	// �ˉe��
	DirectX::XMVECTOR nearP = {};	// �~�������̒��S����̍ŋߓ_
	// �ˉe���̎Z�o
	DirectX::XMVECTOR v = { DirectX::XMVectorSubtract(spPos,capPos) };
	t = DirectX::XMVectorGetX(DirectX::XMVector3Dot(v, direction));
	nearP = DirectX::XMVectorAdd(capPos, DirectX::XMVectorScale(direction, t));
	if (t < 0) {
		nearP = capPos;
	}
	if (t > l) {
		nearP = capEndPos;
	}
	// ��������B�������Ă��Ȃ��ꍇ��return����
	DirectX::XMVECTOR nearP2sp = DirectX::XMVectorSubtract(spPos, nearP);
	float leng = DirectX::XMVectorGetX(DirectX::XMVector3Length(nearP2sp));
	if (CapRadius + sphereRadius < leng) {
		return false;
	}
	// nearP2sp�̐��K����@���Ƃ���CollideSphere�֐����Ăяo���B
	nearP2sp = DirectX::XMVector3Normalize(nearP2sp);

	return true;

}

 bool Collision::IntersectFanVsSphere(
	const DirectX::XMFLOAT3& fanPosition,
	const DirectX::XMFLOAT3& fanDir,
	float rangeDegree,
	float fanLength,
	const DirectX::XMFLOAT3& objPosition,
	float objRadius
){
	DirectX::XMFLOAT2 fanPoint = {
	fanPosition.x - objPosition.x,
	fanPosition.z - objPosition.z };

	float vecLength = sqrtf((fanPoint.x *fanPoint.x) + (fanPoint.y * fanPoint.y));

	if (fanLength < vecLength)
	{
		// �������Ă��Ȃ�
		return false;
	}
	// ��Ɠ_�̃x�N�g����P�ʃx�N�g���ɂ���
	DirectX::XMFLOAT2 normalFanPoint = {
	fanPoint.x/ vecLength,
	fanPoint.y/ vecLength };

	// ���όv�Z
	float dot = normalFanPoint.x * -fanDir.x + normalFanPoint.y * -fanDir.z;

	// ��͈̔͂�cos�ɂ���
	float fanCos = cosf(rangeDegree);

	// �_����͈͓̔��ɂ��邩���r����
	if (fanCos > dot)
	{
		// �������ĂȂ�
		return false;
	}
	return true;
}
bool Collision::IntersectRayVsCylinder(
	const DirectX::XMFLOAT3& start,
	const DirectX::XMFLOAT3& end,
	const DirectX::XMFLOAT3& cylinderPosition,
	float cylinderRadius,
	float cylinderHeight,
	DirectX::XMFLOAT3& outCylinderPosition)
{
	//�����`�F�b�N
	//if (end.y  > cylinderPosition.y + cylinderHeight)
	//{
	//    return false;
	//}
	//if (end.y < cylinderPosition.y + cylinderHeight)
	//{
	//    return false;
	//}

	float range = cylinderRadius;
	float vx = cylinderPosition.x - end.x;
	float vz = cylinderPosition.z - end.z;
	float vx2 = cylinderPosition.x - start.x;
	float vz2 = cylinderPosition.z - start.z;
	float lengthSq = (vx * vx + vz * vz);
	float lengthSq2 = (vx2 * vx2 + vz2 * vz2);
	if (lengthSq > range && lengthSq2 > range)
	{
		return false;
	}

	//�P�ʃx�N�g����
	float length = sqrtf(lengthSq);
	vx /= length;
	vz /= length;


	outCylinderPosition.x = start.x + vx * range;
	outCylinderPosition.y = cylinderPosition.y;
	outCylinderPosition.z = start.z + vz * range;
	return true;
}
// ���C�ƃ��f���̌�������
bool Collision::IntersectRayVsModel(
	const DirectX::XMFLOAT3& start,
	const DirectX::XMFLOAT3& end,
	const Model* model,
	HitResult& result,
	const DirectX::XMFLOAT4X4& world)
{
	DirectX::XMVECTOR WorldStart   = DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR WorldEnd     = DirectX::XMLoadFloat3(&end);
	DirectX::XMVECTOR WorldRayVec    = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
	DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

	// ���[���h��Ԃ̃��C�̒���
	DirectX::XMStoreFloat(&result.distance, WorldRayLength);

	bool hit = false;
	const SkinnedMeshResouurce* resource = model->GetResource();
	for (const SkinnedMeshResouurce::Mesh& mesh : resource->GetMeshes())
	{
		Animation::Keyframe::node node{};
		// ���b�V���m�[�h�擾
		if (resource->AnimationClips.size() > 0) {
			node = model->GetKeyframe().nodes.at(mesh.nodeIndex);
		}
		else
		{
			node.globalTransform = mesh.defaultGlobalTransform;
		}
		XMStoreFloat4x4(&node.globalTransform, XMLoadFloat4x4(&node.globalTransform) * XMLoadFloat4x4(&world));
		// ���C�����[���h��Ԃ��烍�[�J����Ԃ֕ϊ�
		DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.globalTransform);
		DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

		DirectX::XMVECTOR Start = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
		DirectX::XMVECTOR End = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
		DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
		DirectX::XMVECTOR Dir = DirectX::XMVector3Normalize(Vec);
		DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);

		// ���C�̒���
		float neart;
		DirectX::XMStoreFloat(&neart, Length);

		// �O�p�`(��)�Ƃ̌�������@
		// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@����O�p�`�𒊏o����
		const std::vector<SkinnedMeshResouurce::Vertex>& vertices = mesh.vertices;
		const std::vector<UINT>& indices = mesh.indices;

		int materialIndex = -1;
		DirectX::XMVECTOR HitPosition;
		DirectX::XMVECTOR HitNormal;
		for (const SkinnedMeshResouurce::Mesh::subset& subset : mesh.subsets)
		{
			for (UINT i = 0; i < subset.indexCount; i += 3)
			{
				UINT index = subset.startIndexLocation + i;

				// �O�p�`�̒��_�𒊏o
				const SkinnedMeshResouurce::Vertex& a = vertices.at(indices.at(index));
				const SkinnedMeshResouurce::Vertex& b = vertices.at(indices.at(index + 1));
				const SkinnedMeshResouurce::Vertex& c = vertices.at(indices.at(index + 2));

				DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
				DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
				DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

				// �O�p�`�̎O�Ӄx�N�g�����Z�o
				DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
				DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
				DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

				//�O�p�`�̖@���x�N�g�����Z�o
				DirectX::XMVECTOR Normal = DirectX::XMVector3Cross(AB, BC);

				// ���ς̌��ʂ��v���X�Ȃ�Η�����
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Dir, Normal);
				float dot;
				DirectX::XMStoreFloat(&dot, Dot);
				if (dot >= 0) continue;

				// ���C�ƕ��ʂ̌�_���Z�o
				DirectX::XMVECTOR V = DirectX::XMVectorSubtract(A, Start);
				DirectX::XMVECTOR T = DirectX::XMVectorDivide(DirectX::XMVector3Dot(Normal, V), Dot);
				float t;
				DirectX::XMStoreFloat(&t, T);
				if (t < .0f || t > neart)continue;	// ��_�܂ł̋��������܂łɌv�Z�����ŋߋ������
													// �傫���Ƃ��̓X�L�b�v
				DirectX::XMVECTOR Position = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(Dir, T), Start);

				// ��_���O�p�`�̓����ɂ��邩�̔���
				// �P��
				DirectX::XMVECTOR V1 = DirectX::XMVectorSubtract(A, Position);
				DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(V1, AB);
				DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, Normal);
				float dot1;
				DirectX::XMStoreFloat(&dot1, Dot1);
				if (dot1 < 0.0f)continue;

				// 2��
				DirectX::XMVECTOR V2 = DirectX::XMVectorSubtract(B, Position);
				DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(V2, BC);
				DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, Normal);
				float dot2;
				DirectX::XMStoreFloat(&dot2, Dot2);
				if (dot2 < 0.0f)continue;

				// 3��
				DirectX::XMVECTOR V3 = DirectX::XMVectorSubtract(C, Position);
				DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(V3, CA);
				DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, Normal);
				float dot3;
				DirectX::XMStoreFloat(&dot3, Dot3);
				if (dot3 < 0.0f)continue;

				// �ŒZ�������X�V
				neart = t;

				// ��_�Ɩ@�����X�V
				HitPosition = Position;
				HitNormal = Normal;
				materialIndex = subset.materialUniqueId;
			}
		}
		if (materialIndex >= 0)
		{
			// ���[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
			DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
			DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
			DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
			float distance;
			DirectX::XMStoreFloat(&distance, WorldCrossLength);

			// �q�b�g���ۑ�
			if (result.distance > distance) {
				DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);
				result.distance = distance;
				result.materialIndex = materialIndex;
				DirectX::XMStoreFloat3(&result.position, WorldPosition);
				DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
				hit = true;
			}
		}

	}


	return hit;
}

bool Collision::IntersectCubeVsCylinder(
	const DirectX::XMFLOAT3& CubePosition,
	float CubeHalf,
	const DirectX::XMFLOAT3& CylinderPosition,
	float cylinderRadius,
	float cylinderHeight,
	DirectX::XMFLOAT3& outCylinderPosition
)
{
	DirectX::XMFLOAT3 maxPosition = { CubePosition.x + CubeHalf,CubePosition.y + CubeHalf,CubePosition.z + CubeHalf };
	DirectX::XMFLOAT3 minPosition = { CubePosition.x - CubeHalf,CubePosition.y - CubeHalf,CubePosition.z - CubeHalf };
	// �����`�F�b�N
	if (maxPosition.y < CylinderPosition.y)return false;
	if (minPosition.y > CylinderPosition.y + cylinderHeight)return false;

	// XZ�ł͈̔̓`�F�b�N

	return false;
}
