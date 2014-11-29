#include "formatter.hpp"
#include "sink_message.hpp"




namespace logx {

	namespace impl {

		sink_message_impl::sink_message_impl(details::message_base* message, const std::unordered_map<std::type_index, std::shared_ptr<tag>>& _default_tags) : mMessage(message)
		{
			mArgs = mMessage->args();

			mArgsAsString.reserve(mArgs.size());
			for (auto arg : mArgs)
			{
				mArgsAsString.emplace_back(arg->to_string());

				auto* asTag = arg->as_tag();
				if (asTag)
					mTags.emplace(typeid(*asTag), asTag);
			}

			for (auto& e : _default_tags)
			{
				if (!mTags.count(e.first))
				{
					mTags.emplace(e.first, e.second.get());
				}
			}

			mDescription = format(mMessage->msg(), mArgsAsString);
		}


		const string& sink_message_impl::msg() const
		{
			return mDescription;
		}

		const std::vector<string>& sink_message_impl::args() const
		{
			return mArgsAsString;
		}

		const std::vector<const tag*>& sink_message_impl::tags() const
		{
			if (mTagList.size() != mTags.size())
			{
				for (auto& e : mTags)
				{
					mTagList.push_back(e.second);
				}
			}
			return mTagList;
		}

		const tag* sink_message_impl::_get_tag(const std::type_info& _ty) const
		{
			auto it = mTags.find(_ty);
			return it == mTags.end() ? nullptr : it->second;
		}

	}
}
