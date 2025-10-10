#pragma once

// =================================
// �y�N���X�T�v�z
// DirectX�̃o�b�t�@�֘A�̗񋓌^�ϊ����s�����[�e�B���e�B�N���X
//
// *����
// �d�����w�b�_�[���C���N���[�h���Ă��܂�
// �w�b�_�[�ɃC���N���[�h�񐄏�
// =================================


// =================================
// �w�b�_�[
// =================================
#include <d3d11.h>
#include "GraphicsEnums.h"


// =================================
// �N���X
// =================================
class DirectX_FormatConverter
{
public:
    // �g�p�ړI�ɕϊ�
    static D3D11_USAGE ToDXUsage(BufferUsage usage);

    // �A�N�Z�X�����ɕϊ�
    static UINT ToDXCPUAccess(CPUAccess access);

    // ����Format ���� DXGI_FORMAT �ɕϊ�
    static DXGI_FORMAT ToDXFormat(Format format);

    // DXGI_FORMAT ���� ����Format �ɕϊ�
    static Format ToSelfFormat(DXGI_FORMAT format);

    // �T���v���[�ϊ�
    static D3D11_FILTER ConvertFilter(SamplerFilter filter);
    static D3D11_TEXTURE_ADDRESS_MODE ConvertAddressMode(SamplerAddressMode mode);
    static D3D11_COMPARISON_FUNC ConvertComparisonFunc(SamplerComparisonFunc func);
};

