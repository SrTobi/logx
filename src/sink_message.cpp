#include <assert.h>
#include "logx/config.hpp"
#include "sink_message.hpp"




namespace logx {

	namespace impl {

		sink_message_impl::sink_message_impl(details::message_base* message, const std::unordered_map<std::type_index, std::shared_ptr<tag>>& _default_tags)
			: mBaseMessage(message)
		{
			mMessage = mBaseMessage->msg();
			mTagList = mBaseMessage->tags();
			mAnnotations = mBaseMessage->annotations();

			for (auto tg : mTagList)
			{
				assert(tg);
				
				mTags[typeid(*tg)] = tg;
			}
		}


		const string& sink_message_impl::msg() const
		{
			return mMessage;
		}

		const std::vector<const tag*>& sink_message_impl::tags() const
		{
			return mTagList;
		}

		const std::vector<const annotation*>& sink_message_impl::annotations() const
		{
			return mAnnotations;
		}

		const tag* sink_message_impl::_get_tag(const std::type_info& _ty) const
		{
			auto it = mTags.find(_ty);
			return it == mTags.end() ? nullptr : it->second;
		}
	}
}
