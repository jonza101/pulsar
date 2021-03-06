#pragma once

#include "dcb/types.h"

#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>


namespace pulsar
{
	namespace dcb
	{
		class layout_elem
		{
		private:
			friend class raw_layout;
			friend class baked_layout;
			friend class layout_elem_ref;

			struct extra_data
			{
				std::unordered_map<std::string, int>	m_keys;
				std::vector<pulsar::dcb::layout_elem>	m_elems;
			};

		private:
			pulsar::dcb::TYPE	m_type = pulsar::dcb::Empty;
			std::shared_ptr<extra_data>	m_extra_data;
			size_t	m_offset = 0ull;

		private:
			layout_elem() = default;
			layout_elem(pulsar::dcb::TYPE type);

			static pulsar::dcb::layout_elem		&get_empty_elem();
			void	add_data(pulsar::dcb::TYPE type, const std::string &key);
			bool	validate_key(const std::string &key);

			bool	crosses_boundary(size_t offset, size_t size) const noexcept;
			size_t	pad_boundary(size_t offset) const noexcept;
			size_t	pad_if_crosses_boundary(size_t offset, size_t size) const noexcept;

			// OFFSET BASED, FUNCTIONS WORK ONLY AFTER FINALIZATION
			size_t	get_offset_begin() const noexcept;
			size_t	get_offset_end() const noexcept;

			size_t	size() const noexcept;

			size_t	finalize(size_t offset_in);
			size_t	finalize_struct(size_t offset_in);

			public:
				pulsar::dcb::layout_elem	&operator[](const std::string &key);
				const pulsar::dcb::layout_elem	&operator[](const std::string &key) const;

				// ADD AND RETURN PARENT LAYOUT_ELEM
				layout_elem		&add(pulsar::dcb::TYPE type, const std::string &key);

				// ADD AND RETURN NEW LAYOUT_ELEM
				template <pulsar::dcb::TYPE type>
				layout_elem		&add(const std::string &key)
				{
					this->add_data(type, key);
					return (this->m_extra_data->m_elems.back());
				}

				bool	exists() const;
		};
	}
}
