
#include "input_layout.h"
#include "exceptions/gfx_exception.h"


void	pulsar::input_layout::free()
{
	if (this->mp_layout)
	{
		this->mp_layout->Release();
		this->mp_layout = NULL;
	}
}

void	pulsar::input_layout::create_input_layout(ID3DBlob *shader_blob, const D3D11_INPUT_ELEMENT_DESC *ied, UINT ied_num_elements)
{
	GFX_ASSERT(pulsar::gfx::instance().device()->CreateInputLayout(ied, ied_num_elements, shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), &this->mp_layout));
}

void	pulsar::input_layout::set(ID3DBlob *shader_blob, const D3D11_INPUT_ELEMENT_DESC *ied, UINT ied_num_elements)
{
	this->free();
	this->create_input_layout(shader_blob, ied, ied_num_elements);
}
