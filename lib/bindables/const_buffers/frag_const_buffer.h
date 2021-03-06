#pragma once

#include "const_buffer.h"


namespace pulsar
{
	template <typename T>
	class frag_const_buffer : public pulsar::const_buffer<T>
	{
	public:
		frag_const_buffer<T> &operator=(const frag_const_buffer<T>&) = delete;
		frag_const_buffer<T> &operator=(frag_const_buffer<T>&&) = delete;
		frag_const_buffer(const frag_const_buffer<T>&) = delete;
		frag_const_buffer(frag_const_buffer<T>&&) = delete;
		frag_const_buffer() = default;
		frag_const_buffer(UINT slot) : pulsar::const_buffer<T>(slot){ }
		frag_const_buffer(const T &data, UINT slot = 0u) : pulsar::const_buffer<T>(data, slot){ }
		~frag_const_buffer() = default;

		void	bind() const
		{
			pulsar::gfx::instance().device_context()->PSSetConstantBuffers(this->m_slot, 1u, &this->mp_buffer);
		}
		void	unbind() const
		{
			ID3D11Buffer *nullPtr[1] = { NULL };
			pulsar::gfx::instance().device_context()->PSSetConstantBuffers(this->m_slot, 1u, nullPtr);
		}
	};
}
