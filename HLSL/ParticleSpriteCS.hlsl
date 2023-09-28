#define SLASHING 0
#define DIFFUSION 1
#define CONVERGENCE 2
#define SLAH 3
#define EXPANSION 4
#define WEPONCONVERGENCE 5
#define CORRUPTION 6
#define CHILE 7

cbuffer global : register(b0)
{
	float4x4 View;       // �r���[�ϊ��s��
	float4x4 Projection; // �����ϊ��s��
}
cbuffer calculation : register(b1)
{
	float3 Pattern; // �r���[�ϊ��s��
	float3 Tail;  // �����ϊ��s��
	float  Max;
}
//�V�F�[�_�̓��̓f�[�^��`
struct GS_INPUT_GPU2 {
	float3 Position      : POSITION;     // ���_���W(���[���h���W�n)
	float3 Velocity      : VELOCITY;       // ���݂̑��x�x�N�g��(���[���h���W�n)
	float4 ParticleSize  : PARTICLESIZE;
	float  Speed         : SPEED;       // ���݂̉����x
	uint   ParticleType  : PARTICLETYPE;
	float  Timer         : TIMER;
};
//CPU�ւ̏�������
struct CPUGPUBuffer
{
	float3 toCpuPosition;      // ���W�l

};


static float3 Acceleration = float3(0.0, -4.8, 0.0);  // �����x(m/s^2)

// ���̓o�b�t�@(�\�����o�b�t�@�B�ǂݍ��ݐ�p)
StructuredBuffer<GS_INPUT_GPU2> Input : register(t0);
// �o�̓o�b�t�@(�\�����o�b�t�@�B�ǂݏ����\)
RWStructuredBuffer<GS_INPUT_GPU2> Result : register(u0);
//RWStructuredBuffer<CPUGPUBuffer> Result2 : register(u1);


//�X���b�h�O���[�v�̐��iCPU���Ńf�B�X�p�b�`�������j
//#define THG_X 10

//�X���b�h�̐�
#define TH_X 200
#define TH_Y 1
#define TH_Z 1



// �V�F�[�_�֐�
[numthreads(TH_X, TH_Y, TH_Z)]   // �X���b�h �O���[�v�̃X���b�h��
void main(uint3 Gid : SV_GroupID,//�O���[�vID�@�f�B�X�p�b�`���Ŏw��
//	uint3 DTid : SV_DispatchThreadID,
uint3 GTid : SV_GroupThreadID//�X���b�hID�@�����Ŏw��
//	uint GI : SV_GroupIndex
)
{
	uint  x = GTid.x;
	uint  y = Gid.x;
	//	uint  CULMS_NO = TH_X;

	uint node = TH_X * y + x;
	float3 myLocation = Input[node].Position;//�����̈ʒu
	float3 myVelocity = Input[node].Velocity;
	float3 mySpeed = Input[node].Speed;
	float4 mySize = Input[node].ParticleSize;
	float  myTimer = Input[node].Timer;
	float3 P = 0;
	float3 Dir = 0;
	float pdist = 0;
	float3 N = 0;
	switch (Input[node].ParticleType) {
		//sincos()
	case SLASHING:
		break;
	case DIFFUSION:
		myVelocity.y += mySpeed * cos(myTimer);
		myVelocity = normalize(myVelocity);
		myLocation.x = myLocation.x + myVelocity.x * mySpeed;
		myLocation.y = myLocation.y + myVelocity.y * mySpeed;
		myLocation.z = myLocation.z + myVelocity.z * mySpeed;
		myTimer += 0.12;
		mySpeed -= mySpeed * 0.01;
		break;
	case CONVERGENCE:
		Dir = myVelocity - myLocation;
		pdist = length(Dir);
		Dir = normalize(Dir);
		if (pdist < 0.2) {
			mySize = float4(0, 0, 0, 0);
			break;
		}
		myLocation.x = myLocation.x + Dir.x * mySpeed;
		myLocation.y = myLocation.y + Dir.y * mySpeed;
		myLocation.z = myLocation.z + Dir.z * mySpeed;

		myTimer += 0.1;
		//mySpeed += 0.01;
		break;
	case SLAH:
		mySize.y += 0.4;
		mySize.x -= 0.02;
		if (mySize.x < 0) {
			mySize.x = 0;
		}
		myTimer += 0.1;
		break;
	case EXPANSION:
		mySize.y += mySpeed;
		mySize.x += mySpeed;
		mySize.w += mySpeed;


		myTimer += 0.1;
		//float c = 0;
		//float s = 0;
		//sincos(myTimer, s, c);
		//myVelocity.x = s;
		//myVelocity.y = c;
		break;
	case WEPONCONVERGENCE:
		Dir = Tail - Pattern;
		pdist = length(Dir) / Max;
		Dir = normalize(Dir);

		P.x = Pattern.x + Dir.x * (pdist * node);
		P.y = Pattern.y + Dir.y * (pdist * node);
		P.z = Pattern.z + Dir.z * (pdist * node);
		N = P - myLocation;
		pdist = length(N);
		if (pdist < 0.1) {
			mySize = float4(0, 0, 0, 0);
			break;
		}
		N = normalize(N);

		myLocation.x = myLocation.x + N.x * mySpeed;
		myLocation.y = myLocation.y + N.y * mySpeed;
		myLocation.z = myLocation.z + N.z * mySpeed;
		mySpeed +=  0.001;
		myTimer += 0.01;
		break;
	case CORRUPTION:
		
		Dir = Tail - Pattern;
		Dir = normalize(Dir);
		
		P.x = Pattern.x + Dir.x * mySize.z;
		P.y = Pattern.y + Dir.y * mySize.z;
		P.z = Pattern.z + Dir.z * mySize.z;
		if (myTimer == 0) {
			myLocation = P;
		}
		Dir = P - myLocation;
		pdist = length(Dir);
		myLocation.x = myLocation.x + myVelocity.x * mySpeed;
		myLocation.y = myLocation.y + myVelocity.y * mySpeed;
		myLocation.z = myLocation.z + myVelocity.z * mySpeed;
		myTimer++;
		if (pdist > 5) {
			myTimer = 0;
		}
		break;
	case CHILE:
		
		myLocation.y  += mySpeed * 0.001;

		if (myLocation.y  < -3) {
			myLocation.y = 10;
		}
		myTimer += 0.01;
		break;
	}
//	const float length = 0.2f;//�߂��̓_�̔��f�p
	
	

	  
	Result[node].Speed = mySpeed;
	Result[node].Velocity = myVelocity;
	Result[node].Position = myLocation;
	Result[node].ParticleSize = mySize;
	Result[node].ParticleType = Input[node].ParticleType;
	Result[node].Timer = myTimer;
	if (node == 1) {
		//Result2[0].toCpuNo = 123;          //��ԑ����ꏊ
		//Result2[0].toCpuPosition = Total_Pos / (float)count;
		//	Result2[0].toCpuPosition.x = 0;      // ���W�l
		//	Result2[0].toCpuPosition.y = 0;      // ���W�l
		//	Result2[0].toCpuPosition.z = 0;      // ���W�l
	}
}