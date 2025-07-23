#include "DirectX.h"
#include <d3d11.h>
#include <wrl/client.h> // �}�C�N���\�t�g���񋟂���X�}�[�g�|�C���^

// =======================================
// DirectX ����
// =======================================

namespace DirectX11 {

	// =====================================================
	// ����������
	// =====================================================
	void Init(uint16_t Width, uint16_t Height)
	{
		unsigned int createDeviceFlags = 0; // �f�o�C�X�쐬�p�̃t���O
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG; // �f�o�b�O���Ƀf�o�b�O���C���[��L���@�i�r�b�gOR�Ł{����j
#endif

		// �h���C�o�[�̗D��x���쐬�@(���ɔz��) ------------------------------------------------
		// �g�p�\�ȃh���C�o�[�^�C�v�����Ɏ������߂̔z�� (�D�揇�ʏ�)
		D3D_DRIVER_TYPE driverTypes[] = {
			D3D_DRIVER_TYPE_HARDWARE,  // GPU���g�p���č����`��
			D3D_DRIVER_TYPE_WARP,      // CPU��GPU�̑�����s���B�i�}���`�X���b�h�̂��߁A���������_�����O�j
			D3D_DRIVER_TYPE_REFERENCE, // CPU�ŕ`�悷��\�t�g�E�F�A�����_�����O�@�i�P�X���b�h�̂��ߒx���A�J���E���،����j
		};
		unsigned int numDriverTypes = sizeof(driverTypes) / sizeof(D3D_DRIVER_TYPE); // ���[�v����񐔂��擾����

		// �@�\�̃��x���̗D��x���쐬 ------------------------------------------------------------
		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_0, // DirectX11�Ή� (�R���s���[�g�V�F�[�_�[�Ȃǂ��g�p�\)
			
			// �����Ή��ł���悤�ɂ���ꍇ�V�F�[�_�[��ʂɏ����Ȃ��Ƃ����Ȃ�
			// D3D_FEATURE_LEVEL_10_1, �R���s���[�g�V�F�[�_�[�Ȃǂ͎g�p�ł��Ȃ�
			// D3D_FEATURE_LEVEL_10_0,
		};
		unsigned int numFeatureLevels = sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL); // ���[�v����񐔂��擾����

		// �X���b�v�`�F�C���쐬 -------------------------------------------------------------------
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC)); // 0�������āA����������
		swapChainDesc.BufferCount = 2; // �_�u���o�b�t�@
		swapChainDesc.BufferDesc.Width = Width;   // ��ʂ̏c��
		swapChainDesc.BufferDesc.Height = Height; // ��ʂ̉���
	}



}