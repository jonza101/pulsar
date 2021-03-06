#pragma once

#include <string>


namespace pulsar
{
	class bindable;
	class buffer_resource;

	namespace rg
	{
		class source
		{
		private:
			std::string m_name;

		protected:
			source(const std::string &name);

		public:
			source &operator=(const source&) = delete;
			source &operator=(source&&) = delete;
			source(const source&) = delete;
			source(source&&) = delete;
			source() = delete;
			virtual ~source() = default;

			const std::string	&name() const;

			virtual pulsar::bindable	**yield_bindable();
			virtual pulsar::buffer_resource		**yield_buffer_resource();

			virtual void	validate() const;
		};
	}
}
