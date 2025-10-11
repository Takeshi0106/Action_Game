#include "GameMain.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Color.h"
#include "Timer.h"


// ==========================================
// �\���́@�f�o�b�O�p
// ==========================================
/*
struct Vertex {
	Vector3 pos;
	Color color;
};
struct TransformCB
{
	Matrix4x4 WorldMatrix;
	Matrix4x4 ViewMatrix;
	Matrix4x4 ProjMatrix;
};
// �O�p�`�̒��_
Vertex vertices[3] =
{
{ { 0.0f, 0.57735f, 0.0f }, {1, 0, 0, 1} },   // �㒸�_ (y = ��3/2 * 0.5)
{ { 0.5f, -0.288675f, 0.0f }, {0, 1, 0, 1} }, // �E�����_
{ { -0.5f, -0.288675f, 0.0f }, {0, 0, 1, 1} } // �������_
};
float angle;


void GameMain::Init(BaseDrawManager* _drawManager)
{
	m_DrawManager = _drawManager;

	// �萔�o�b�t�@������
	// ���[���h�s��
	Matrix4x4 world = Matrix4x4::CreateIdentityMatrix();

	// �r���[�s��i�J���������������j
	Vector3 eye(0, 0, -5);   // �J�����ʒu
	Vector3 at(0, 0, 0);     // �����_
	Vector3 up(0, 1, 0);     // �����
	Matrix4x4 view = Matrix4x4::CreateViewMatrix_LH(eye, at, up);

	// �v���W�F�N�V�����s��i�������e�j
	float fov = 3.14159265f / 4.0f;     // ����p45��
	float aspect = 1280.0f / 720.0f;
	float nearZ = 0.1f;
	float farZ = 100.0f;
	Matrix4x4 proj = Matrix4x4::CreateProjectionMatrix_LH(fov, aspect, nearZ, farZ);

	TransformCB mat = { world.toGPU(), view.toGPU(),proj.toGPU() };


	// ���_�o�b�t�@�쐬
	m_DrawManager->CreateVertexBuffer(
		"VS_TriangleDebug",
		vertices,
		sizeof(Vertex),
		PrimitiveType::TriangleStrip,
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// �萔�o�b�t�@�쐬
	m_DrawManager->CreateConstantBuffer(
		"Transform1",
		&mat,
		sizeof(mat),
		BufferUsage::Dynamic,
		CPUAccess::Write);


	Timer::Init(); // �^�C�}�[������
	Timer::Start(); // �^�C�}�[�J�n
}
*/