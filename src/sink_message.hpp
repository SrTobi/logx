#include <unordered_map>
#include <typeindex>
#include "logx/sink.hpp"
#include "logx/details/base_message.hpp"

namespace logx {

	namespace impl {

		class sink_message_impl : public sink_message
		{
		public:
			sink_message_impl(details::message_base* message, const std::unordered_map<std::type_index, std::shared_ptr<tag>>& _default_tags);


			virtual const string& msg() const override;
			virtual const std::vector<string>& args() const override;
			virtual const std::vector<const tag*>& tags() const override;

		protected:
			virtual const tag* _get_tag(const std::type_info& _ty) const override;

		private:
			details::message_base* mMessage;
			string mDescription;
			std::vector<const message_arg*> mArgs;
			std::vector<string> mArgsAsString;
			mutable std::vector<const tag*> mTagList;
			std::unordered_map<std::type_index, const tag*> mTags;
		};
	}
}